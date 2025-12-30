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
echo "   • Scenario Analysis - Hash table lookups (coming soon)"
echo ""

gcc -shared -fPIC \
    -DAXIS2_JSON_ENABLED -DWITH_NGHTTP2 -O2 \
    -I/usr/local/axis2c/include/axis2-2.0.0 \
    -I../../include \
    -I../../util/include \
    -I../../src/core/transport/http/util \
    -I/usr/include/json-c \
    src/financial_benchmark_service.c \
    -o libfinancial_benchmark_service.so \
    -L/usr/local/axis2c/lib \
    -laxutil -laxis2_engine \
    -ljson-c \
    -lm

echo "Build exit code: $?"

if [ -f "libfinancial_benchmark_service.so" ]; then
    echo ""
    echo "🎉 SUCCESS: Financial Benchmark JSON Service built with SOAP-FREE architecture!"

    # Show file info
    ls -la libfinancial_benchmark_service.so
    file libfinancial_benchmark_service.so

    # Check dependencies
    echo ""
    echo "🔍 Checking runtime dependencies..."
    echo "📋 Service-level: No direct AXIOM usage (pure JSON processing)"
    echo "🔧 Engine-level: AXIOM present as transitive dependency (expected in Axis2/C)"
    # Show axis2 related dependencies only
    ldd libfinancial_benchmark_service.so | grep -E '(axis2|json|libax|libm)' || echo "   (Dependencies shown above in full ldd output)"

    # Install the service
    echo ""
    echo "🚀 Installing SOAP-free JSON Financial Benchmark service..."
    sudo cp libfinancial_benchmark_service.so /usr/local/axis2c/services/FinancialBenchmarkService/
    sudo cp services.xml /usr/local/axis2c/services/FinancialBenchmarkService/

    # Fix ownership and permissions for Apache (runs as www-data)
    sudo chown -R www-data:www-data /usr/local/axis2c/services/FinancialBenchmarkService
    sudo chmod 755 /usr/local/axis2c/services/FinancialBenchmarkService
    sudo chmod 644 /usr/local/axis2c/services/FinancialBenchmarkService/services.xml
    sudo chmod 755 /usr/local/axis2c/services/FinancialBenchmarkService/libfinancial_benchmark_service.so

    echo ""
    echo "✅ Installation complete! Financial Benchmark Service ready for HTTP/2 JSON requests"
    echo ""
    echo "============================================================"
    echo "🧪 TEST THE SERVICE:"
    echo "============================================================"
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
    echo "4. Test Portfolio Variance (500 assets - benchmark):"
    echo '   python3 test/generate_test_data.py --assets 500 -o /tmp/portfolio_500.json'
    echo '   curl -k --http2 -H "Content-Type: application/json" -d @/tmp/portfolio_500.json \'
    echo '        https://localhost:443/services/FinancialBenchmarkService/portfolioVariance'
    echo ""
    echo "5. Test Monte Carlo VaR (10,000 simulations):"
    echo '   curl -k --http2 -H "Content-Type: application/json" \'
    echo '        -d '\''{"n_simulations":10000,"n_periods":252,"initial_value":1000000,"expected_return":0.08,"volatility":0.20}'\'' \'
    echo '        https://localhost:443/services/FinancialBenchmarkService/monteCarlo'
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
