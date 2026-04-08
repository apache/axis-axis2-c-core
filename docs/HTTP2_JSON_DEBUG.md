# HTTP/2 JSON Deployment Debugging Guide

Practical debugging guide for Axis2/C HTTP/2 JSON services running under
Apache httpd on Debian/Ubuntu systems. Based on real deployment issues
encountered on Ubuntu 22.04 with the packaged Apache (`apache2` package).

## Quick Reference: Log Locations

| Log | Path | What it tells you |
|-----|------|-------------------|
| Apache error log | `/var/log/apache2/error.log` | Module load failures, SSL errors, child crashes |
| Apache access log | `/var/log/apache2/access.log` | Request routing (empty = requests not reaching vhost) |
| Apache other vhosts | `/var/log/apache2/other_vhosts_access.log` | Requests hitting wrong vhost |
| Axis2/C log | `/var/log/axis2c/axis2.log` | Service loading, transport init, worker creation |

## Quick Reference: Diagnostic Commands

```bash
# Is Apache running and on which ports?
systemctl status apache2
ss -tlnp | grep apache2

# Which vhosts are active?
apache2ctl -S

# Which modules are loaded?
apache2ctl -M | grep -E 'ssl|http2|axis2'

# Test config syntax
apache2ctl configtest

# Recent core dumps from Apache
coredumpctl list apache2 | tail -5
```

---

## Problem 1: Apache Starts but Nothing Listens / Curl Returns 000

**Symptoms:**
- `systemctl status apache2` shows active
- `curl -k https://localhost/` returns empty or 000
- `access.log` is empty

**Diagnosis:** Check if the SSL vhost is actually loaded.

```bash
apache2ctl -S
```

If only the port 80 vhost appears and the port 443 vhost is missing:

```bash
# Is the site config file present?
ls /etc/apache2/sites-available/axis2-services.conf

# Is it enabled (symlinked)?
ls /etc/apache2/sites-enabled/ | grep axis2

# Enable it
a2ensite axis2-services
apache2ctl configtest && systemctl restart apache2
```

**Root cause:** The site config was never copied to `sites-available/` or
`a2ensite` was never run. The deploy script copies from the source tree —
if the file isn't committed to git, it won't exist on the target machine.

---

## Problem 2: TLS Handshake Bytes in Port 80 Log

**Symptom:** `other_vhosts_access.log` shows:
```
penguin.alphatheory.local:80 127.0.0.1 - - [...] "\x16\x03\x01\x02" 400 499
```

**What it means:** `\x16\x03\x01` is a TLS ClientHello hitting the plain
HTTP port 80 vhost. The client is sending HTTPS but there is no port 443
listener, so the connection fails and may fall through to port 80.

**Fix:** Ensure `mod_ssl` is loaded and the 443 vhost is active:
```bash
a2enmod ssl
apache2ctl -S   # should show *:443 vhost
```

---

## Problem 3: OCSP Stapling Errors with Self-Signed Certificates

**Symptom in error.log:**
```
AH02217: ssl_stapling_init_cert: can't retrieve issuer certificate!
AH02604: Unable to configure certificate for stapling
```

**What it means:** `SSLUseStapling on` requires a CA-signed certificate
with an OCSP responder URL. Self-signed certificates have no issuer to
query for stapling.

**Fix:** Disable OCSP stapling in the vhost config:
```apache
# SSLUseStapling on
```

Re-enable when switching to a CA-signed certificate (e.g., Let's Encrypt).

---

## Problem 4: "Transport sender is NULL" — Worker Creation Fails

**Symptom in axis2.log:**
```
Loading shared library libaxis2_http_sender.so Failed. No such file or directory
Transport sender is NULL for transport http, unable to continue
```

**Symptom in error.log:**
```
[Axis2] Error creating mod_axis2 apache2 worker
Child returned a Fatal error... Apache is exiting!
```

**What it means:** `axis2.xml` references `axis2_http_sender` and
`axis2_http_receiver`, which map to shared libraries. In HTTP/2 mode these
libraries are intentionally **not built** — Apache httpd handles all
transport via `mod_axis2` (see `docs/HTTP2_CONDITIONAL_COMPILATION.md`).

**Fix:** Comment out the `<transportSender>` and `<transportReceiver>`
blocks in `axis2.xml`:
```xml
<!--transportSender name="http" class="axis2_http_sender">
    ...
</transportSender-->

<!--transportReceiver name="http" class="axis2_http_receiver">
    ...
</transportReceiver-->
```

The source-controlled copy is `samples/server/axis2.xml`. After editing,
run `make install` to propagate to `$(prefix)/share/axis2c/axis2.xml`.

See also: `docs/HTTP2_AXIS2_DOT_XML.md`

---

## Problem 5: Child Process Segfault (SIGSEGV) Loop

**Symptom in error.log:**
```
child pid NNNNN exit signal Segmentation fault (11), possible coredump in /etc/apache2
```

The parent process stays alive, keeps spawning children that immediately
crash — Apache appears "running" but serves no requests.

### Getting a Backtrace

On systemd systems, core dumps are captured by `systemd-coredump`:

```bash
# List recent Apache crashes
coredumpctl list apache2 | tail -10

# Show crash info including stack trace
coredumpctl info apache2 | head -80

# Full GDB backtrace (if debug symbols available)
coredumpctl debug apache2 --debugger=gdb \
    --debugger-arguments="-batch -ex 'bt full' -ex 'quit'"
```

If `coredumpctl` is not available, enable manual core dumps:
```bash
# Set core dump pattern and size limit
echo '/tmp/core.%e.%p' > /proc/sys/kernel/core_pattern
ulimit -c unlimited

# Note: systemctl-started processes inherit systemd's limits, not the shell's.
# For systemd services, edit the unit override:
systemctl edit apache2
# Add:
#   [Service]
#   LimitCORE=infinity

systemctl restart apache2
sleep 3
ls /tmp/core.apache2.*

# Analyze with GDB
gdb /usr/sbin/apache2 /tmp/core.apache2.* -batch -ex 'bt full' -ex 'quit'
```

### Common Segfault: axis2_get_instance Signature Mismatch

**Stack trace pattern:**
```
#0  axis2_svc_create (libaxis2_engine.so)
#1  axis2_<servicename>_create (lib<servicename>.so)
#2  axutil_class_loader_create_dll (libaxutil.so)
#3  axis2_msg_recv_load_and_init_svc_impl (libaxis2_engine.so)
#4  axis2_apache2_worker_create (libmod_axis2.so)
```

**Root cause:** The service's `axis2_get_instance` function has the wrong
signature. The class loader (`axutil_class_loader_create_dll`) calls it as:

```c
// Expected signature (CREATE_FUNCT from axutil_dll_desc.h):
int axis2_get_instance(void **inst, const axutil_env_t *env);

// Called as:
create_funct(&obj, env);
```

If the service instead exports:
```c
// WRONG — causes segfault:
axis2_svc_t* axis2_get_instance(const axutil_env_t *env);
```

Then `&obj` (a `void**`) is misinterpreted as `env`, and the real `env`
goes to an unused parameter slot. Any dereference of `env` inside the
service (e.g., `env->allocator` in `axis2_svc_create`) segfaults.

**Fix:** Match the `CREATE_FUNCT` / `DELETE_FUNCT` typedefs exactly:
```c
AXIS2_EXTERN int AXIS2_CALL
axis2_get_instance(void **inst, const axutil_env_t *env)
{
    *inst = axis2_my_service_create(env);
    if (!(*inst)) return AXIS2_FAILURE;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN int AXIS2_CALL
axis2_remove_instance(void *inst, const axutil_env_t *env)
{
    axis2_svc_t *svc = (axis2_svc_t *)inst;
    if (svc) axis2_svc_free(svc, env);
    return AXIS2_SUCCESS;
}
```

**Alternative approach:** Some services (e.g., `bigdata-h2-service`,
`login-service`, `testws-service`) avoid this trap entirely by **not
exporting `axis2_get_instance`**. They rely solely on the `ServiceClass`
parameter in `services.xml` and the `_invoke_json` dlsym lookup path in
the `JsonRpcMessageReceiver`. This sidesteps the `CREATE_FUNCT` signature
requirement altogether. Only export `axis2_get_instance` / `axis2_remove_instance`
if your service needs custom initialization during worker startup.

---

## Problem 6: Apache Module Not Found at Install Time

**Symptom during `make install`:**
```
Neither mod_axis2.so nor libmod_axis2.so found at /usr/local/apache2/modules/
```

**What it means:** The Makefile.am `apachemoddir` is set to a path that
doesn't exist. Source-built Apache uses `/usr/local/apache2/modules/`;
packaged Apache uses system paths.

**Fix:** Set `apachemoddir = @prefix@/lib` in
`src/core/transport/http/server/apache2/Makefile.am`, then `autoreconf -fiv`
and rebuild. The `axis2-services.conf` loads the module from
`/usr/local/axis2c/lib/libmod_axis2.so` which matches `--prefix=/usr/local/axis2c`.

---

## Problem 7: json-c Version Incompatibility

**Symptom during `make`:**
```
error: 'json_tokener_error_memory' undeclared
```

**What it means:** The code uses `json_tokener_error_memory` which was
added in json-c **0.17**, but the version guard checks for `>= 0.14`.
Ubuntu 22.04 ships json-c 0.15 which passes the guard but lacks the enum.

**Fix:** In `src/core/transport/http/util/axis2_json_reader.c`, change the
version guard for `json_tokener_error_memory` and `json_tokener_error_size`
from `(14 << 8)` to `(17 << 8)`:

```c
#if defined(JSON_C_VERSION_NUM) && JSON_C_VERSION_NUM >= ((0 << 16) | (17 << 8))
    case json_tokener_error_memory:
        return "error_memory";
    case json_tokener_error_size:
        return "error_size";
#endif
```

---

## Problem 8: Post-Restart Health Check Passes but Service Crashes

**Symptom:** The deploy script reports "Apache is running" but verification
curls all return 000.

**What it means:** Apache's parent process starts successfully (passes
`systemctl is-active` check), but child processes crash during worker
initialization. With `mpm_event`, the parent keeps trying to spawn children,
creating a crash loop visible in `error.log`.

**Diagnosis:**
```bash
# The parent is alive but children are dying
tail -20 /var/log/apache2/error.log   # look for "Child returned a Fatal error"
tail -20 /var/log/axis2c/axis2.log    # look for loading errors before the crash
```

**Common causes (in order of likelihood):**
1. Missing transport libs in `axis2.xml` (Problem 4)
2. Service DLL signature mismatch (Problem 5)
3. Missing service shared libraries
4. Corrupt `axis2.xml` (syntax errors)

---

## Problem 9: Request-Path Segfault — NULL Transport Descriptor

**Symptom:** Worker init succeeds (axis2.log shows "Axis2 worker created")
but Apache child crashes on the first incoming request.

**Stack trace pattern (from `coredumpctl info`):**
```
#0  axis2_transport_in_desc_get_recv (libaxis2_engine.so)
#1  axis2_apache2_worker_process_request (libmod_axis2.so)
#2  axis2_handler (libmod_axis2.so)
#3  ap_run_handler (apache2)
```

**Root cause:** In `apache2_worker.c`, the request processing path calls:
```c
in_desc = axis2_conf_get_transport_in(conf, env, AXIS2_TRANSPORT_ENUM_HTTP);
// in_desc is NULL because transportReceiver is commented out in axis2.xml
receiver = axis2_transport_in_desc_get_recv(in_desc, env);  // SEGFAULT
```

When `transportReceiver` is absent from `axis2.xml` (correct for HTTP/2
mode), `axis2_conf_get_transport_in()` returns NULL. The code then passes
NULL to `axis2_transport_in_desc_get_recv()` which dereferences it.

**Fix:** NULL-guard the transport descriptor before use:
```c
if(in_desc)
{
    receiver = axis2_transport_in_desc_get_recv(in_desc, env);
    if(receiver)
        axis2_transport_receiver_set_server_ip(receiver, env, ...);
}
```

**Key insight:** There are **two separate crash points** when removing
transport config from axis2.xml — one during init (Problem 4: "Transport
sender is NULL") and one during request handling (this problem). Fixing
Problem 4 alone is not sufficient; the request path must also tolerate
NULL transport descriptors.

---

## Problem 10: "Operation Not Found" for Valid Service

**Symptom in axis2.log:**
```
Operation Not found. Endpoint reference is : /services/MyService/someOperation
```

**Symptom in response:**
```json
{"error":{"message":"Service processing failed","code":500}}
```

**What it means:** The service is loaded and running, but the operation
name in the URL path doesn't match any `<operation name="...">` in
`services.xml`.

**Diagnosis:** Check what operations are actually registered:
```bash
grep '<operation name=' /usr/local/axis2c/services/MyService/services.xml
```

**Common causes:**
- URL uses a different operation name than what's in `services.xml`
- The test/deploy script uses a placeholder operation name (e.g., `benchmark`)
  that was never defined
- Case sensitivity mismatch

---

## Packaged Apache vs Source-Built Apache

Many issues stem from path differences between these two environments:

| Component | Packaged (Debian/Ubuntu) | Source-built |
|-----------|-------------------------|--------------|
| Binary | `/usr/sbin/apache2` | `/usr/local/apache2/bin/httpd` |
| Modules dir | `/usr/lib/apache2/modules/` | `/usr/local/apache2/modules/` |
| Config dir | `/etc/apache2/` | `/usr/local/apache2/conf/` |
| Site management | `a2ensite` / `a2dissite` | Manual `Include` directives |
| Module management | `a2enmod` / `a2dismod` | `LoadModule` in httpd.conf |
| mod_http2 name | `mod_http2.so` (`a2enmod http2`) | `mod_h2.so` |
| Service management | `systemctl restart apache2` | `httpd -k restart` |

The `deploy-localhost.sh` script and `axis2-services.conf` are designed for
the **packaged** Apache. The individual service build scripts
(`build_json_service.sh`, etc.) detect source-built Apache at
`/usr/local/apache2` and configure it automatically; otherwise they skip
module installation and defer to `deploy-localhost.sh`.

---

## Related Documentation

- `docs/HTTP2_AXIS2_DOT_XML.md` — Why axis2.xml doesn't need HTTP/2 transport config
- `docs/HTTP2_CONDITIONAL_COMPILATION.md` — What's built/excluded in HTTP/2 mode
- `docs/HTTP2_LIBRARY_LOADING.md` — How Axis2/C loads service shared libraries
- `docs/HTTP2_APACHE2_WORKER.md` — Apache worker initialization details
- `docs/userguide/json-httpd-h2-userguide.md` — Full deployment guide
