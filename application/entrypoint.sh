#!/bin/sh

if [ -z "$LOCAL_SSHD_BIND_IP" ]; then
    LOCAL_SSHD_BIND_IP="0.0.0.0"
fi

if [ -z "$LOCAL_SSHD_BIND_PORT" ]; then
    LOCAL_SSHD_BIND_PORT="22"
fi

/usr/sbin/sshd -D -e -o "ListenAddress $LOCAL_SSHD_BIND_IP" -o "Port $LOCAL_SSHD_BIND_PORT"