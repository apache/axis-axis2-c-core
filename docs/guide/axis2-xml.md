# axis2.xml Configuration

The `axis2.xml` file is the main configuration file for Axis2/C. It controls transports, modules, handlers, and global settings.

## Location

Default locations:

- `$AXIS2C_HOME/axis2.xml`
- `/usr/local/axis2c/axis2.xml`
- Specified via `AXIS2_REPO` environment variable

## Basic Structure

```xml
<axisconfig name="AxisC">
    <!-- Transport receivers -->
    <transportReceiver name="https" class="axis2_http2_receiver">
        <!-- HTTP/2 configuration -->
    </transportReceiver>

    <!-- Transport senders -->
    <transportSender name="https" class="axis2_http2_sender">
        <!-- Client transport configuration -->
    </transportSender>

    <!-- Modules to engage -->
    <module ref="addressing"/>

    <!-- Global handlers -->
    <phaseOrder type="inflow">
        <!-- Incoming request handlers -->
    </phaseOrder>
</axisconfig>
```

## HTTP/2 Configuration

### Enabling HTTP/2

```xml
<transportReceiver name="https" class="axis2_http2_receiver">
    <parameter name="port">8443</parameter>
    <parameter name="enableHTTP2">true</parameter>

    <!-- TLS Configuration -->
    <parameter name="SSLServerCert">/path/to/server.crt</parameter>
    <parameter name="SSLServerKey">/path/to/server.key</parameter>
    <parameter name="SSLCACert">/path/to/ca.crt</parameter>

    <!-- Enable mTLS (client certificate verification) -->
    <parameter name="SSLVerifyClient">require</parameter>
</transportReceiver>
```

### TLS Cipher Suites

```xml
<parameter name="SSLCipherSuite">
    TLS_AES_256_GCM_SHA384:TLS_CHACHA20_POLY1305_SHA256:TLS_AES_128_GCM_SHA256
</parameter>
```

## Service Deployment

### Service Repository

```xml
<parameter name="ServicesDirectory">services</parameter>
<parameter name="EnableAllServices">false</parameter>
```

### Hot Deployment

```xml
<parameter name="EnableHotDeployment">true</parameter>
<parameter name="EnableHotUpdate">true</parameter>
```

## JSON Configuration

### JSON Message Receiver

```xml
<messageReceiver name="json" class="axis2_json_msg_recv">
    <parameter name="prettyPrint">false</parameter>
    <parameter name="contentType">application/json</parameter>
</messageReceiver>
```

## Legacy SOAP Configuration

### SOAP Message Receivers

```xml
<messageReceiver name="soap11" class="axis2_soap_msg_recv"/>
<messageReceiver name="soap12" class="axis2_soap12_msg_recv"/>
```

### WS-Addressing

```xml
<module ref="addressing"/>
```

## Handler Configuration

### Phase Order

```xml
<phaseOrder type="inflow">
    <phase name="Transport"/>
    <phase name="Security"/>
    <phase name="Dispatch"/>
</phaseOrder>

<phaseOrder type="outflow">
    <phase name="MessageOut"/>
    <phase name="Security"/>
    <phase name="Transport"/>
</phaseOrder>
```

## See Also

- [HTTP/2 Configuration Details](../HTTP2_AXIS2_DOT_XML.md)
- [Services Configuration](../HTTP2_SERVICES_DOT_XML.md)
- [Security Guide](../SECURITY.md)
