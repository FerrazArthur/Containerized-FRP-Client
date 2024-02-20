#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#define MAXLEN 10000
#define EXTRA 5
/* add 5 to account for prefix "data="*/
#define MAXINPUT MAXLEN  + EXTRA + 2
/* 1 for added line break, 1 for trailing NUL */
#define DATAFILE "/data.txt"

void unencode(char *src, char *last, char *dest) {
    // converte strings formato URL para formato normal
    for (; src != last; src++, dest++) {
        if (*src == '+')
            *dest = ' ';
        else if (*src == '%') {
            int code;
            if (sscanf(src + 1, "%2x", &code) != 1)
                code = '?';
            *dest = code;
            src += 2;
        } else
            *dest = *src;
    }
    *dest = '\n';
    *++dest = '\0';
}

int main(void) {
    char *lenstr;
    char input[MAXINPUT], data[MAXINPUT];
    long len;
    printf("%s%c%c\n", "Content-Type:text/html;charset=iso-8859-1", 13, 10);
    lenstr = getenv("CONTENT_LENGTH");
    if (lenstr == NULL || sscanf(lenstr, "%ld", &len) != 1 || len > MAXLEN)
        printf("Erro de invocação, Forma incorreta.\n");
    else {
        FILE *f;
        fgets(input, len + 1, stdin);
        unencode(input, input + len, data);
        f = fopen(DATAFILE, "a");
        if (f == NULL)
            printf("Desculpe, não podemos armazenar seus dados.\n");
        else
        {
            //if data starts with "data=", ignore it;
            if (strncmp(data, "data=", EXTRA) != 0)
                fputs(data, f);
            else
                fputs(data+EXTRA, f);
            fclose(f);
            printf("Obrigado! sua contribuição foi armazenada!\n");
        }
    }
    return 0;
}
