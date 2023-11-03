# Quant1-FRPC

This project provides a Docker container image that allows Quant1 developers to enable a personalized reverse proxy locally and share their applications globally via Quant1's FRP server.

## Usage

The intended use of this branch is using Docker Compose and a `.credentials` file, making it easier to manage.

### Configuration and deploy

#### Create credentials file

Create a `.credentials` file in the same directory as your `docker-compose.yaml` file. The `.credentials` file should contain your Quant1 username and password separated by a space, for example:

```bash
cat > .credentials
```

```bash
<my user> <my password> <press enter>
<press ctrl-c>
```

#### Configure your proxy

##### PROXY_NAME

This is the name of your service to the server, it doest matter to the final user but is how the frps will identify you. Default: '"your quant1 user"_proxy'

##### PROXY_TYPE

The type of service youre offering, it can be http or https, tcp, udp, ldap, etc... Default: http

##### PROXY_LOCAL_IP

The local ip where your service is running. Default is 127.0.0.1(localhost)

##### PROXY_LOCAL_PORT

The local port of your service. Default is 3000

#### Up and go

With that file and the docker-compose.yml in the same folder, you can then start the app with

```bash
docker compose up -d
```

> You may want to see the logs to see your service's url address

```bash
docker compose logs quant1-frp-client
```

## Acess your service via web

The service will be available via a url personalized based on your username, obtained by applying a md5sum hash to your quant1 username and appending .cloud.quant1.com.br to it.

```bash
printf "arthur" | md5sum | awk '{print $1".cloud.quant1.com.br"}
```
