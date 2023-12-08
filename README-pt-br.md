# CONFIGURAÇÃO DE TESTE PARA FRPSERVER COMO UPSTREAM DO NGINX

[![en](https://img.shields.io/badge/lang-en-red)](README.md) [![pt-br](https://img.shields.io/badge/lang-pt--br-green)](README.pt-br.md)

Esse repositorio fornece uma configuração completa para testar o servidor e cliente frp; ela inclui:

- (dnsmasq) Um serviço para configurar um servidor DNS local básico em 127.0.0.1 que redireciona para o serviço traefik;
- (traefik) Um provedor de DNS local de alto nível para resolver o host de domínio para serviços Docker com fácil configuração usando rótulos (labels);
- (nginx-server) Um serviço nginx para redirecionar solicitações e fornecer autenticação;
- (cert-script) Um serviço com um script para gerar certificados TLS autoassinados;
- (frp-server) Um serviço com frp-server;
- (frp-client) Um serviço com frp-client;
- (wagi-server) Um serviço com uma aplicação "localhost" para teste.

## ARQUITETURA

- O dnsmarq está ouvindo na porta 53, e o sistema resolve (resolv) deve priorizar 127.0.0.1. Ele redireciona suas solicitações para o serviço traefik;
- O traefik inspeciona a rede do docker compose para os rótulos dos outros servidores. Ele ignora a maioria (por causa dos rótulos de ignorar) e cria um domínio personalizado como my-app.test para o nginx;
- O cert-script executa para gerar os certificados autoassinados e então para;
- O frp-server é executado;
- Com o frp-server em execução e os certificados, o nginx escutará solicitações https na porta 443 e as redirecionará como http para o servidor frp;
- O frp-client precisa do frp-server para comunicação e receberá suas solicitações proxy;
- O wagi-server não tem dependência e fornece uma resposta web simples.

### Configuração do FRP-CLIENT

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

Como o dnsmasq escuta a porta 53 para solicitações e o `systemd-resolved` também, o primeiro precisa ser interrompido.

Para executar, sigo as etapas:

```bash
docker compose up -d
```

Aguarde a conclusão.  
O serviço dnsmasq reclamará que já há alguém ouvindo na porta 53. Você pode verificar qual serviço é com:

```bash
sudo netstat -lpn | grep ":53 "
```

No meu caso, usando o Ubuntu Jammy, é o `systemd-resolved`. Pare esse serviço por um tempo; aqui está o comando:

```bash
sudo systemctl stop systemd-resolved.service
```

E, em seguida, reinicie o serviço dnsmasq com:

```bash
docker compose restart dnsmasq -d
```

Agora você só precisará alterar o arquivo `/etc/resolv.conf` e mudar o nameserver para `127.0.0.1`.

> O systemd-resolved, uma vez reiniciado, redefinirá este arquivo, então não é tão problemático editá-lo.

Mude de:

```bash
# Some comments

nameserver 127.0.0.53
options edns0 trust-ad
search .
```

Para:

```bash
# Some comments

nameserver 127.0.0.1 # ALTERE ISTO
options edns0 trust-ad
search .
```

### USO

Você pode simplesmente usar o curl, com a opção `-k` para aceitar certificados não confiáveis e receber uma resposta para o servidor wagi, como está configurado.

```bash
$curl -k https://my-app.test/quicksort?--help
Argumentos válidos:
        --tamanho <inteiro> -> Quantidade de elementos a serem reordenados
        --imprime -> Ativa impressão dos elementos antes e depois
        --help -> Imprime ajuda
```
