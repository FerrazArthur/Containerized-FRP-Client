# TESTING SETUP FOR FRP SERVER AS UPSTREAM FROM NGINX

[![en](https://img.shields.io/badge/lang-en-red)](README.md) [![pt-br](https://img.shields.io/badge/lang-pt--br-green)](README.pt-br.md)

This provides a full featured (except for LDAP authentication) setup for testing the application for easier [frp](https://github.com/fatedier/frp) + nginx + docker configuration.

It includes:

- (dnsmasq) A service for setting up a pseudo-TLD (pseudo-Top-Level-Domain) in `127.0.0.1` that redirects to traefik service;
- (traefik) A tool used for setting up local hostnames for docker services via labels;
- (nginx-server) A nginx service for redirecting requests and provide TLS autentification;
- (cert-script) A service with a custom script to generate self-signed TLS certification;
- (frp-server) A service with a frp server;
- (frp-client) Contains the application for configure and start the frp client;
- (wagi-server) A service with a "localhost" application for testing.  

## ARCHITECTURE OVERVIEW

- The dnsmarq is listening in port 53, and the system resolve must be prioritizing `127.0.0.1`. It'll redirect it's requests for traefik service;
- The traefik inspects the docker compose context for the labels of the other servers. It'll create a custom domain as my-app.test for nginx;
- The cert-script executes for generating the self signed certifications then stops;
- The frp-server execute and listen for requests;
- Nginx will listen for https requests on port 443 and redirect them as http to frp server;
- The frp-client receives frp-server proxied requests;
- The wagi-server doest have or is a dependecy and provides a simple web response, acessible via frp-client.

## CONFIGURATIONS

[FRP client configuration](README_FRPC.md)

Create a `.env` file with the following content

```.env
PROXY_NAME="wagi-proxy"
PROXY_TYPE="http"
PROXY_LOCAL_IP=wagi-server
PROXY_LOCAL_PORT=3000
LOCAL_WAGI_BIND_PORT=3000
LOCAL_WAGI_BIND_IP=0.0.0.0
CUSTOM_APP_URL='my-app.test'
```

Create a `.credentials` file with the following content

```.credentials
user password

```

### Setup

To run it, follow the steps:  

```bash
docker compose up -d
```

Wait for it to complete.  
The dnsmasq service will complain that there's already a process listening on port 53. You can check which service/process it is with the command:  

```bash
sudo netstat -lpn | grep ":53 "
```

In my case, using ubuntu jammy, it's `systemd-resolved`. If it's also your case, follow the step bellow. If it's another service, you'll can try to stop it or `kill` the process.  

In order to stop systemd-resolved, type:  

```bash
sudo systemctl stop systemd-resolved.service
```

> Your system may have more processes listening on that port.  

Restart the dnsmasq service with:  

```bash
docker compose restart dnsmasq
```

You'll only have to change the `/etc/resolv.conf` file and add nameserver `127.0.0.1`.

> The systemd-resolved, once restarted, will redefault this file so it's not that big of a deal to edit it.

<details><summary>Example</summary>

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

nameserver 127.0.0.1 # ADD THIS LINE
nameserver 127.0.0.53
options edns0 trust-ad
search .
```

</details>

### Testing

You can simply curl, with option `-k` for accepting untrusted certificated and receive a response. for wagi server such as it's configured.

The documentation for the testing application is can be read [here](/application/README.md).

```bash
$curl -k https://my-app.test/quicksort?--help
Argumentos válidos:
        --tamanho <inteiro> -> Quantidade de elementos a serem reordenados
        --imprime -> Ativa impressão dos elementos antes e depois
        --help -> Imprime ajuda
```

### FAQ

#### Could not resolve host

If you receive a `Could not resolve host` error message, repeat `docker compose up -d`. Sometimes when dnsmasq container throws the `listen tcp4 0.0.0.0:53: bind: address already in use` error, the daemon stops the initialization of another container.

#### dial tcp: lookup registry-1.docker.io: no such host

If you you're getting this kind of behaviour from docker build or even the browser, is because you stopped your system resolver and didnt restarted it. You can type `sudo systemctl restart systemd-resolved.service` and it'll fix it (assuming your resolver is systemd. Anyway, restarting the computer also solves it).
