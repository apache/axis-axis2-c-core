# Packaging Axis2/C for Linux Distributions

This document outlines the requirements and roadmap for packaging Axis2/C
for official inclusion in Linux distributions like Debian and Ubuntu.

## Current Status

**JIRA**: [AXIS2C-1340](https://issues.apache.org/jira/browse/AXIS2C-1340)
**Target**: 2.0.1 release

Axis2/C currently uses a self-contained installation layout with a custom
prefix (`/usr/local/axis2c` by default). While this works well for standalone
deployments, it does not comply with the Filesystem Hierarchy Standard (FHS)
required by Debian, Ubuntu, and other Linux distributions.

## Filesystem Hierarchy Standard (FHS) Requirements

For official Debian/Ubuntu package inclusion, software must follow the FHS:

| Content Type | FHS Location | Current Axis2/C |
|-------------|--------------|-----------------|
| Binaries | `/usr/bin` | `${prefix}/bin` |
| Libraries | `/usr/lib/<multiarch>` | `${prefix}/lib` |
| Config files | `/etc/axis2c/` | `${prefix}/axis2.xml` |
| Shared data | `/usr/share/axis2c/` | `${prefix}/` |
| Services | `/usr/share/axis2c/services/` | `${prefix}/services/` |
| Modules | `/usr/share/axis2c/modules/` | `${prefix}/modules/` |
| Log files | `/var/log/axis2c/` | `${prefix}/logs/` |
| Runtime data | `/var/lib/axis2c/` | N/A |

Reference: https://refspecs.linuxfoundation.org/FHS_3.0/fhs/index.html

## Implementation Roadmap

### Phase 1: Build System Changes (2.0.1)

1. **Add `--enable-fhs` configure option**
   ```bash
   ./configure --enable-fhs --prefix=/usr
   ```

   When enabled:
   - Config files install to `${sysconfdir}/axis2c/`
   - Data files install to `${datadir}/axis2c/`
   - Use standard `${libdir}` for libraries

2. **Make default paths configurable at build time**

   Update `configure.ac`:
   ```m4
   AC_ARG_ENABLE([fhs],
     [AS_HELP_STRING([--enable-fhs], [Use FHS-compliant paths])],
     [enable_fhs=$enableval],
     [enable_fhs=no])

   if test "x$enable_fhs" = "xyes"; then
     AC_DEFINE([AXIS2_FHS_LAYOUT], [1], [Use FHS directory layout])
     axis2_confdir='${sysconfdir}/axis2c'
     axis2_datadir='${datadir}/axis2c'
   else
     axis2_confdir='${prefix}'
     axis2_datadir='${prefix}'
   fi
   AC_SUBST([axis2_confdir])
   AC_SUBST([axis2_datadir])
   ```

3. **Update platform headers**

   In `util/include/platforms/unix/axutil_unix.h`:
   ```c
   #ifdef AXIS2_FHS_LAYOUT
   #define AXIS2_PLATFORM_LOG_PATH        "/var/log/axis2c/axis2.log"
   #define AXIS2_CONFIGURATION_RESOURCE   "/etc/axis2c/axis2.xml"
   #else
   #define AXIS2_PLATFORM_LOG_PATH        "/usr/local/axis2/log/axutil_log"
   #define AXIS2_CONFIGURATION_RESOURCE   "/usr/local/axis2/axis2.xml"
   #endif
   ```

### Phase 2: Runtime Improvements (2.0.1+)

1. **XDG Base Directory support for user configs**
   - Check `$XDG_CONFIG_HOME/axis2c/` for user-specific config
   - Falls back to `~/.config/axis2c/`

2. **Improved path resolution order**
   ```
   1. Command-line argument (-r repo_path)
   2. AXIS2C_HOME environment variable
   3. XDG_CONFIG_HOME/axis2c/ (user config)
   4. /etc/axis2c/ (system config, FHS mode)
   5. Compiled-in default (legacy mode)
   ```

3. **pkg-config support**

   Create `axis2c.pc.in`:
   ```
   prefix=@prefix@
   exec_prefix=@exec_prefix@
   libdir=@libdir@
   includedir=@includedir@

   Name: Axis2/C
   Description: Apache Axis2/C Web Services Engine
   Version: @VERSION@
   Libs: -L${libdir} -laxis2_engine -laxis2_axiom -laxutil
   Cflags: -I${includedir}/axis2-@VERSION@
   ```

### Phase 3: Documentation and Migration (2.0.1)

1. **Update all documentation** to show both layouts
2. **Create migration guide** for existing deployments
3. **Update sample configurations** for Apache httpd

## Files Requiring Changes

### Build System
- `configure.ac` - Add FHS option and path variables
- `Makefile.am` files - Use new path variables
- Add `axis2c.pc.in` for pkg-config

### Source Code
- `util/include/platforms/unix/axutil_unix.h`
- `util/include/platforms/windows/axutil_windows.h`
- `util/src/log.c` - Path resolution logic
- `src/core/deployment/dep_engine.c` - Config file lookup
- `src/core/deployment/conf_init.c` - Repository path logic

### Documentation
- `docs/userguide/json-httpd-h2-userguide.md`
- `docs/HTTP2_*.md` files
- Sample Apache configurations
- README.md

## Debian Package Structure (Target)

```
axis2c/
├── DEBIAN/
│   ├── control
│   ├── conffiles        # Lists /etc/axis2c/* as config files
│   └── postinst         # Creates /var/log/axis2c, sets permissions
├── etc/
│   └── axis2c/
│       └── axis2.xml
├── usr/
│   ├── bin/
│   │   └── axis2_http_server
│   ├── include/
│   │   └── axis2-2.0/
│   ├── lib/
│   │   └── x86_64-linux-gnu/
│   │       ├── libaxis2_engine.so.2.0.0
│   │       ├── libaxis2_axiom.so.2.0.0
│   │       └── ...
│   └── share/
│       ├── axis2c/
│       │   ├── services/
│       │   └── modules/
│       └── doc/
│           └── axis2c/
└── var/
    └── log/
        └── axis2c/
```

## Testing Checklist

Before releasing FHS support:

- [ ] Standalone HTTP server works with FHS paths
- [ ] Apache module loads correctly
- [ ] Client applications find config files
- [ ] Log files created in correct location
- [ ] Services can be deployed and discovered
- [ ] Modules can be loaded
- [ ] SSL/TLS certificates found correctly
- [ ] Backward compatibility with AXIS2C_HOME
- [ ] Build and install with `--enable-fhs --prefix=/usr`
- [ ] Build and install with legacy layout (no --enable-fhs)

## References

- [Filesystem Hierarchy Standard 3.0](https://refspecs.linuxfoundation.org/FHS_3.0/fhs/index.html)
- [Debian Policy Manual - File System Structure](https://www.debian.org/doc/debian-policy/ch-opersys.html)
- [XDG Base Directory Specification](https://specifications.freedesktop.org/basedir-spec/basedir-spec-latest.html)
- [AXIS2C-1340: FHS Compliance Issue](https://issues.apache.org/jira/browse/AXIS2C-1340)
