#!/bin/sh

# Search for a configfile
conf=$( find . -type f -name '*_client.toml' )

# If config file already exists
if [ -n "$conf" ]; then
    # If QUANT1_USER is not already set as a environment variable
    if [ -z "$QUANT1_USER" ]; then
        QUANT1_USER="${conf%'_client.toml'}"
    fi
else
    # It's the first execution and we configure the client
    # If QUANT1_USER is not already set as a environment variable
    if [ -z "$QUANT1_USER" ]; then
        read -p "Insira o usuário quant1: " QUANT1_USER
    fi
    ./configure.sh "$QUANT1_USER"
fi

OUTPUT="$?"

if [ "$OUTPUT" != "0" ]; then
    printf "Não foi possível executar o cliente frp.\n"
    exit 1
fi
  
./start.sh "$QUANT1_USER"
