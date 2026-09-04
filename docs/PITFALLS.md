# Pitfalls

Things that are true about this codebase and that the compiler will not tell
you. Each one cost someone real time. They are here so the next person spends
that time on something else.

---

## Ownership and memory

### A hash owns the keys you give it

Since AXIS2C-1632, `axutil_hash_set` with `AXIS2_HASH_KEY_STRING` **copies** the
key, and `axutil_hash_free` releases the copy. Deleting an entry frees it too.

Four teardowns had not caught up and were freeing keys before the hash did,
double-freeing every one. They are fixed, but learn the shape rather than the
sites: **any call site that frees a key it put into a hash is now wrong**, and
nothing in the type system will say so.

### A free function can outlive the library that defines it

A property's `free_func` is a pointer into whichever library defined it. If that
library is `dlclose`d before the property is released, calling it is a jump into
unmapped memory — a wild jump, not a double free, and it does not look like a
memory bug in a debugger.

This bit the HTTP sender: it stored the client in a property whose `free_func`
lives in `libaxis2_http_sender`, and that property is released by
`axis2_options_free`, which runs *after* `axis2_conf_ctx_free` has closed the
library. Transport descriptors now detach rather than close, so the code stays
mapped. Services and modules still unload through their own paths.

Two traps found while fixing it, both still worth knowing:
`axutil_dll_desc_set_dl_handler` used to reject NULL, which made detaching
impossible and turned the first fix into a silent no-op; and the same setter
called `AXIS2_FREE` on a handle that came from `dlopen`.

### `mime_parts_map` is transferred, not borrowed

`axiom_mime_parser_parse_for_attachments` hands its parts map to the caller, and
`axiom_mime_parser_free` deliberately does not free it. Production callers keep
it and free the parser separately. A unit test that frees only the parser leaks
the map, and that is the test's bug, not the library's.

### Under `mod_axis2`, `free` does nothing

`axis2_module_free` has no body outside the `APR_HAS_SHARED_MEMORY` branch, and
`AXIS2_MALLOC` is `apr_palloc` from a pool created in the `child_init` hook —
its own comment says the pool "lasts the lifetime of the httpd server child
process". `local_pool`, `current_pool` and `global_pool` are all that same pool,
so `axutil_allocator_switch_to_*_pool` are no-ops there too.

Consequences, both easy to get backwards:

- **There is no per-request reclaim.** Memory is returned when the child process
  exits, not when the request ends. Growth is bounded by
  `MaxConnectionsPerChild`, and the shipped `docs/userguide/httpd.conf` sets that
  to `0` — never recycle.
- **A double free in code reached only through `mod_axis2` is a no-op there**,
  and is still a real defect anywhere else the code runs. Fix it; do not
  conclude from a clean httpd run that it was not real.

---

## Types

### Signedness alone fixes nothing

`axis2_ssize_t` is signed so that `-1` can mean failure. That is necessary and
not sufficient: compare a signed `-1` against a `size_t` or a `sizeof` and it
converts to a huge positive value and sails through a "is this long enough"
test. `url.c` did exactly that. `-Wsign-compare` is on to catch the next one.

### `axis2_char_t` is plain `char`

Plain `char` is signed on x86 and unsigned on ARM, PowerPC and s390. The code
assumes the signed reading — there are `ctype` calls that pass an
`axis2_char_t` straight to `isxdigit()` without the `(unsigned char)` cast those
macros require. All seven live `configure.ac` files pass `-fsigned-char` so the
two agree. Anything that compiles sources against these libraries outside the
build must pass it too, or the same `char` behaves differently either side of a
call. See `HTTP2_ANDROID.md`.

---

## Build system

### Sub-packages do not inherit the root's `CFLAGS`

`util`, `axiom`, `neethi`, `guththila`, `samples` and `tools/md5` are separate
packages configured through `AC_CONFIG_SUBDIRS`. Each has its own
`if test "$GCC" = "yes"` block. **A flag added only to the root `configure.ac`
reaches `src/` and nothing else.** When adding a compiler flag, add it to all
seven together.

`samples` builds with `-Werror`, so a warning flag added there fails the build
rather than informing anyone. Codegen flags are safe; warning flags are not.

### Verify a flag from the generated Makefiles, not the build log

Most compile lines name their source relative to the current directory, so a
per-directory grep of the make log samples a handful of lines and can report a
clean result for a package that has none of the flag. It once reported "9 of 9"
where the truth was 0 of about 120.

```sh
for m in Makefile util/src/Makefile axiom/src/om/Makefile \
         neethi/src/Makefile guththila/src/Makefile; do
    printf "%-28s " "$m"
    grep -m1 '^CFLAGS = ' "$m" | grep -q 'YOUR-FLAG' && echo OK || echo MISSING
done
```

### `VERSION_NO` lives in five `configure.ac` files

They must move together. A partial bump is invisible until a generated
`Makefile` in one sub-package still shows the old `-version-info`.

### Objects do not record the flags they were built with

Reconfiguring in place does not force a rebuild. `make clean` after every
`configure`, especially when switching host or toolchain — two "build defects"
were reported and retracted because of this.

---

## The test suite

### A failing test stops `make check` recursing

The suite once reported 19 tests where 43 existed; roughly half had not run in a
long time, and each fix surfaced the next hidden failure (19 → 30 → 35 → 39 →
43). **Check the test count itself**, not just that the run was green.

### `--with-gtest` is required, and it wants source

Without it, every test `Makefile.am` takes its `else` branch, all suites compile
out, automake prints `All tests passed!` over zero tests and exits 0. Configure
now warns and `check-local` prints a banner, but know the shape. The flag needs
the googletest **source** tree; a distro binary package will not do, and no
`libgtest-dev` is required — the suites link the archive the build compiles.

### The codegen tests bypass the automake harness

They print their own `PASS:` lines without going through `TESTS`, so they appear
in the output but in no count and gate nothing.

### Measuring leaks under ASan

Use `ASAN_OPTIONS=detect_leaks=1:exitcode=0`. With the default exit code a
leaking suite fails, which stops the recursion and silently truncates the run —
a measurement taken that way reached 23 of 44 tests.

Expect most reports to be noise: of 127 in one full-suite run, 124 were
`/usr/bin/sed` and `coreutils/ls`, which ASan instruments because `make check`
shells out to them. Filter to reports containing a frame in the tree before
counting anything.

---

## Dead surface, deliberately kept

### XPath has no callers

`axiom_xpath_compile*` and `axiom_xpath_evaluate*` are a public API that nothing
in the tree uses outside their own module and their two tests — only header
declarations. They also hold the only real leaks in the suite. Before spending
time on them, note that fixing them changes nothing for any current caller.

### `include/axis2_ntlm.h` is a tombstone

It declares nothing and contains an `#error` naming Kerberos, OAuth 2.0 and mTLS
as replacements. That is the point: a downstream `#include <axis2_ntlm.h>` gets
a message saying what happened instead of "file not found". Do not delete it as
dead code.
