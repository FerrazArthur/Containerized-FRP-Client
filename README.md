# TESTING SETUP FOR FRPSERVER AS UPSTREAM FROM NGINX

[![en](https://img.shields.io/badge/lang-en-red)](README.md) [![pt-br](https://img.shields.io/badge/lang-pt--br-green)](README.pt-br.md)

This provides a full featured setup for testing frp's server and client. It's intended to provide the user to access frps proxies via https while the nginx handles the certifications.

It comes packed with:

- (dnsmasq) A service for setting up a basic local DNS server in `127.0.0.1` that redirects to traefik service;
- (traefik) A high level local DNS provider for resolving domain host to docker services with easy configuration with the usage of labels;
- (nginx-server) A nginx service for redirecting requests and provide autentification;
- (cert-script) A service with a script for generating self-signed TLS certification;
- (frp-server) A service with frp-server;
- (frp-client) A service with frp-client;
- (wagi-server) A service with a "localhost" application for testing.  

## ARCHITECTURE

- The dnsmarq is listening in port 53, and the system resolv must be prioritizing `127.0.0.1`. It redirect it's requests for traefik service;
- The traefik inspects the docker compose network it's on for the labels of the other servers. It'll ignore most(because of ignore labels) and create a custom domain as my-app.test for nginx;
- The cert-script executes for generating the self signed certifications then stops;
- The frp-server execute;
- With frp-server running and the certificates, nginx will listen for https requests on port 443 and redirect them as http to frp server;
- The frp-client needs frp-server for communication and receive it's proxied requests;
- The wagi-server doest have or is a dependecy and provides a simple web response.

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

### SETUP

As dnsmasq listen on port 53 for requests and the `systemd-resolved` as well, the former need to me stopped. 

To run, i follow the steps:

```bash
docker compose up -d
```

Wait for it to complete.  
The dnsmasq service will complain that there's already someone listening on port 53. You can check with service it is with:

```bash
sudo netstat -lpn | grep ":53 "
```

In my case, using ubuntu jammy, it's `systemd-resolved`. Stop that service for a while, here the command is:

```bash
sudo systemctl stop systemd-resolved.service
```

And then restart the dnsmasq service with:

```bash
docker compose restart dnsmasq -d
```

Now you'll only have to change the `/etc/resolv.conf` file and change nameserver to ``127.0.0.1``.

> The systemd-resolved, once restarted, will redefault this file so it's not that big of a deal to edit it.

Change it from:

```bash
# Some comments

nameserver 127.0.0.53
options edns0 trust-ad
search .
```

Into:  

```bash
# Some comments

nameserver `127.0.0.1` # CHANGE THISS
options edns0 trust-ad
search .
```

### USAGE 

You can simply curl, with option `-k` for accepting untrusted certificated and receive a response. for wagi server such as it's configured.

```bash
$curl -k https://my-app.test/quicksort?--help
Argumentos válidos:
        --tamanho <inteiro> -> Quantidade de elementos a serem reordenados
        --imprime -> Ativa impressão dos elementos antes e depois
        --help -> Imprime ajuda
```
