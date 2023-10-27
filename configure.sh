#!/bin/sh

serverAddr="194.163.176.185"
serverPort="7000"

autenticate(){
    # Verify if argument is passed
    if [ "$1" == "" ]; then
        echo "Erro: É esperado um nome de usuário Quant1 como argumento."
        exit 1
    fi

    # Try to authenticate user
    ldapwhoami -x -W -H ldap://ldap.quant1.com.br:389 -D "cn=$1,ou=users,dc=ldap,dc=quant1,dc=com,dc=br" > /dev/null

    OUTPUT="$?"

    if [ "$OUTPUT" != "0" ]; then
        echo "Erro: Usuário \"""$1""\" não foi devidamente autentificado."
        exit 1
    else
        echo "Usuário autenticado com sucesso!"
    fi
}
# Personalize client proxi file

create_config_file(){
    # Setup frp server address
    echo "Insira um nome para o proxy ("$1"-proxy)"
    read name_value
    name_value="${name_value:-$1_proxy}" # If empty, assert default value

    echo "Insira o tipo de conexão (http)"
    read type_value
    type_value="${type_value:-http}"

    echo "Insira o ip local (127.0.0.1)"
    read ip_value
    ip_value="${ip_value:-127.0.0.1}"

    echo "Insira a porta local (3000)"
    read local_port_value
    local_port_value="${local_port_value:-3000}"

    #custom_domain=$( echo -n "$1" | md5sum | awk '{print $1".cloud.quant1.com.br"}' )
    custom_domain="test.frp.quant1.com.br"

    awk -v addr_field="serverAddr" -v port_field="serverPort" \
        -v addr_value="\"""$serverAddr""\"" -v port_value="$serverPort" \
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
    
    echo "$1"
    echo "Configuration Done! you can enjoy globally acess to you localserver in the url:"
    echo "$custom_domain"
}

autenticate "$1"
create_config_file "$1"
