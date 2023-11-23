# Quant1-FRPC

This project provides a Docker container image that allows Quant1 developers to enable a personalized reverse proxy locally and share their applications globally via Quant1's FRP server.  
Este projeto fornece uma imagem de contêiner Docker que permite aos desenvolvedores da Quant1 habilitar um proxy reverso personalizado localmente e compartilhar suas aplicações globalmente por meio do servidor FRP da Quant1.

## Usage

The intended use of this branch is using Docker Compose, a `.credentials` and a `server_config` file, making it easier to manage.  
O uso pretendido desta branch é usando o Docker Compose, um arquivo `.credentials` e um arquivo `server_config`, tornando mais fácil de gerenciar.  

Configuration can be done by adding a `.env` file as in the example `.env.example`.  
A configuração pode ser feita adicionando um arquivo `.env`, como no exemplo `.env.example`.

### Configuration and deploy

#### Create credentials file

Create a `.credentials` file in the same directory as your `docker-compose.yaml` file.  
Crie um arquivo `.credentials` no mesmo diretório do seu arquivo `docker-compose.yaml`.  

The `.credentials` file should contain your Quant1 username and password separated by a space, for example:  
O arquivo `.credentials` deve conter seu nome de usuário e senha da Quant1 separados por um espaço, por exemplo:

```bash
cat > .credentials
```

```bash
<my user> <my password> <press enter>
<press ctrl-c>
```

#### Create server_config file

Create a `server_config` file in the same directory as your `docker-compose.yaml` file.  
Crie um arquivo `server_config` no mesmo diretório do seu arquivo `docker-compose.yaml`.  

The `server_config` file should contain the server host ip and port, separated by a `:`, for example:  
O arquivo `server_config` deve conter o IP e a porta do servidor, separados por `:`, por exemplo:  

> This file probably already exists and is configured with quant1 frps server ip and port.  

> Este arquivo provavelmente já existe e está configurado com o IP e a porta do servidor frps da Quant1.

```bash
cat > server_config
```

```bash
<frp_server_ip>:<frp_server_port><press enter>
<press ctrl-c>
```

#### Configure your proxy inside .env file

Copy the `.env.example` file and rename it with `.env`.  
Copie o arquivo `.env.example` e renomeie-o para `.env`.  


It should look like the following:  
Deve ficar assim:  

```bash
PROXY_NAME="Meu web app"
PROXY_TYPE="http"
PROXY_LOCAL_IP=127.0.0.1
PROXY_LOCAL_PORT=3000
```

##### PROXY_NAME

This is the name of your service to the server, it doest matter to the final user but is how the frps will identify you.  
Este é o nome do seu serviço para o servidor, não importa para o usuário final, mas é como o frps irá identificá-lo.  

Default: '"your quant1 user"_proxy'  

##### PROXY_TYPE

The type of service youre offering, it can be http or https, tcp, udp, ldap, etc...  
O tipo de serviço que você está oferecendo; pode ser http, https, tcp, udp, ldap, etc...  

Default: http

##### PROXY_LOCAL_IP

The local ip where your service is running.  
O IP local onde seu serviço está em execução.  

Default is 127.0.0.1(localhost)

##### PROXY_LOCAL_PORT

The local port of your service.  
A porta local do seu serviço.  

Default is 3000

#### Up and go

With the .env and the docker-compose.yml in the same folder, as well as a server-config file if you wish to change the default server URL:port, you can start the app.  
Com o arquivo .env e o docker-compose.yml no mesmo diretório, assim como um eventual arquivo server-config para alterar o url:port padrão do servidor frp, você ja pode iniciar o client.  

You can start it with  
Você pode iniciar o client com:  

```bash
docker compose up -d
```

> You may want to see the logs to see your service's url address  

> Você pode querer ver os logs para ver o endereço URL do seu serviço

```bash
docker compose logs quant1-frp-client -f
```

## Acess your service via web

The service will be available via a url personalized based on your username, obtained by applying a md5sum hash to your quant1 username and appending .cloud.quant1.com.br to it.  
O serviço estará disponível por meio de uma URL personalizada com base no seu nome de usuário, obtida aplicando um hash md5sum ao seu nome de usuário Quant1 e anexando .cloud.quant1.com.br a ele.

```bash
printf "<my_quant1_username>" | md5sum | awk '{print $1".cloud.quant1.com.br"}
```

<details>

<summary> Alternative use method </summary>

## Usage with docker container run

The intended use of the container described below is to configure it once, and then it can be started or stopped as needed.  
O uso pretendido do contêiner descrito abaixo é configurá-lo uma vez, e então ele pode ser iniciado ou interrompido conforme necessário.  

Since the configuration requires user input, a TTY is necessary. However, after the initial configuration, you can stop it and then restart it, and it should function properly in a detached state.  
Como a configuração requer entrada do usuário, um TTY é necessário. No entanto, após a configuração inicial, você pode pará-lo e depois reiniciá-lo, e ele deve funcionar corretamente em um estado desconectado.

### Creating the container

```bash
docker container run --name Quant1-frpc -it --network host registry.quant1.com.br/arthur/quant1-frpc
```

### Stop/starting it

To stop the execution  
Para parar a execução  

```bash
docker container stop Quant1-frpc
```

To restart the execution
Para reiniciar a execução  

```bash
docker container start Quant1-frpc
```

</details>
