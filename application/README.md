# wagi-tests

[![en](https://img.shields.io/badge/lang-en-red)](README.md) [![pt-br](https://img.shields.io/badge/lang-pt--br-green)](README.pt-br.md)

## Modules

Both applications are written in C and compiled to wasm in order to be served via wagi.

### append

Any message passed as body of the request is going to be appended (creates the file if doesnt exist) to the file "data.txt".

```bash
curl http://localhost:3000/append -d "[Your message]"
```

### Quicksort

This is a load generator algorithm that implements the wost case search for quicksort O($n^2$).

To see all possible arguments, type:

```bash
$ curl http://localhost:3000/quicksort?--help
Argumentos válidos:
        --tamanho <inteiro> -> Quantidade de elementos a serem reordenados
        --imprime -> Ativa impressão dos elementos antes e depois
        --help -> Imprime ajuda
```

#### The space character '\\&'

The command equivalent to

```bash
./executable --tamanho 20 --imprime
```

Needs to substitute ' '(space) with '\\&'

```bash
curl http://localhost:3000/quicksort?--tamanho\&20\&--imprime
```
