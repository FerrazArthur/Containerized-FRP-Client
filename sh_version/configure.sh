#!/bin/sh

# Check if environment server addr is set. If not, default it
SERVER_ADDR="${SERVER_ADDR:-194.163.176.185}"

SERVER_PORT="${SERVER_PORT:-7000}"

autenticate(){
    # IF $2 is defined, no matter the value, this code will run as not having an interactive shell 
    # Verify if argument is passed
    if [ -z "$1" ]; then
        printf "Erro: É esperado um nome de usuário Quant1 como argumento.\n"
        exit 1
    fi

    # Authenticate user
    # If usage of script is interactive
    if [ -z "$2" ]; then
        ldapwhoami -x -W -H ldap://ldap.quant1.com.br:389 -D "cn=$1,ou=users,dc=ldap,dc=quant1,dc=com,dc=br" > /dev/null
    else
        read -r password
        ldapwhoami -x -H ldap://ldap.quant1.com.br:389 -D "cn=$1,ou=users,dc=ldap,dc=quant1,dc=com,dc=br" -w "$password"  > /dev/null
        password="protected"
    fi

    OUTPUT="$?"

    if [ "$OUTPUT" != "0" ]; then
        printf "Erro: Usuário \"""$1""\" não foi devidamente autentificado.\n"
        exit 1
    else
        printf "Usuário autenticado com sucesso!\n\n"
    fi
}
# Personalize client proxi file

create_config_file(){
    # Setup frp server address
    read -r -p "Insira um nome para o proxy ["$1"-proxy]: " name_value 
    name_value="${name_value:-$1_proxy}" # If empty, assert default value

    read -r -p "Insira o tipo de conexão [http]: " type_value
    type_value="${type_value:-http}"

    read -r -p "Insira o ip local [127.0.0.1]: " ip_value
    ip_value="${ip_value:-127.0.0.1}"

    read -r -p "Insira a porta local [3000]: " local_port_value
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
    
    printf "\nConfiguração concluída! sua aplicação local poderá ser acessada globalmente através do domínio:\n""$custom_domain""\n\n"
}

autenticate "$1" "$2"
create_config_file "$1"
