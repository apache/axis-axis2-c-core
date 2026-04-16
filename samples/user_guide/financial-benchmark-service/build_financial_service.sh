#!/bin/bash
set -e

echo "🚀 Building Financial Benchmark JSON Service - SOAP-FREE HTTP/2 Architecture"
echo ""
echo "📊 PURPOSE: Demonstrate Axis2/C performance on resource-constrained hardware"
echo "           where Java/WildFly cannot run (old Android phones, embedded systems)"
echo ""

# Create services directory
echo "📁 Creating services directory..."
sudo mkdir -p /usr/local/axis2c/services/FinancialBenchmarkService

echo "🔨 Building with PURE JSON dependencies (AXIOM-FREE + SOAP-FREE)..."
echo "❌ EXCLUDED: -laxis2_axiom (XML object model not needed for pure JSON)"
echo "❌ EXCLUDED: SOAP service skeleton framework patterns"
echo "✅ INCLUDING: -laxutil -laxis2_engine -ljson-c -lm (math library)"
echo ""
echo "📈 ALGORITHMS INCLUDED:"
echo "   • Portfolio Variance - O(n²) covariance matrix calculation"
echo "   • Monte Carlo VaR   - Compute-intensive simulation"
echo "   • Scenario Analysis - Hash table lookups"
echo ""
echo "🤖 MCP (Model Context Protocol) binary also built:"
echo "   • financial-benchmark-mcp — JSON-RPC 2.0 stdio, runs as an MCP client subprocess"
echo ""

# ── 1. Build the httpd shared library ─────────────────────────────────────────
#
# Includes:
#   financial_benchmark_service.c     — core computation + JSON I/O
#   financial_benchmark_service_handler.c — Axis2/C HTTP entry points (D1)
#   mcp_catalog_handler.c             — /_mcp/openapi-mcp.json endpoint (D1)
#   axis2_json_secure_fault.c         — correlation-ID error responses (D2)
gcc -shared -fPIC \
    -DAXIS2_JSON_ENABLED -DWITH_NGHTTP2 -O2 \
    -I/usr/local/axis2c/include/axis2-2.0.0 \
    -I../../include \
    -I../../util/include \
    -I../../src/core/transport/http/util \
    -I/usr/include/json-c \
    src/financial_benchmark_service.c \
    src/financial_benchmark_service_handler.c \
    src/mcp_catalog_handler.c \
    src/axis2_json_secure_fault.c \
    -o libfinancial_benchmark_service.so \
    -L/usr/local/axis2c/lib \
    -laxutil -laxis2_engine \
    -ljson-c \
    -lm

# ── 2. Build the MCP stdio binary (C1) ────────────────────────────────────────
gcc -O2 \
    -DAXIS2_JSON_ENABLED -DWITH_NGHTTP2 \
    -I/usr/local/axis2c/include/axis2-2.0.0 \
    -I../../include \
    -I../../util/include \
    -I../../src/core/transport/http/util \
    -I/usr/include/json-c \
    src/financial_benchmark_service.c \
    src/finbench_mcp.c \
    src/finbench_mcp_main.c \
    -o financial-benchmark-mcp \
    -L/usr/local/axis2c/lib \
    -Wl,-rpath,/usr/local/axis2c/lib \
    -laxutil -laxis2_engine \
    -ljson-c \
    -lm

echo "Build exit code: $?"

if [ -f "libfinancial_benchmark_service.so" ] && [ -f "financial-benchmark-mcp" ]; then
    echo ""
    echo "🎉 SUCCESS: Financial Benchmark Service built (httpd .so + MCP binary)"

    # Show file info
    ls -la libfinancial_benchmark_service.so financial-benchmark-mcp
    file libfinancial_benchmark_service.so financial-benchmark-mcp

    # Check dependencies
    echo ""
    echo "🔍 Checking runtime dependencies..."
    ldd libfinancial_benchmark_service.so | grep -E '(axis2|json|libax|libm)' || true
    ldd financial-benchmark-mcp | grep -E '(axis2|json|libax|libm)' || true

    # ── Install httpd service ──
    echo ""
    echo "🚀 Installing httpd JSON service (mod_axis2)..."
    sudo cp libfinancial_benchmark_service.so /usr/local/axis2c/services/FinancialBenchmarkService/
    sudo cp services.xml /usr/local/axis2c/services/FinancialBenchmarkService/
    sudo chown -R www-data:www-data /usr/local/axis2c/services/FinancialBenchmarkService
    sudo chmod 755 /usr/local/axis2c/services/FinancialBenchmarkService
    sudo chmod 644 /usr/local/axis2c/services/FinancialBenchmarkService/services.xml
    sudo chmod 755 /usr/local/axis2c/services/FinancialBenchmarkService/libfinancial_benchmark_service.so

    # ── Install MCP binary ──
    echo "🤖 Installing MCP stdio binary..."
    sudo mkdir -p /usr/local/axis2c/bin
    sudo cp financial-benchmark-mcp /usr/local/axis2c/bin/
    sudo chmod 755 /usr/local/axis2c/bin/financial-benchmark-mcp

    echo ""
    echo "✅ Installation complete!"
    echo ""
    echo "============================================================"
    echo "🧪 TEST THE SERVICE:"
    echo "============================================================"
    echo ""
    echo "── HTTP/2 JSON (via Apache httpd) ──"
    echo ""
    echo "1. Start Apache httpd with mod_axis2:"
    echo "   sudo systemctl start apache2  # or httpd"
    echo ""
    echo "2. Get service metadata:"
    echo '   curl -k --http2 https://localhost:443/services/FinancialBenchmarkService/metadata'
    echo ""
    echo "3. Test Portfolio Variance (100 assets - quick test):"
    echo '   python3 test/generate_test_data.py --assets 100 | \'
    echo '   curl -k --http2 -H "Content-Type: application/json" -d @- \'
    echo '        https://localhost:443/services/FinancialBenchmarkService/portfolioVariance'
    echo ""
    echo "4. Test Scenario Analysis:"
    echo '   curl -k --http2 -H "Content-Type: application/json" \'
    echo '        -d '"'"'{"assets":[{"asset_id":1,"current_price":100,"position_size":10,"scenario_prices":[110,100,90],"probabilities":[0.4,0.35,0.25]}]}'"'"' \'
    echo '        https://localhost:443/services/FinancialBenchmarkService/scenarioAnalysis'
    echo ""
    echo "5. Test Monte Carlo VaR (10,000 simulations):"
    echo '   curl -k --http2 -H "Content-Type: application/json" \'
    echo '        -d '\''{"n_simulations":10000,"n_periods":252,"initial_value":1000000,"expected_return":0.08,"volatility":0.20}'\'' \'
    echo '        https://localhost:443/services/FinancialBenchmarkService/monteCarlo'
    echo ""
    echo "── MCP stdio (stdio-based MCP client) ──"
    echo ""
    echo "6. Add to your MCP client config (path varies by client;"
    echo "   Claude Desktop's ~/.config/claude/claude_desktop_config.json shown):"
    echo '   {"mcpServers":{"axis2c-financial":{"command":"/usr/local/axis2c/bin/financial-benchmark-mcp"}}}'
    echo ""
    echo "7. Smoke-test MCP stdio directly:"
    echo '   echo '"'"'{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"protocolVersion":"2024-11-05","capabilities":{},"clientInfo":{"name":"test","version":"1.0"}}}'"'"' \'
    echo '        | /usr/local/axis2c/bin/financial-benchmark-mcp'
    echo ""
    echo "   Expected: {\"jsonrpc\":\"2.0\",\"id\":1,\"result\":{\"protocolVersion\":\"2024-11-05\",...}}"
    echo ""
    echo "============================================================"
    echo "📊 EXPECTED BENCHMARK RESULTS (on modest hardware):"
    echo "============================================================"
    echo ""
    echo "┌─────────────────────────────────────────────────────────────┐"
    echo "│  Test                    │ Axis2/C      │ Java/WildFly     │"
    echo "├─────────────────────────────────────────────────────────────┤"
    echo "│  Portfolio 500 assets    │ ~5ms         │ Cannot run (4GB+)│"
    echo "│  Monte Carlo 10K sims    │ ~100ms       │ Cannot run (4GB+)│"
    echo "│  Memory usage            │ ~30MB        │ 4-8GB minimum    │"
    echo "│  Device cost             │ \$20          │ \$2000+           │"
    echo "└─────────────────────────────────────────────────────────────┘"
    echo ""
    echo "🔍 CHECK LOGS:"
    echo "   tail -f /usr/local/axis2c/logs/axis2.log"
    echo "   (Look for FinBench calculation timing logs)"

else
    echo ""
    echo "❌ Build failed - check compiler errors above"
    echo ""
    echo "💡 Common solutions:"
    echo "1. Install json-c development files: sudo apt install libjson-c-dev"
    echo "2. Check Apache Axis2/C installation: ls -la /usr/local/axis2c/lib"
    echo "3. Verify include paths exist: ls -la /usr/local/axis2c/include/axis2-2.0.0"
    echo "4. Ensure math library is available: gcc ... -lm"
    exit 1
fi
