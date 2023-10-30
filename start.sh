#!/bin/sh

start_server(){
    echo "Iniciando cliente FRP..."
    if [ -z "$1" ]; then
        echo "Erro: Usuário não definido. Por favorn certifique-se de que o container foi configurado corretamente."
        exit 1
    fi
    cat "$1_client.toml"
    frpc -c "$1_client.toml"
}


start_server "$1"
