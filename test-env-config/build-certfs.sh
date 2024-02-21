#!/bin/sh

mkdir -p /etc/nginx/snippets/

mkdir -p /etc/ssl/private/

mkdir -p /etc/ssl/certs/

printf "Creating self-signed certificate...\n"

openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout \
 /etc/ssl/private/nginx-selfsigned.key -out /etc/ssl/certs/nginx-selfsigned.crt \
  -subj "/C=BR/ST=ES/L=São Mateus/O=Self-Signed/CN=frp-server"

printf "ssl_certificate /etc/ssl/certs/nginx-selfsigned.crt;\n"\
"ssl_certificate_key /etc/ssl/private/nginx-selfsigned.key;\n" | cat > /etc/nginx/snippets/self-signed.conf

openssl dhparam -out /etc/ssl/certs/dhparam.pem 2048

printf "Configuring SSL parameters...\n"

printf "ssl_protocols TLSv1 TLSv1.1 TLSv1.2;\n"\
"ssl_prefer_server_ciphers on;\n"\
"ssl_ciphers \"EECDH+AESGCM:EDH+AESGCM:AES256+EECDH:AES256+EDH\";\n"\
"ssl_ecdh_curve secp384r1;\n"\
"ssl_session_cache shared:SSL:10m;\n"\
"ssl_session_tickets off;\n"\
"ssl_stapling off;\n"\
"ssl_stapling_verify on;\n"\
"resolver 8.8.8.8 8.8.4.4 valid=300s;\n"\
"resolver_timeout 5s;\n"\
"add_header Strict-Transport-Security \"max-age=63072000; includeSubdomains\";\n"\
"add_header X-Frame-Options DENY;\n"\
"add_header X-Content-Type-Options nosniff;\n" | cat > /etc/nginx/snippets/ssl-params.conf

printf "Configuring self-signed certificate... Done. Exiting...\n"