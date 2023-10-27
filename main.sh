#!/bin/sh

# Search for a configfile
conf=$( find . -type f -name '*_client.toml' )

if [ -n "$conf" ]; then
    # Split the username from the file.
    USER="${conf%'_client.toml'}"
else
    # If it doesnt find then it's the first execution and we configure the client
    echo "Insira o usuário quant1:"
    read -r USER
    ./configure.sh "$USER"
fi

export USER

OUTPUT="$?"

if [ "$OUTPUT" != "0" ]; then
    echo "Não foi possível executar o cliente frp."
    exit 1
fi

./start.sh
