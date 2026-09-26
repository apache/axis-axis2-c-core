# ASan and gdb with the JSON Samples under Apache httpd

How to run an Axis2/C JSON service (the samples under `samples/user_guide`,
for example FinancialBenchmarkService) under AddressSanitizer inside Apache
httpd, find memory errors and leaks, and use gdb where ASan alone cannot see.
Written against Ubuntu 22.04 with the packaged `apache2`, GCC 11 and APR 1.7;
the steps carry over to other distributions with different paths.

The approach in one line: build Axis2/C and the service with ASan into a
private prefix, run them in a **separate, single-process httpd** with the ASan
runtime preloaded, and never touch the system Apache.

---

## 1. Build with ASan into a private prefix

Start from a clean tree (a fresh `git archive` plus `./autogen.sh`, or a fresh
checkout). Reusing a tree that was built without ASan leaves stale objects that
`make` considers current.

```bash
export CFLAGS="-g -O1 -fsanitize=address -fno-omit-frame-pointer"
export LDFLAGS="-fsanitize=address"
./configure --prefix=$HOME/a2c-asan --enable-json=yes --enable-http2 \
    --with-apache2=/usr/include/apache2 --with-apr=/usr/include/apr-1.0 \
    APACHE2_HOME=/usr
make -j"$(nproc)" && make install
```

`--enable-http2` is required for the JSON path. `-fno-omit-frame-pointer` keeps
ASan on its fast frame-pointer unwinder (see §6).

Then the service, with `AXIS2C_HOME` exported before `configure`:

```bash
export AXIS2C_HOME=$HOME/a2c-asan
cd samples && ./configure --prefix=$AXIS2C_HOME --enable-json --enable-http2
cd user_guide/financial-benchmark-service
make libfinancial_benchmark_service.la
S=$AXIS2C_HOME/services/FinancialBenchmarkService
mkdir -p $S && cp .libs/libfinancial_benchmark_service.so services.xml $S/
```

Build the library target only. Under ASan the sample's MCP stdio binary fails
to link (`axis2_http_*` undefined through `libaxis2_engine`), and it is not
needed to test the service in httpd. In an `--enable-http2` build,
`make -C user_guide` as a whole also fails, in `user_guide/clients` (SOAP
clients need the HTTP/1.1 sender); build the service directory alone.

Check the result is instrumented:

```bash
ldd $AXIS2C_HOME/lib/libmod_axis2.so | grep asan          # libasan.so.6
nm -D $S/libfinancial_benchmark_service.so | grep -c __asan
```

## 2. A separate httpd instance

`LoadModule` is global to an httpd, so the ASan `mod_axis2` needs its own
instance: its own `Listen`, `PidFile`, logs and `ServerRoot`.

```apache
ServerName a2c-asan
Listen 8097
User www-data
Group www-data
PidFile /home/you/a2c-asan-rig/run/httpd.pid
ErrorLog /home/you/a2c-asan-rig/logs/error.log
LogLevel info

LoadModule mpm_event_module  /usr/lib/apache2/modules/mod_mpm_event.so
LoadModule authz_core_module /usr/lib/apache2/modules/mod_authz_core.so
LoadModule http2_module      /usr/lib/apache2/modules/mod_http2.so

LoadModule axis2_module /home/you/a2c-asan/lib/libmod_axis2.so
# The JSON receiver finds <serviceclass>_invoke_json with dlsym(RTLD_DEFAULT),
# which sees only libraries loaded RTLD_GLOBAL. LoadFile loads the service
# that way; it must follow mod_axis2, whose libraries the service needs.
LoadFile /home/you/a2c-asan/services/FinancialBenchmarkService/libfinancial_benchmark_service.so
Axis2RepoPath /home/you/a2c-asan
Axis2LogFile /home/you/a2c-asan-rig/logs/axis2.log
Axis2LogLevel info

LimitRequestBody 0
Protocols h2c http/1.1
H2Direct on

<Location /services>
    SetHandler axis2_module
    Require all granted
</Location>
```

Notes:

- Without the `LoadFile`, every call answers
  `Service 'FinancialBenchmarkService' has no _invoke_json handler`, and
  `axis2.log` shows `financial_benchmark_service_invoke_json not found via
  dlsym`. That is the rig, not the service.
- On Ubuntu, `unixd_module` and `log_config_module` are built into `apache2`;
  do not `LoadModule` them.
- `logs/` and `run/` must be owned by `www-data`.
- `h2c` keeps TLS out of the picture; call it with
  `curl --http2-prior-knowledge`.

## 3. Run it under ASan

The packaged `apache2` binary is not instrumented, so the ASan runtime must be
loaded first with `LD_PRELOAD`, or ASan refuses to start ("ASan runtime does not
come first in initial library list"). Run single-process with `-X`, so one
process holds all the state and the report comes from it:

```bash
R=/home/you/a2c-asan-rig
sudo env LD_PRELOAD=/lib/x86_64-linux-gnu/libasan.so.6 \
  ASAN_OPTIONS=detect_leaks=1:halt_on_error=0:allow_user_segv_handler=0:handle_segv=1:malloc_context_size=25:log_path=$R/logs/asan \
  nohup apache2 -X -d $R -f $R/httpd.conf > $R/stdout.log 2>&1 &
```

`apache2 -t` fails without the preload for the same reason; that is expected.

What the options do here:

| Option | Why |
|---|---|
| `log_path=…/asan` | Reports go to `asan.<pid>`, not to a detached stderr. The directory must be writable by the user httpd runs as. |
| `allow_user_segv_handler=0`, `handle_segv=1` | `mod_axis2` installs its own SIGSEGV handler, which logs `CRASH DETECTED` and exits. Without these, it runs instead of ASan's and the ASan stack is lost. |
| `halt_on_error=0` | Keep serving after a non-fatal report. |
| `malloc_context_size=25` | Deeper allocation stacks in leak reports. |
| `detect_leaks=1` | LeakSanitizer at exit (the default on x86_64 Linux; explicit here). |

Drive it with the same requests as any other test. `curl` posts one body to
many URLs on a single HTTP/2 connection, which makes thousands of requests
cheap:

```bash
U=http://127.0.0.1:8097/services/FinancialBenchmarkService/portfolioVariance
args=(); for i in $(seq 1000); do args+=(-o /dev/null "$U"); done
curl -s --http2-prior-knowledge -H 'Content-Type: application/json' \
     --data-binary @body.json -w '%{http_code}\n' "${args[@]}" | sort | uniq -c
```

Stop it with `sudo kill -TERM $(cat $R/run/httpd.pid)`. LeakSanitizer runs as
the process exits and writes `asan.<pid>` only if it finds something.

## 4. Leaks: prove LeakSanitizer ran

"No report" means either no leaks or LeakSanitizer never ran. Tell them apart
by planting a leak through gdb before stopping:

```bash
P=$(cat $R/run/httpd.pid)
sudo gdb -p $P -batch -q -ex 'call (void*)malloc(12345)' -ex detach
sudo kill -TERM $P
grep -E '^(Direct|Indirect) leak|^SUMMARY' $R/logs/asan.*
# Direct leak of 12345 byte(s) in 1 object(s) allocated from: ...
```

If the planted leak is reported and nothing else is, the requests leaked
nothing that LeakSanitizer can see.

**LeakSanitizer does not run while a debugger is attached** (it needs ptrace
itself to stop the threads), so always `detach` before the process exits.

## 5. Growth that is not a leak: live heap bytes

LeakSanitizer reports only memory nothing points to. A structure that grows
with every request but stays reachable is not a leak to it, and RSS under ASan
is dominated by the quarantine of freed blocks. Ask the allocator how many
bytes are live, after a warm-up and again after many more requests:

```bash
heap() { sudo gdb -p $P -batch -q \
  -ex 'print (unsigned long)__sanitizer_get_current_allocated_bytes()' \
  -ex detach 2>/dev/null | grep -oE '= [0-9]+' | tr -d '= '; }
# send 1,000 requests;  h1=$(heap)
# send 5,000 more;      h2=$(heap)
echo $(( (h2 - h1) / 5000 )) bytes per request
```

A flat number with rising RSS in the ordinary (non-ASan) build points at the
C library's allocator keeping freed memory per thread, not at Axis2/C.

Limits: this counts `malloc` memory. APR pools obtained with `mmap` (Ubuntu's
APR allocator) are invisible to both this count and ASan's use-after-free
detection.

## 6. Crashes ASan cannot explain: gdb

When ASan's report ends in `<empty stack>` or the process dies before it can
report, catch the signal in gdb instead. The event MPM uses signals for its
own shutdown, so pass everything except SIGSEGV:

```bash
P=$(cat $R/run/httpd.pid)
( sleep 4; sudo kill -TERM $P ) &
sudo gdb -p $P -batch -q \
  -ex 'handle all nostop noprint pass' \
  -ex 'handle SIGSEGV stop print nopass' \
  -ex continue -ex 'info symbol $pc' -ex 'x/2i $pc' -ex 'bt 25' -ex kill
```

Two traps found this way:

- **`fast_unwind_on_malloc=0` crashes at exit.** With the slow unwinder, a
  `free` during `dlclose` of a module at exit walks into `_Unwind_Backtrace`
  on a library that is being unmapped, and ASan itself faults. Keep the
  default fast unwinder and build with `-fno-omit-frame-pointer`.
- **Do not `pkill -f` a pattern that appears in your own command line.** Over
  ssh, `pkill -f httpd.conf` matches the remote shell running it. Stop by PID
  from the `PidFile`.

## 7. A child that crashes in teardown

At the time of writing, an Apache child can crash in Axis2's own teardown
(`axis2_shutdown`, run from pool cleanup) as it exits. That is tracked
separately. It stops LeakSanitizer from running, because the process dies on a
signal instead of exiting. To take a leak report past it, make
`axis2_shutdown` return immediately in the running process, then detach and
stop as usual:

```bash
sudo gdb -p $P -batch -q \
  -ex 'set {unsigned char[3]}axis2_shutdown = {0x31, 0xc0, 0xc3}' \
  -ex 'x/3bx axis2_shutdown' -ex detach          # xor eax,eax; ret (x86_64)
sudo kill -TERM $P
```

Skipping the teardown does not hide leaks from requests: the long-lived Axis2
objects remain reachable from `mod_axis2`'s globals and are not reported, and
anything a request allocated and lost is still unreachable and is.

## 8. Checklist

1. Clean tree; `CFLAGS`/`LDFLAGS` with `-fsanitize=address -fno-omit-frame-pointer`.
2. Service built as its library target and copied into `services/<Name>/`.
3. Separate httpd: own port, pid file and logs; `LoadFile` of the service after `mod_axis2`.
4. `LD_PRELOAD=libasan`, `-X`, `allow_user_segv_handler=0`, `log_path` writable.
5. Smoke test: one request answers `"status":"SUCCESS"`.
6. Load; then plant a leak, detach, `SIGTERM`, read `asan.<pid>`.
7. For growth: live heap bytes after warm-up and after N more requests.
8. For a crash with no stack: gdb with `handle all nostop` and SIGSEGV stopping.
