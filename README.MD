# TESTING SETUP FOR FRPSERVER WITH HTTPS ENABLED

This provides a full featured setup with:

- (nginx-server) A nginx service for redirecting requests and provide autentification;
- (cert-script) A service with a script for generating self-signed TLS certification;
- (frp-server) A service with frp-server;
- (frp-client) A service with frp-client;
- (wagi-server) A service with a "localhost" application for testing.  

## ARCHITECTURE

The first execution must be cert-script for generating the certifications.  
Then the frp-server can execute, followed by nginx.
At last, the frp-client can run. The wagi-server doest have or is a dependecy.  

### FRP-CLIENT configuration

[README](README_FRPC.md)

## ENV

```.env
PROXY_NAME="wagi-proxy"
PROXY_TYPE="http"
PROXY_LOCAL_IP=wagi-server
PROXY_LOCAL_PORT=3000
LOCAL_WAGI_BIND_PORT=3000
LOCAL_WAGI_BIND_IP=0.0.0.0
```
