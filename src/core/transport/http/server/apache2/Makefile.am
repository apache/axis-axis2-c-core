# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
# 
#   http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# Install to Apache modules directory instead of standard lib directory
apachemoddir = /usr/local/apache2/modules
apachemod_LTLIBRARIES = libmod_axis2.la

# Automatic build fix: Force recompilation when object files are missing
# This prevents the "cannot find .libs/*.o" linker errors after .libs cleanup
.PHONY: check-build-state
check-build-state:
	@if [ ! -f .libs/libmod_axis2_la-mod_axis2.o ]; then \
		echo "🔧 BUILD FIX: Missing object files detected, forcing source recompilation..."; \
		touch $(libmod_axis2_la_SOURCES); \
		echo "✅ BUILD FIX: Source files touched, make will now recompile automatically"; \
	fi

# Ensure build state check runs before any compilation
all-local: check-build-state

libmod_axis2_la_SOURCES = mod_axis2.c\
                       apache2_stream.c\
                       apache2_out_transport_info.c\
                       apache2_worker.c\
                       axis2_apache2_request_processor_factory.c\
                       axis2_apache2_request_processor_json_impl.c\
                       axis2_apache2_request_processor_soap_impl_simple.c

libmod_axis2_la_CFLAGS = -DLINUX=2 -D_REENTRANT -D_XOPEN_SOURCE=500 -D_BSD_SOURCE -D_SVID_SOURCE -D_GNU_SOURCE -DWITH_NGHTTP2

libmod_axis2_la_CPPFLAGS = $(APRINC) \
                          -I$(top_srcdir)/include \
                          -I$(top_srcdir)/util/include \
                          -I$(top_srcdir)/axiom/include \
                          -I$(top_srcdir)/src/core/transport/http \
                          -I$(top_srcdir)/src/core/description \
                          -I$(top_srcdir)/src/core/context \
                          -I$(top_srcdir)/src/core/phaseresolver \
                          -I$(top_srcdir)/src/core/engine \
                          -I$(top_srcdir)/src/core/deployment \
                          -I/usr/include/apache2 \
                          @APACHE2INC@ \
                          @APRINC@

# NOTE: AXIOM includes restored for backward compatibility
# The interface pattern supports both SOAP and JSON processing

# Essential libraries for Apache module - both SOAP and JSON support
# Use .la files for proper libtool dependency resolution
libmod_axis2_la_LIBADD   = $(top_builddir)/util/src/libaxutil.la \
		     $(top_builddir)/src/core/transport/http/common/libaxis2_http_common.la \
		     $(top_builddir)/src/core/engine/libaxis2_engine.la \
		     $(top_builddir)/src/core/transport/http/util/libaxis2_http_util.la \
		     $(top_builddir)/axiom/src/om/libaxis2_axiom.la

# Add JSON support conditionally when AXIS2_JSON_ENABLED is set
if AXIS2_JSON_ENABLED
libmod_axis2_la_LIBADD += $(JSON_LIBS)
libmod_axis2_la_CPPFLAGS += $(JSON_CFLAGS)
endif

# Core dependencies:
# - libaxutil.la (ESSENTIAL - utilities used throughout)
# - libaxis2_http_common.la (ESSENTIAL - http_out_transport_info functions)
# - libaxis2_engine.la (NEEDED - apache2_worker.c calls axis2_conf_get_all_svcs)
# - libaxis2_http_util.la (NEEDED - http transport utility functions)
# - libaxis2_axiom.la (NEEDED - SOAP processing and AXIOM functions)

libmod_axis2_la_LDFLAGS = -module -avoid-version -no-undefined

EXTRA_DIST=axis2_apache2_worker.h apache2_stream.h axis2_apache2_out_transport_info.h axis2_apache2_request_processor.h

# Ensure Apache module (.so file) is properly installed with STRICT validation
install-exec-hook:
	@echo "=== APACHE MODULE INSTALLATION WITH STALENESS VALIDATION ==="
	@echo "Module will be installed to: $(DESTDIR)$(apachemoddir)"
	@echo "Final system location: $(apachemoddir)"
	@echo "DESTDIR='$(DESTDIR)' apachemoddir='$(apachemoddir)'"
	@echo "=== STEP 0: DEBUG - Check local build files ==="
	@echo "Local .libs directory contents:"
	@ls -la $(srcdir)/.libs/ || echo "⚠️  No .libs directory found"
	@echo "=== STEP 1: Ensure mod_axis2.so exists with correct name ==="
	if test -f "$(DESTDIR)$(apachemoddir)/mod_axis2.so"; then \
		echo "[INSTALL_OK] Module already correctly named as mod_axis2.so"; \
	elif test -f "$(DESTDIR)$(apachemoddir)/libmod_axis2.so"; then \
		echo "[INSTALL] Renaming libmod_axis2.so to mod_axis2.so"; \
		mv "$(DESTDIR)$(apachemoddir)/libmod_axis2.so" "$(DESTDIR)$(apachemoddir)/mod_axis2.so" || { \
			echo "❌ FATAL: Failed to rename libmod_axis2.so to mod_axis2.so"; \
			exit 1; \
		}; \
	else \
		echo "❌ FATAL: Neither mod_axis2.so nor libmod_axis2.so found at $(DESTDIR)$(apachemoddir)/"; \
		echo "🔧 LIBTOOL INSTALLATION FAILED - The shared library should have been copied by libtool"; \
		echo "💡 Checking if local build succeeded:"; \
		if test -f "$(srcdir)/.libs/libmod_axis2.so"; then \
			echo "✅ Local build succeeded: $(srcdir)/.libs/libmod_axis2.so exists"; \
			echo "❌ Issue: libtool --mode=install failed to copy shared library"; \
			echo "🚀 AUTOMATIC FIX: Copying shared library manually"; \
			cp "$(srcdir)/.libs/libmod_axis2.so" "$(DESTDIR)$(apachemoddir)/mod_axis2.so" || { \
				echo "❌ FATAL: Manual copy failed"; \
				exit 1; \
			}; \
			echo "✅ FIXED: Shared library copied successfully"; \
		else \
			echo "❌ Build failed: No shared library in .libs directory"; \
			echo "Available files:"; \
			ls -la "$(DESTDIR)$(apachemoddir)/" || true; \
			exit 1; \
		fi; \
	fi
	@echo "=== STEP 2: Force copy to system location if DESTDIR was set ==="
	if test -n "$(DESTDIR)" -a "$(DESTDIR)" != "/" -a -f "$(DESTDIR)$(apachemoddir)/mod_axis2.so"; then \
		echo "[INSTALL] DESTDIR detected - copying from staging to system location"; \
		mkdir -p "$(apachemoddir)" || { \
			echo "❌ FATAL: Cannot create $(apachemoddir)"; \
			exit 1; \
		}; \
		cp "$(DESTDIR)$(apachemoddir)/mod_axis2.so" "$(apachemoddir)/mod_axis2.so" || { \
			echo "❌ FATAL: Failed to copy to system location"; \
			exit 1; \
		}; \
		echo "[INSTALL_SUCCESS] Copied to system: $(apachemoddir)/mod_axis2.so"; \
	fi
	@echo "=== STEP 3: INSTALLATION VALIDATION - Check built artifacts vs installed module ==="
	FINAL_MODULE="$(apachemoddir)/mod_axis2.so"; \
	if test ! -f "$${FINAL_MODULE}"; then \
		FINAL_MODULE="$(DESTDIR)$(apachemoddir)/mod_axis2.so"; \
	fi; \
	echo "[VALIDATE] Checking final module: $${FINAL_MODULE}"; \
	if test ! -f "$${FINAL_MODULE}"; then \
		echo "❌ FATAL: Final module not found at any location"; \
		echo "Checked: $(apachemoddir)/mod_axis2.so"; \
		echo "Checked: $(DESTDIR)$(apachemoddir)/mod_axis2.so"; \
		exit 1; \
	fi; \
	echo "[VALIDATE] Module timestamp: $$(stat -c '%y' "$${FINAL_MODULE}")"; \
	\
	BUILD_ARTIFACT="$(srcdir)/.libs/libmod_axis2.so"; \
	if test -f "$${BUILD_ARTIFACT}"; then \
		echo "[VALIDATE] Built artifact: $$(stat -c '%y' "$${BUILD_ARTIFACT}")"; \
		if test "$${FINAL_MODULE}" -ot "$${BUILD_ARTIFACT}"; then \
			echo "❌ STALENESS: Installed module older than built artifact"; \
			echo "This indicates the installation process failed to copy the latest build."; \
			echo ""; \
			echo "🔧 AUTOMATIC FIX: Copying latest build to installation location"; \
			cp "$${BUILD_ARTIFACT}" "$${FINAL_MODULE}" || { \
				echo "❌ FATAL: Failed to copy latest build"; \
				exit 1; \
			}; \
			echo "✅ FIXED: Latest build copied to $${FINAL_MODULE}"; \
		else \
			echo "[VALIDATE_OK] Installed module is up to date with built artifact"; \
		fi; \
	else \
		echo "[VALIDATE] No built artifact found - assuming clean install"; \
	fi

	@echo "=== INSTALLATION AND VALIDATION COMPLETED SUCCESSFULLY ==="

# Ensure proper cleanup of libtool artifacts
CLEANFILES = *.o *.lo *.la
DISTCLEANFILES = *.o *.lo *.la
MAINTAINERCLEANFILES = Makefile.in

# Force removal of libtool .libs directory and all contents
clean-local:
	rm -rf .libs

distclean-local: clean-local
	rm -f *.o *.lo *.la

# Verification target to check source vs binary consistency
verify-build:
	@echo "🔍 Verifying build consistency..."
	@if test -f ".libs/libmod_axis2.so"; then \
		echo "📅 Built binary: $$(stat -c '%y' .libs/libmod_axis2.so)"; \
		echo "📅 Source file: $$(stat -c '%y' mod_axis2.c)"; \
		if test ".libs/libmod_axis2.so" -ot "mod_axis2.c"; then \
			echo "❌ ERROR: Binary older than source - rebuild needed!"; \
			echo "💡 Run 'make clean && make' to fix"; \
			exit 1; \
		else \
			echo "✅ Build timestamp verification passed"; \
		fi; \
		if strings ".libs/libmod_axis2.so" | grep -q "DEBUG.*post_config"; then \
			echo "✅ Debug strings found in built binary"; \
		else \
			echo "ℹ️  Debug strings not found in built binary"; \
		fi; \
	else \
		echo "❌ Built binary not found - run 'make' first"; \
		exit 1; \
	fi
	@echo "🎯 Build verification completed"

# Verification target for installed module
verify-install:
	@echo "🔍 Verifying installed module..."
	@if test -f "$(apachemoddir)/mod_axis2.so"; then \
		echo "📅 Installed: $$(stat -c '%y' $(apachemoddir)/mod_axis2.so)"; \
		echo "📅 Source: $$(stat -c '%y' mod_axis2.c)"; \
		if test "$(apachemoddir)/mod_axis2.so" -ot "mod_axis2.c"; then \
			echo "❌ ERROR: Installed binary older than source!"; \
			echo "💡 Run 'sudo make install' to fix"; \
			exit 1; \
		else \
			echo "✅ Installation timestamp verification passed"; \
		fi; \
		if strings "$(apachemoddir)/mod_axis2.so" | grep -q "DEBUG.*post_config"; then \
			echo "✅ Debug strings found in installed module"; \
		else \
			echo "ℹ️  Debug strings not found in installed module"; \
		fi; \
	else \
		echo "❌ Installed module not found at $(apachemoddir)/mod_axis2.so"; \
		echo "💡 Run 'sudo make install' to fix"; \
		exit 1; \
	fi
	@echo "🎯 Installation verification completed"

# Apache module will be installed automatically to /usr/local/apache2/modules/ by libtool








