# Quant1-FRPC

[![en](https://img.shields.io/badge/lang-en-red)](README_FRPC.md) [![pt-br](https://img.shields.io/badge/lang-pt--br-green)](README_FRPC.pt-br.md)

This project provides a Docker container image that allows Quant1 developers to enable a personalized reverse proxy locally and share their applications globally via Quant1's FRP server.

## Usage

The intended use of this is using Docker Compose, a `.credentials` and a `server_config` file, making it easier to manage.  

Configuration can be done by adding a `.env` file as in the example `.env.example`.  

### Configuration

#### Create credentials file

Create a `.credentials` file in the same directory as your `docker-compose.yaml` file.  

The `.credentials` file should contain your Quant1 username and password separated by a space, for example:  

```bash
cat > .credentials
```

```bash
<my user> <my password> <press enter>
<press ctrl-c>
```

#### Create server_config file

Create a `server_config` file in the same directory as your `docker-compose.yaml` file.  

The `server_config` file should contain the server host ip and port, separated by a `:`, for example:  

> This file is available within the repository, configured with quant1 frps server ip and port.  

```bash
cat > server_config
```

```bash
<frp_server_ip>:<frp_server_port><press enter>
<press ctrl-c>
```

#### Configure your proxy inside .env file

Copy the `.env.example` file and rename it with `.env`.  

```bash
PROXY_NAME="Meu web app"
PROXY_TYPE="http"
PROXY_LOCAL_IP=127.0.0.1
PROXY_LOCAL_PORT=3000
```

##### PROXY_NAME

This is the name of your service to the server, it doest matter to the final user but is how the frps will identify you.  

Default: '"your quant1 user"_proxy'  

##### PROXY_TYPE

The type of service youre offering, it can be http or https, tcp, udp, ldap, etc...  

Default: http

##### PROXY_LOCAL_IP

The local ip where your service is running.  

Default is 127.0.0.1(localhost)

##### PROXY_LOCAL_PORT

The local port of your service.  

Default is 3000

## Acess your service via web

The service will be available via a url personalized based on your username, obtained by your quant1 username appended with .frp.quant1.com.br.

```bash
"<my_quant1_username>.frp.quant1.com.br"
```
