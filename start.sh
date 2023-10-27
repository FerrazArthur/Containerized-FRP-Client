#!/bin/sh

start_server(){
    echo "Iniciando cliente FRP..."

    frpc -c "$1_client.toml"
}

if [ "$USER" == "" ]; then
    echo "Erro: Usuário não definido para \"USER\". Certifique-se de que o container foi configurado corretamente"
    exit 1
fi

start_server "$USER"
