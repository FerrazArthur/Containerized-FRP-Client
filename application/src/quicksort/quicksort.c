#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define HELP 505
#define READ_INT 101

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void speed_quicksearch(int n, int imprime) {
    int *arr = NULL;
    arr = (int*) malloc(sizeof(int)*n);
    int inv = n;
    for (int i = 0; i < n; i++)
        arr[i] = inv--;

    if (imprime == 1) {
        printf("Array antes da ordenação:\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    quickSort(arr, 0, n - 1);

    if (imprime == 1) {
        printf("Array ordenado:\n");
        for (int i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }
}

int get_number(const char *arg) {
    // Inicialize num como uma string vazia
    char num[20] = "";  // Defina o tamanho conforme necessário

    // Variável para rastrear a posição em num
    int num_index = 0;

    // Itera através dos caracteres em arg
    for (int i = 0; arg[i] != '\0'; i++) {
        // Se o caractere for um dígito, adicione-o a num
        if (isdigit(arg[i])) {
            num[num_index] = arg[i];
            num_index++;
        } else {
            // Se encontrar qualquer caractere não dígito, pare de ler
            break;
        }
    }

    // Garanta que num seja uma string válida
    num[num_index] = '\0';

    // Converta num em um inteiro usando atoi
    return atoi(num);
}

int process_args(int *imprime, char *arg) {
    if (strstr(arg, "help") != NULL)
        return HELP;
    else if (strstr(arg, "imprime") != NULL)
    {
        *imprime = 1;
        return 0;
    }
    else if (strstr(arg, "tamanho") != NULL)
        return READ_INT;
    else
        return 1;
}

int main(int argc,char *argv[]) {
    char *hint = "Argumentos válidos:\n\
        --tamanho <inteiro> -> Quantidade de elementos a serem reordenados\n\
        --imprime -> Ativa impressão dos elementos antes e depois\n\
        --help -> Imprime ajuda\n";

    int imprime = 0;

    int n = 3000;

    int code_return = 0;

    printf("%s\n", "Content-Type:application/c\n");

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--", 2) == 0) {
            code_return = process_args(&imprime, argv[i]+2);
            
            //tenta ler o novo valor para tamanho
            if (code_return == READ_INT) {
                if (i+1 < argc)
                {
                    n = get_number(argv[++i]);
                    code_return = 0;
                }
                else
                    code_return = 1;
            }
        }
        else
            code_return = 1;
    }


    if (code_return == HELP) {
        printf("%s", hint);
        return 0;
    }
    else if (code_return == 1){
        printf("Argumento não reconhecido. Use \"--help\" para ver os \
argumentos válidos.\n");
        return 1;
    }

    speed_quicksearch(n, imprime);

    return 0;
}


