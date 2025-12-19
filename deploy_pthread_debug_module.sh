#!/bin/bash
# Deploy enhanced Apache module with pthread debugging
set -e

echo "Deploying enhanced module with pthread debugging..."
echo "Module timestamp: $(stat -c '%y' /home/robert/w2/axis-axis2-c-core/src/core/transport/http/server/apache2/.libs/libmod_axis2.so)"

# Stop Apache
echo "Stopping Apache..."
sudo systemctl stop apache2-custom

# Deploy to both locations
echo "Copying enhanced module to Apache modules directory..."
sudo cp /home/robert/w2/axis-axis2-c-core/src/core/transport/http/server/apache2/.libs/libmod_axis2.so /usr/local/apache2/modules/mod_axis2.so

echo "Copying enhanced module to axis2c lib directory..."
sudo cp /home/robert/w2/axis-axis2-c-core/src/core/transport/http/server/apache2/.libs/libmod_axis2.so /usr/local/axis2c/lib/libmod_axis2.so.0.7.0

# Verify deployment
echo "Verifying deployment timestamps..."
ls -la /usr/local/apache2/modules/mod_axis2.so
ls -la /usr/local/axis2c/lib/libmod_axis2.so.0.7.0

# Start Apache
echo "Starting Apache with enhanced pthread debugging module..."
sudo systemctl start apache2-custom

echo "Deployment complete. Enhanced module with pthread debugging is now active."
echo "Test with: curl -sk --http2 -H \"Content-Type: application/json\" -d '{\"test\":\"pthread_debug\"}' https://localhost/services/BigDataH2Service/processBigDataSet --max-time 5"