# FRP Client

[![en](https://img.shields.io/badge/lang-en-red)](README_FRPC.md) [![pt-br](https://img.shields.io/badge/lang-pt--br-green)](README_FRPC.pt-br.md)

Este projeto fornece uma imagem de contêiner Docker de fácil configuração, que permite habilitar um proxy reverso personalizado localmente e compartilhar suas aplicações globalmente por meio de um servidor FRP.

## Uso

O uso pretendido é usando o Docker Compose, um arquivo `.credentials` e um arquivo `server_config`, tornando mais fácil de gerenciar.  

A configuração pode ser feita adicionando um arquivo `.env`, como no exemplo `.env.example` e um arquivo `.credentials` como em `.credentials.example`

### Configuração

#### Criar arquivo com as credenciais

Esse projeto inclui um módulo de autentificação para um servidor LDAP genérico. Qualquer combinação de usuário e senha será válido.

Para incluir o endereço de um servidor real e passar a usufruir dessa funcionalidade, faça as alterações:

- Substitua o valor da diretiva `LDAP_HOST` em [authenticate.h](/src/authenticate/authenticate.h) para `ldap:YOUR_LDAP_URL:YOUR_LDAP_PORT`;
- Remova os comentários da função `authenticate_LDAP_user` em [authenticate.c](/src/authenticate/authenticate.c).

Crie um arquivo `.credentials` no mesmo diretório do seu arquivo `docker-compose.yaml`.  

O arquivo `.credentials` deve conter seu nome de usuário e senha separados por um espaço, por exemplo:

```bash
cat > .credentials
```

```bash
<Meu usuário> <Minha senha de acesso> <Aperte enter>
<ctrl-c>
```

#### Arquivo de configuração do servidor

Para adicionar um servidor frp, crie um arquivo `server_config` no mesmo diretório do seu arquivo `docker-compose.yaml`.  

O arquivo `server_config` deve conter o IP e a porta do servidor, separados por `:`, por exemplo:

```bash
cat > server_config
```

```bash
<frp_server_ip>:<frp_server_port><Aperte enterr>
<ctrl-c>
```

#### Personalizando seu proxy com o arquivo .env

Copie o arquivo `.env.example` e renomeie-o para `.env`.  

```bash
PROXY_NAME="Meu web app"
PROXY_TYPE="http"
PROXY_LOCAL_IP=127.0.0.1
PROXY_LOCAL_PORT=3000
CUSTOM_APP_URL='my-app.test'
```

##### PROXY_NAME

Este é o nome do seu serviço para o servidor, não importa para o usuário final, mas é como o frps irá identificá-lo.  

Default: '"your user"_proxy'  

##### PROXY_TYPE

O tipo de serviço que você está oferecendo; pode ser http, https, tcp, udp, ldap, etc...  

Default: http

##### PROXY_LOCAL_IP

O IP local onde seu serviço está em execução.  

Default is 127.0.0.1(localhost)

##### PROXY_LOCAL_PORT

A porta local do seu serviço.  

Default is 3000

##### CUSTOM_APP_URL

O serviço estará disponível por meio da URL personalizada. É preciso que seja um domínio válido e existente.
