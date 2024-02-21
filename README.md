# Cliente FRP

[![en](https://img.shields.io/badge/lang-en-red)](README.md) [![pt-br](https://img.shields.io/badge/lang-pt--br-green)](README.pt-br.md)

This project provides an easily configurable Docker container image, allowing you to enable a custom reverse proxy locally and share your applications globally through an FRP server.

## Usage

The intended use of this is using Docker Compose, a `.credentials` and a `server_config` file, making it easier to manage.  

Configuration can be done by adding a `.env` file as in the example `.env.example` and a file `.credentials` as in the example `.credentiais.example`.

### Configuration

#### Create credentials file

This project includes an authentication module for a generic LDAP server. Any combination of username and password will be valid.

To include the address of a real server and start using this functionality, make the following changes:

- Replace the value of the `LDAP_HOST` directive in [authenticate.h](/src/authenticate/authenticate.h) with `ldap://YOUR_LDAP_URL:YOUR_LDAP_PORT`;
- Uncomment the `authenticate_LDAP_user` function in [authenticate.c](/src/authenticate/authenticate.c).

Create a `.credentials` file in the same directory as your `docker-compose.yaml` file.  

The `.credentials` file should contain your username and password separated by a space, for example:  

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
PROXY_NAME="My web app"
PROXY_TYPE="http"
PROXY_LOCAL_IP=127.0.0.1
PROXY_LOCAL_PORT=3000
CUSTOM_APP_URL='my-app.test'
```

##### PROXY_NAME

This is the name of your service to the server, it doest matter to the final user but is how the frps will identify you.  

Default: '"your user"_proxy'  

##### PROXY_TYPE

The type of service youre offering, it can be http or https, tcp, udp, ldap, etc...  

Default: http

##### PROXY_LOCAL_IP

The local ip where your service is running.  

Default: 127.0.0.1(localhost)

##### PROXY_LOCAL_PORT

The local port of your service.  

Default: 3000

##### CUSTOM_APP_URL

The service will be available via this url. It's imperative that this url is valid and exists.

#### Up and go

You can check how it's supposed to be used in the `test-environment` branch, witch provide a frp server and a pseudoTLD server as "up and go" docker services.
