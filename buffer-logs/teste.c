#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int valor1;
    int valor2;
    char *texto;
}Bloco;

int main(void)
{
    int tamanho = 2;
    Bloco *lista;

    lista = malloc(sizeof(Bloco) * tamanho);

    lista[0].valor1 = 1;
    lista[0].valor2 = 1;
    lista[0].texto = malloc(10);
    strcpy(lista[0].texto, "bloco 1");

    lista[1].valor1 = 2;
    lista[1].valor2 = 2;
    lista[1].texto = malloc(10);
    strcpy(lista[1].texto, "bloco 2");

    for (int i = 0; i < tamanho; i++)
    {
        printf("(%d %d)", lista[i].valor1, lista[i].valor2);
        printf(" - %s\n", lista[i].texto);
    }

    for (int i = 0; i < tamanho; i++)
    {
        free(lista[i].texto);
    }

    free(lista);

    return 0;
}