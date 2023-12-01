# Quant1-FRPC

[![en](https://img.shields.io/badge/lang-en-red)](README.md) [![pt-br](https://img.shields.io/badge/lang-pt--br-green)](README.pt-br.md)

Este projeto fornece uma imagem de contêiner Docker que permite aos desenvolvedores da Quant1 habilitar um proxy reverso personalizado localmente e compartilhar suas aplicações globalmente por meio do servidor FRP da Quant1.

## Uso

O uso pretendido é usando o Docker Compose, um arquivo `.credentials` e um arquivo `server_config`, tornando mais fácil de gerenciar.  

A configuração pode ser feita adicionando um arquivo `.env`, como no exemplo `.env.example`.

### Configuração

#### Criar arquivo com as credenciais

Crie um arquivo `.credentials` no mesmo diretório do seu arquivo `docker-compose.yaml`.  

O arquivo `.credentials` deve conter seu nome de usuário e senha da Quant1 separados por um espaço, por exemplo:

```bash
cat > .credentials
```

```bash
<Meu usuário quant1> <Minha senha de acesso> <Aperte enter>
<ctrl-c>
```

#### Arquivo de configuração do servidor

Caso deseje alterar o servidor frp default, crie um arquivo `server_config` no mesmo diretório do seu arquivo `docker-compose.yaml`.  

O arquivo `server_config` deve conter o IP e a porta do servidor, separados por `:`, por exemplo:  

> Este arquivo esta disponível no repositório, configurado com o IP e a porta do servidor frps da Quant1.

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
```

##### PROXY_NAME

Este é o nome do seu serviço para o servidor, não importa para o usuário final, mas é como o frps irá identificá-lo.  

Default: '"your quant1 user"_proxy'  

##### PROXY_TYPE

O tipo de serviço que você está oferecendo; pode ser http, https, tcp, udp, ldap, etc...  

Default: http

##### PROXY_LOCAL_IP

O IP local onde seu serviço está em execução.  

Default is 127.0.0.1(localhost)

##### PROXY_LOCAL_PORT

A porta local do seu serviço.  

Default is 3000

## Acesse seu serviço na World Wide Web com sua url personalizada

O serviço estará disponível por meio de uma URL personalizada com seu nome de usuário, seguido do prefixo .frp.quant1.com.br.

```bash
"<my_quant1_username>.frp.quant1.com.br"
```