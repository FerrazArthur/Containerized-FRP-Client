# CONFIGURAÇÃO DE TESTE PARA FRP SERVER COMO UPSTREAM DO NGINX

[![en](https://img.shields.io/badge/lang-en-red)](README.md) [![pt-br](https://img.shields.io/badge/lang-pt--br-green)](README.pt-br.md)

Esse repositorio fornece um ambiente completo (exceto um servidor LDAP para autentificação) para testar a aplicação que simplifica a integração do [frp](https://github.com/fatedier/frp) com o docker e nginx.

Inclui os serviços:

- (dnsmasq) Um serviço para configurar um pseudo-TLD (pseudo-Top-Level-Domain) em `127.0.0.1`;
- (traefik) Uma ferramenta utilizada para configurar hostnames locais para os serviços Docker;
- (nginx-server) Um serviço nginx para redirecionar solicitações e fornecer autenticação TLS;
- (cert-script) Um serviço com um script para gerar certificados TLS autoassinados;
- (frp-server) Um serviço com um servidor frp;
- (frp-client) Contém a aplicação para configurar e iniciar um client frp;
- (wagi-server) Um serviço com uma aplicação "localhost" para teste.

## ARQUITETURA

- O dnsmarq está ouvindo na porta 53, e o sistema resolve (resolv) deve priorizar `127.0.0.1`. Ele redirecionará suas solicitações para o serviço traefik;
- O traefik inspeciona a rede do docker compose para os rótulos dos outros servidores. Ele cria um domínio personalizado como my-app.test para o nginx;
- O cert-script executa para gerar os certificados autoassinados e então para;
- O frp-server é executado e fica aguardando requisições;
- Com o frp-server em execução e os certificados, o nginx escutará solicitações https na porta 443 e as redirecionará como http para o servidor frp;
- O frp-client receberá solicitações atravpes do proxy com o frp-server;
- O wagi-server não tem dependência e fornece uma resposta web simples, acessável através do frp-client.

### Configuração do cliente FRP

[README](README_FRPC.pt-br.md)

## ENV

```.env
PROXY_NAME="wagi-proxy"
PROXY_TYPE="http"
PROXY_LOCAL_IP=wagi-server
PROXY_LOCAL_PORT=3000
LOCAL_WAGI_BIND_PORT=3000
LOCAL_WAGI_BIND_IP=0.0.0.0
CUSTOM_APP_URL='my-app.test'
```

### SETUP

Para executar o ambiente, siga as etapas:  

```bash
docker compose up -d
```

Aguarde a conclusão.  
O serviço dnsmasq reclamará que já há algum processo ouvindo na porta 53. Você pode verificar qual serviço ou processo é esse com o commando:  

```bash
sudo netstat -lpn | grep ":53 "
```

No meu caso, usando o Ubuntu Jammy, é o `systemd-resolved`. Se esse é seu caso também, siga o passo abaixo. Do contrário, você pode tentar adaptar o comando ou matar diretamente o processo com um `kill`.

Para parar o serviço systemd-resolved, digite:

```bash
sudo systemctl stop systemd-resolved.service
```

> Seu sistema pode conter mais processos ouvindo nessa porta.  

Reinicie o serviço dnsmasq com:  

```bash
docker compose restart dnsmasq
```

Você precisará alterar o arquivo `/etc/resolv.conf` e adicionar o nameserver `127.0.0.1`.  

> O systemd-resolved, uma vez reiniciado, redefinirá este arquivo, então não é tão problemático editá-lo.  

<details><summary>Exemplo</summary>

Mude de:  

```bash
# Alguns comentários

nameserver 127.0.0.53
options edns0 trust-ad
search .
```

Para:

```bash
# Alguns comentários

nameserver 127.0.0.1 # ADICIONE ESTA LINHA
nameserver 127.0.0.53
options edns0 trust-ad
search .
```

</details>

### TESTANDO

Você pode simplesmente usar o curl, com a opção `-k` para aceitar certificados não confiáveis e receber uma resposta para o servidor wagi, como está configurado.

A documentação da aplicação testada pode ser encontrada [aqui](/application/README.pt-br.md).

```bash
$curl -k https://my-app.test/quicksort?--help
Argumentos válidos:
        --tamanho <inteiro> -> Quantidade de elementos a serem reordenados
        --imprime -> Ativa impressão dos elementos antes e depois
        --help -> Imprime ajuda
```

### FAQ

#### Could not resolve host

Se você receber uma mensagem de erro `Could not resolve host`, digite o comando `docker compose up -d`. Às vezes, quando o contêiner dnsmasq lança o erro `listen tcp4 0.0.0.0:53: bind: address already in use`, o daemon docker interrompe a inicialização de outro contêiner.

#### dial tcp: lookup registry-1.docker.io: no such host

Se você estiver enfrentando esse tipo de comportamento durante a construção do docker ou mesmo no navegador, é porque você parou o resolver do sistema e não o reiniciou. Você pode digitar `sudo systemctl restart systemd-resolved.service` e isso vai corrigir (assumindo que seu resolver seja o systemd. De toda forma, reiniciar o computar também resolve).
