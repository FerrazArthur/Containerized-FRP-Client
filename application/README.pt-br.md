# wagi-tests

[![en](https://img.shields.io/badge/lang-en-red)](README.md) [![pt-br](https://img.shields.io/badge/lang-pt--br-green)](README.pt-br.md)

## Módulos

Ambas as aplicações são escritas em C e compiladas para wasm para serem servidas via wagi.

### append

Qualquer mensagem enviada como corpo da solicitação será anexada (cria o arquivo se não existir) ao arquivo "data.txt".

```bash
curl http://localhost:3000/append -d "[Sua mensagem]"\```

### Quicksort

Este é um algoritmo gerador de carga que implementa a busca no pior caso para quicksort O($n^2$).

Para ver todos os argumentos possíveis, digite:

```bash
$ curl http://localhost:3000/quicksort?--help
Argumentos válidos:
        --tamanho <inteiro> -> Quantidade de elementos a serem reordenados
        --imprime -> Ativa impressão dos elementos antes e depois
        --help -> Imprime ajuda
```

#### O caractere de espaço '\\&'

O comando equivalente a

```bash
./executable --tamanho 20 --imprime
```

Precisa substituir ' '(espaço) por '\\&'

```bash
curl http://localhost:3000/quicksort?--tamanho\&20\&--imprime
```
