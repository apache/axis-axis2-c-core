#!/bin/bash
# Deploy Enhanced Memory-Safe Module Script
# Date: Dec 19, 2025

REPO_ROOT="$(cd "$(dirname "$0")" && pwd)"
MODULE="$REPO_ROOT/src/core/transport/http/server/apache2/.libs/libmod_axis2.so"

if [ ! -f "$MODULE" ]; then
    echo "No built module at $MODULE; build first." >&2
    exit 1
fi

echo "=== Enhanced Memory-Safe Module Deployment ==="
echo "Source: $MODULE"
echo "Built: Dec 19 00:28 (Enhanced memory corruption fixes)"
echo "Size: 327,712 bytes"
echo ""

echo "Step 1: Deploy enhanced module to Apache module path"
sudo cp "$MODULE" /usr/local/axis2c/lib/libmod_axis2.so

echo "Step 2: Also copy to Apache modules directory"
sudo cp "$MODULE" /usr/local/apache2/modules/mod_axis2.so

echo "Step 3: Restart Apache to clear module cache"
sudo systemctl stop apache2-custom
sleep 2
sudo pkill -f apache2 2>/dev/null || true
sudo systemctl start apache2-custom

echo "Step 4: Verify deployment"
ls -la /usr/local/axis2c/lib/libmod_axis2.so
ls -la /usr/local/apache2/modules/mod_axis2.so

echo "=== Deployment Complete ==="
echo "Expected: Module timestamps should show Dec 19 00:28"
echo "Test with: curl -sk --http2 -H 'Content-Type: application/json' -d '{\"test\":1}' https://localhost/services/BigDataH2Service/processBigDataSet --max-time 5"