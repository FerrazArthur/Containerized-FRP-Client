#!/bin/sh

# This file intends on passing docker-compose arguments to the service.


config_autenticate_input(){
    cut -d ' ' -f 2 < /run/secrets/QUANT1_CREDENTIALS
}
config_input(){
    config_autenticate_input
    printf "$PROXY_NAME""\n"
    printf "$PROXY_TYPE""\n"
    printf "$PROXY_LOCAL_IP""\n"
    printf "$PROXY_LOCAL_PORT""\n"
}

main_input(){
    # Search for a configfile
    conf=$( find . -type f -name '*_client.toml' )

    # If config file doesnt exists
    if [ -z "$conf" ]; then
        # It's the first execution and we configure the client
        # If QUANT1_USER is not already set as a environment variable
        if [ -z "$QUANT1_USER" ]; then
            cut -d ' ' -f 1 < /run/secrets/QUANT1_CREDENTIALS
        fi
        config_input
    fi
}
main_input
