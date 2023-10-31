#!/bin/sh

# Check if environment server addr is set. If not, default it
SERVER_ADDR="${SERVER_ADDR:-194.163.176.185}"

SERVER_PORT="${SERVER_PORT:-7000}"

autenticate(){
    # Verify if argument is passed
    if [ -z "$1" ]; then
        printf "Erro: É esperado um nome de usuário Quant1 como argumento.\n"
        exit 1
    fi

    # Try to authenticate user
    ldapwhoami -x -W -H ldap://ldap.quant1.com.br:389 -D "cn=$1,ou=users,dc=ldap,dc=quant1,dc=com,dc=br" > /dev/null

    OUTPUT="$?"

    if [ "$OUTPUT" != "0" ]; then
        printf "Erro: Usuário \"""$1""\" não foi devidamente autentificado.\n"
        exit 1
    else
        printf "Usuário autenticado com sucesso!\n"
    fi
}
# Personalize client proxi file

create_config_file(){
    # Setup frp server address
    read -p "Insira um nome para o proxy ["$1"-proxy]: " name_value 
    name_value="${name_value:-$1_proxy}" # If empty, assert default value

    read -p "Insira o tipo de conexão [http]: " type_value
    type_value="${type_value:-http}"

    
    read -p "Insira o ip local [127.0.0.1]: " ip_value
    ip_value="${ip_value:-127.0.0.1}"

    
    read -p "Insira a porta local [3000]: " local_port_value
    local_port_value="${local_port_value:-3000}"

    #custom_domain=$( printf "$1" | md5sum | awk '{print $1".cloud.quant1.com.br"}' )
    custom_domain="test.frp.quant1.com.br"

    awk -v addr_field="serverAddr" -v addr_value="\"""$SERVER_ADDR""\"" \
        -v port_field="serverPort" -v port_value="$SERVER_PORT" \
        -v name_field="name" -v name_value="\"""$name_value""\"" \
        -v type_field="type" -v type_value="\"""$type_value""\"" \
        -v local_port_field="localPort" -v local_port_value="$local_port_value" \
        -v ip_field="localIp" -v ip_value="\"""$ip_value""\"" \
        -v dom_field="customDomains" -v dom_value="[\"""$custom_domain""\"]" \
        'BEGIN { FS = " *= *"; OFS = " = " } \
        $1 == addr_field { $2 = addr_value } \
        $1 == port_field { $2 = port_value } \
        $1 == name_field { $2 = name_value } \
        $1 == type_field { $2 = type_value } \
        $1 == local_port_field { $2 = local_port_value } \
        $1 == ip_field { $2 = ip_value } \
        /^localPort/ { print dom_field " = " dom_value }
        { print }' "frpc.toml" > temp && mv temp "$1_client.toml"
    awk '!/^remotePort/' "$1_client.toml" > temp && mv temp "$1_client.toml"
    
    printf "Configuration Done! you can enjoy globally acess to you localserver in the url:\n""$custom_domain""\n\n"
}

autenticate "$1"
create_config_file "$1"
