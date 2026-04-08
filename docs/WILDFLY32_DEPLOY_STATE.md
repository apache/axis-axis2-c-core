# WildFly 32 Deployment — Current State (2026-04-08)

Resume point for next Claude session. Goal: deploy Axis2/Java
FinancialBenchmarkService on WildFly 32 and run performance comparison
against Axis2/C.

## What's Done

### Axis2/C (COMPLETE)
- All services deployed on penguin over HTTP/2
- Performance benchmarks captured in `docs/MCP_EXAMPLES.md`
- MCP stdio verified (initialize, tools/list, tools/call all work)
- Key numbers: 500-asset portfolioVariance 269μs, 100K Monte Carlo 1.08s

### Axis2/Java WAR (BUILT)
- WAR at: `~/repos/axis-axis2-java-core/modules/samples/userguide/src/userguide/springbootdemo-wildfly/target/axis2-json-api-0.0.1-SNAPSHOT.war`
- Contains: FinancialBenchmarkService.aar, BigDataH2Service.aar, Login.aar, testws.aar
- All three financial operations are real (not stubs): portfolioVariance, monteCarlo, scenarioAnalysis

### WildFly 32 (CONFIGURED)
- Fresh AT-standard WildFly 32.0.1.Final at `~/wildfly/`
- `standalone.conf` updated with `--add-modules=java.se` and `--add-opens` (lines 71-75)
- `jdk.net` JBoss Module created at `~/wildfly/modules/system/layers/base/jdk/net/main/module.xml`
- `jboss-deployment-structure.xml` now uses dptv2's exact pattern (no CDI exclusion)
- `beans.xml` with `bean-discovery-mode="none"` already in WAR

## What Failed and Why

### Shell Environment Problem
Claude's shell snapshot keeps restoring `JAVA_OPTS` with `-XX:+PrintFlagsFinal`
which dumps thousands of lines to stdout and breaks WildFly's output redirect.
Even `unset JAVA_OPTS` doesn't persist across Claude's command boundaries.

**Fix for next session:** Ensure `~/.bashrc` does NOT set `JAVA_OPTS` with
`PrintFlagsFinal`. Then start a fresh Claude session so the shell snapshot
is clean.

### CDI / Weld Resolution (SOLVED)
- Weld is enabled in AT's WildFly (don't disable it)
- Do NOT exclude `cdi` subsystem in jboss-deployment-structure.xml
- Use `beans.xml` with `bean-discovery-mode="none"` instead
- This satisfies WildFly's batch subsystem (jberet) CDI dependency

### jdk.net.Sockets (SOLVED)
- Staging uses `--add-modules=java.se` in wrapper.conf (line 30)
- This makes the `jdk.net` module available to the JVM
- Also need `<module name="jdk.net" />` in jboss-deployment-structure.xml
  (same as dptv2)
- Created JBoss Module definition at `~/wildfly/modules/system/layers/base/jdk/net/main/`

## To Resume: Start WildFly

```bash
# 1. Ensure clean JAVA_OPTS (no PrintFlagsFinal)
unset JAVA_OPTS
echo $JAVA_OPTS  # should be empty

# 2. Kill any stale WildFly
pkill -9 -f jboss-modules; sleep 2

# 3. Clean deployment state
rm -f ~/wildfly/standalone/deployments/axis2-json-api.war.failed
rm -rf ~/wildfly/standalone/tmp/* ~/wildfly/standalone/data/*

# 4. Deploy WAR
cp ~/repos/axis-axis2-java-core/modules/samples/userguide/src/userguide/springbootdemo-wildfly/target/axis2-json-api-0.0.1-SNAPSHOT.war \
   ~/wildfly/standalone/deployments/axis2-json-api.war
touch ~/wildfly/standalone/deployments/axis2-json-api.war.dodeploy

# 5. Start WildFly (standalone.conf adds --add-modules=java.se)
~/wildfly/bin/standalone.sh -b 0.0.0.0 > /tmp/wildfly.log 2>&1 &

# 6. Wait and check
sleep 30
grep -E "WFLYSRV0025|deployed" /tmp/wildfly.log
ls ~/wildfly/standalone/deployments/axis2-json-api.war.*
```

## To Test: curl Commands (match Axis2/C examples)

Login first (get JWT token):
```bash
TOKEN=$(curl -s http://localhost:8080/axis2-json-api/services/loginService \
  -H 'Content-Type: application/json' \
  -d '{"doLogin":[{"arg0":{"email":"java-dev@axis.apache.org","credentials":"userguide"}}]}' \
  | python3 -c "import sys,json; print(json.load(sys.stdin)['doLoginResponse']['return']['token'])")
```

portfolioVariance (same 3-asset test as Axis2/C):
```bash
curl -s http://localhost:8080/axis2-json-api/services/FinancialBenchmarkService \
  -H 'Content-Type: application/json' -H "Authorization: Bearer $TOKEN" \
  -d '{"portfolioVariance":[{"arg0":{"nAssets":3,"weights":[0.4,0.3,0.3],"covarianceMatrix":[[0.04,0.006,0.002],[0.006,0.09,0.009],[0.002,0.009,0.01]]}}]}'
```

monteCarlo (10K simulations):
```bash
curl -s http://localhost:8080/axis2-json-api/services/FinancialBenchmarkService \
  -H 'Content-Type: application/json' -H "Authorization: Bearer $TOKEN" \
  -d '{"monteCarlo":[{"arg0":{"nSimulations":10000,"nPeriods":252,"initialValue":1000000,"expectedReturn":0.08,"volatility":0.20,"nPeriodsPerYear":252}}]}'
```

monteCarlo (100K simulations):
```bash
curl -s http://localhost:8080/axis2-json-api/services/FinancialBenchmarkService \
  -H 'Content-Type: application/json' -H "Authorization: Bearer $TOKEN" \
  -d '{"monteCarlo":[{"arg0":{"nSimulations":100000,"nPeriods":252,"initialValue":1000000,"expectedReturn":0.08,"volatility":0.20,"nPeriodsPerYear":252}}]}'
```

500-asset portfolioVariance:
```bash
# Generate test data first
python3 -c "
import json, math
n=500
w=[1.0/n]*n
c=[]
for i in range(n):
    row=[]
    for j in range(n):
        if i==j: row.append(0.04)
        else: row.append(0.01*max(0,1.0-abs(i-j)/50.0))
    c.append(row)
print(json.dumps({'portfolioVariance':[{'arg0':{'nAssets':n,'weights':w,'covarianceMatrix':c}}]}))
" > /tmp/java_pv500.json

curl -s http://localhost:8080/axis2-json-api/services/FinancialBenchmarkService \
  -H 'Content-Type: application/json' -H "Authorization: Bearer $TOKEN" \
  -d @/tmp/java_pv500.json
```

## Performance Comparison (MEASURED 2026-04-08)

| Benchmark | Axis2/C | Axis2/Java | Ratio |
|-----------|---------|------------|-------|
| portfolioVariance (3 assets) | < 1 μs | < 1 μs | ~1x |
| portfolioVariance (500 assets) | 269 μs | 4,849 μs | ~18x slower |
| monteCarlo (10K × 252) | 110 ms | 141 ms | ~1.3x slower |
| monteCarlo (100K × 252) | 1.08 sec | 1.45 sec | ~1.3x slower |
| Peak memory | 48 MB | ~300 MB | ~6x more |
| Startup | instant | ~7 sec (WildFly) | N/A |

### Key findings
- Monte Carlo: Java competitive (~1.3x) — JIT optimizes the tight simulation loop
- Matrix ops (500-asset): C is 18x faster — pure O(n²) array math is C's home turf
- Memory: Java uses ~6x more (JVM + WildFly + Spring overhead)
- Deployment fix: deleted custom `jdk/net/main/module.xml` — JBoss resolves
  `jdk.net` directly as a JPMS module via `--add-modules=java.se`

## Files Modified in This Session

### axis-axis2-c-core (committed)
- `docs/MCP_EXAMPLES.md` — new doc with performance numbers and live session
- `docs/HTTP2_JSON_DEBUG.md` — problems 9-10 added
- `docs/HTTP2_ANDROID.md` — 2-param vs 4-param invoke signature
- `docs/HTTP2_SERVICES_DOT_XML.md` — step 6 dispatch mechanism
- `docs/HTTP2_APACHE2_WORKER.md` — NULL transport descriptor note
- `src/core/receivers/axis2_json_rpc_msg_recv.c` — _invoke_json dispatch
- `src/core/transport/http/server/apache2/apache2_worker.c` — NULL guard
- `src/core/transport/http/server/apache2/axis2_apache2_request_processor_json_impl.c` — GET support
- `samples/user_guide/financial-benchmark-service/src/financial_benchmark_service_handler.c` — axis2_get_instance fix
- Multiple build script and deploy script fixes

### axis-axis2-java-core (uncommitted)
- `modules/samples/userguide/src/userguide/springbootdemo-wildfly/src/main/webapp/WEB-INF/jboss-deployment-structure.xml`
  — uses dptv2 pattern (no CDI exclusion, added jdk.net)

### Local WildFly (~/wildfly/)
- `bin/standalone.conf` — added --add-modules=java.se and --add-opens
- `modules/system/layers/base/jdk/net/main/module.xml` — created jdk.net module
- `standalone/configuration/standalone.xml` — AT standard config

## Key Docs for Context
- Staging wrapper.conf: `~/repos/staging/stg.aws.puppet/at_stg_jobs/files/apps/build/s-rapi2/wrapper.conf`
- dptv2 deployment structure: `~/repos/dptv2/src/main/webapp/WEB-INF/jboss-deployment-structure.xml`
- AT PENGUIN reference: `~/repos/dptv2/docs/PENGUIN.md`
