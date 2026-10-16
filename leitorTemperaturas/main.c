#include <stdio.h>
#include <stdlib.h>

#define RAMINICIAL 100

double calculaMedia(double *, int);
double encontraMaior(double *, int);
double *entradaDados(double *, int *, int *, int *);
double *realocaMemoria(double *, int *);

int main(void)
{
    int qtdAlocacao = RAMINICIAL;

    double *listaLeituras = (double *) malloc(sizeof(double) * qtdAlocacao);
    if (listaLeituras == NULL)
    {
        free(listaLeituras);
        return 1;
    }

    int indice = 0;
    int contador = 0;

    listaLeituras = entradaDados(listaLeituras, &qtdAlocacao, &indice, &contador);
    if (listaLeituras == NULL)
    {
        free(listaLeituras);
        return 1;
    }
    if (contador <= 0)
    {
        printf("sem dados\n");
        free(listaLeituras);
        return 0;
    }

    double media = calculaMedia(listaLeituras, contador);
    double maior = encontraMaior(listaLeituras, contador);
    
    printf("%d leituras, média %.2f, máximo %.2f\n", contador, media, maior);

    free(listaLeituras);
    return 0;
}

double calculaMedia(double *v, int c)
{
    double s = 0;
    
    for (int i = 0; i < c; i++)
    {
        s += v[i];
    }

    return (s/c); // soma / qtd Elementos
}
double encontraMaior(double *v, int c)
{
    double m = v[0];

    for (int i = 1; i < c; i++)
    {
        if (v[i] > m)
        {
            m = v[i];
        }
    }

    return m;
}
double *entradaDados(double *v, int *r, int *id, int *c)
{
    // v = lista de entradas
    // r = qtd alocada para as entradas
    // id = indice da lista de entradas
    // c = contador de entradas

    int leitor;

    while((leitor = scanf("%lf", &v[*id])))
    {
        if (leitor != 1 || leitor == EOF)
        {
            return v;
        }

        (*id)++;
        (*c)++;

        if ((*c) == (*r))
        {
            v = realocaMemoria(v, r);

            if (v == NULL)
            {
                return v;
            }
        }
    }

    return v;
}
double *realocaMemoria(double *v, int *r)
{
    // v = lista de entradas
    // r = qtd alocada para as entradas

    (*r) *= 2;
            
    double *realocador = realloc(v, (sizeof(double) * (*r)));

        if (realocador != NULL)
        {
            return realocador;
        }

        else
        {
            printf("erro\n");
            free(v);
            return NULL;
        }
}