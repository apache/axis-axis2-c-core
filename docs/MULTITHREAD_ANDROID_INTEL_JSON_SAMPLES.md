# Multithreading the Financial Benchmark Monte Carlo: x86-64 VMs and Android

**Status: a proposal for future consideration. Nothing here is implemented.**

The `monteCarlo` operation of the financial benchmark sample
(`samples/user_guide/financial-benchmark-service`) runs every simulation on one
thread. This note records what that costs on a typical x86-64 server VM and on
a typical Android phone, how a multithreaded engine could be added without
losing reproducibility, and why it has not been done yet.

---

## 1. Where the time goes today

For each path and each step the kernel draws a standard normal and takes one
`exp`:

- `rand_normal` is Box–Muller over `xorshift128+`: one `log`, one `sqrt` and one
  `cos` per draw. Box–Muller produces two normals per pair of uniforms; the
  second (`sin`) is discarded.
- The step multiplies the path value by `exp(drift + σ√dt·Z)`, or by the
  correlated form `exp(drift_i + Σ L_ik √dt Z_k)` for a book.
- After the loop, `qsort` over the final values for the percentiles, and two
  passes for the moments.

A 1,000,000-path, 252-step run is about 252 million normal draws and
exponentials. The service starts no threads, so all of it is one core.

## 2. One core against one core

Measured 2026-09-26 on the same commit at `-O2`, scalar pandemic run
(σ = 89.6 %, μ = −20 %, 1,000,000 paths, 252 steps), as reported in
`simulations_per_second`:

| | x86-64 server VM | Android phone |
|---|---|---|
| Machine | KVM guest, 4 vCPUs, 64 GB | Pixel 10 Pro XL |
| CPU | AMD EPYC 7542 (Zen 2, 2019), 2.9 GHz base, 3.4 GHz boost | Google Tensor G5 (2025): 1 core at 3.78 GHz, 5 at 3.05 GHz, 2 at 2.25 GHz |
| Toolchain | GCC 11, glibc | NDK clang, bionic |
| Paths per second | ~86,500 (within 1 % across runs) | ~200,000–236,000 (varies with temperature) |

The phone is about 2.5× faster on this operation, and the reason is not
mysterious: a single-threaded loop runs on one core, so the VM's other vCPUs
and its memory do not take part, and one 2025 phone core is being compared
with one 2019 server core behind a hypervisor. Part of the gap is probably
the compiler and maths library (clang against GCC 11, bionic's `exp`/`log`
against glibc's); that share has not been measured.

The comparison reverses for small, memory-bound work: `portfolioVariance` on a
95-asset matrix takes about 8 µs on the VM and 12–15 µs on the phone.

## 3. What each platform would gain from threads

**x86-64 VM.** Up to the vCPU count (4 here), close to linearly for this
embarrassingly parallel loop. Two limits:

- *The cores are already shared.* Under Apache httpd each request runs on a
  worker thread, and the MPM already spreads concurrent requests across
  cores. Threads inside one request compete with other requests. They help
  latency when requests are few, and help nothing under full load.
- *vCPUs are not dedicated cores.* Steal time from neighbouring guests, and
  turbo frequencies that drop as more cores are busy, make scaling below
  linear and vary run to run.

**Android.** More cores, but not equal ones, and not for long:

- *Heterogeneous cores.* A job split evenly finishes when the slowest share
  finishes; chunks on a small core take several times longer than on the
  prime core. The work has to be handed out in small blocks that fast cores
  take more of, not in equal slices.
- *Thermal limits.* A phone sustains all cores at full clock for seconds, not
  minutes; long runs throttle. A single-threaded 1M-path run already varies
  about 15 % with the phone's temperature.
- *Power state.* With the screen off, Doze lowers clocks: the same request has
  been measured 5–10× slower. Threads do not change that.
- Nothing in Android prevents threads: POSIX threads work in native code as on
  Linux, and the phone's httpd already runs its requests on worker threads.

Expected result: roughly 3–4× on the VM, perhaps 3–6× on the phone for short
runs, less when hot.

## 4. A design that keeps results reproducible

Today the same `random_seed` gives the same answer on every machine; published
numbers depend on it. A parallel engine must give the same answer **whatever
the thread count**, and so cannot share one random stream between threads.

1. **Fixed blocks.** Split the paths into blocks of a fixed size (say 4,096),
   independent of the number of threads.
2. **One stream per block.** Seed block *b* from `(random_seed, b)` through a
   mixing function (splitmix64 is the usual choice for seeding xorshift), so
   block *b* draws the same numbers whichever thread runs it.
3. **Threads take blocks from a counter.** A shared atomic index; each thread
   takes the next block, writes its final values into that block's slots of
   `final_values`, and keeps per-block partial sums, profit counts and maximum
   drawdown.
4. **Combine in block order.** After the join, reduce the partials in
   ascending block order, so floating-point sums are identical for 1, 2 or 8
   threads. Then sort and compute percentiles as today.

With this, a run is reproducible across thread counts and machines. It is
**not** reproducible against today's single stream: the draws differ, so every
Monte Carlo result changes. That is why it should be a separate engine, not a
replacement:

```json
{"n_simulations": 1000000, "engine": "parallel", "max_threads": 8}
```

- `engine` absent or `"sequential"`: today's code, unchanged, the reference.
- `max_threads`: a cap, further bounded by the online core count and by a
  service-wide limit, so one request cannot take every core from the others.
- Below a threshold (say 50,000 paths) stay sequential: thread start-up would
  cost more than it saves.
- Threads are created per request and joined before the response, keeping
  the service stateless; a persistent pool would be an optimisation for later.

## 5. Speed-ups that do not need threads

| Change | Likely gain | Changes results? |
|---|---|---|
| Use both Box–Muller outputs (cache the `sin` value) | ~1.5× | yes |
| Ziggurat or another faster normal generator | ~2–3× | yes |
| `-O3`, keeping strict IEEE semantics (no `-ffast-math`) | 0–10 % | no |
| `-ffast-math` | varies | yes, and not reproducible across compilers |

The first two change the draw sequence, so they belong in the new engine, not
in the sequential reference.

## 6. How to measure it

- Paths per second for 1, 2, 4 and all threads, on both platforms, with the
  phone on power and warmed up, three runs each.
- Determinism: the same seed with 1, 2, 3 and 8 threads must give identical
  output to the last digit.
- ThreadSanitizer on the x86-64 build for races in the block counter and the
  partial results; AddressSanitizer as in `ASAN_GDB_JSON_SAMPLES.md`.
- Latency under concurrent load: with 20 requests in flight, compare
  `sequential` against `parallel`, to confirm intra-request threads do not
  slow the server as a whole.

## 7. Why it waits

- The sequential engine is a like-for-like benchmark: the same algorithm runs
  in other implementations, and its paths per second are meant as a proxy for
  scalar floating-point speed. An optimised C engine compared with an
  unoptimised one elsewhere measures the optimisation, not the platform.
  Keeping the sequential path as the reference preserves that comparison.
- Every published Monte Carlo figure depends on the current draw order;
  changing the default would invalidate them. A new engine alongside the old
  one avoids that.
