#!/bin/sh

start_server(){
    printf "Iniciando cliente FRP...\n"
    if [ -z "$1" ]; then
        printf "Erro: Usuário não definido. Por favor, certifique-se de que o container foi configurado corretamente.\n"
        exit 1
    fi
    frpc -c "$1_client.toml"
}


start_server "$1"
