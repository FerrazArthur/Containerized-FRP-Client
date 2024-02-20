#!/bin/bash

if [ -z "$LOCAL_WAGI_BIND_IP" ]; then
    LOCAL_WAGI_BIND_IP="127.0.0.1"
fi

if [ -z "$LOCAL_WAGI_BIND_PORT" ]; then
    LOCAL_WAGI_BIND_PORT="3000"
fi

wagi -c "modules.toml" -l "$LOCAL_WAGI_BIND_IP"":""$LOCAL_WAGI_BIND_PORT"