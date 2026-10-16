#include <stdio.h>
#include <stdlib.h>

#define RAMINICIAL 100

typedef struct
{
    int id;
    int statusHTTP;
    double latencia;
}Evento;

Evento *entradaDados(int *);
Evento *realocarMemoria(Evento *, int *);
Evento *verificaSLA(Evento *, double, int *, int *);
Evento maiorLatencia(Evento *, int);

int main(void)
{
    // declara e lê limite latência
    double limiteLatencia;
    int leitor = scanf("%lf", &limiteLatencia);
    if (leitor != 1 || leitor == EOF)
    {
        printf("erro\n");
        return -1;
    }

    // coleta eventos
    int contador = 0;
    Evento *listaEventos = entradaDados(&contador);
    if (listaEventos == NULL)
    {
        printf("Erro");
        return -1;
    }
    if (contador <= 0)
    {
        printf("sem dados\n");
        free(listaEventos);
        return 0;
    }
    
    // verifica eventos SLA
    int contadorSLA = 0;
    Evento *listaSLA = verificaSLA(listaEventos, limiteLatencia, &contador, &contadorSLA);
    if (listaSLA == NULL)
    {
        printf("Erro main listaSLA\n");
        free (listaEventos);
        return 0;
    }

    // variaveis de saida
    float P = (contadorSLA * 100) / (float)contador;
    Evento mLatencia = maiorLatencia(listaSLA, contadorSLA);

    if (contadorSLA > 0)
    {
        printf("total: %d eventos, violacoes: %d (%.2f pct), pior_latencia: %.2f ms (servico %d)", contador, contadorSLA, P, mLatencia.latencia, mLatencia.id);
    }
    else
    {
        printf("total: %d eventos, sla intacto", contador);
    }

    free(listaEventos);
    free(listaSLA);
    return 0;
}

Evento *entradaDados(int *c)
{   
    int qtdAlocada = RAMINICIAL;
    Evento *v = (Evento *) malloc(sizeof(Evento) * qtdAlocada);
    if (v == NULL)
    {
        printf("Erro na entrada de dados\n");
        return NULL;
    }

    
    int leitor;
    int indice = 0;

    while((leitor = scanf("%d %d %lf", &(v[indice].id), &(v[indice].statusHTTP), &(v[indice].latencia))))
    {
        if (leitor != 3)
        {
            return v;
        }

        (*c)++;
        indice++;

        if (qtdAlocada == (*c))
        {
            v = realocarMemoria(v, &qtdAlocada);
            if (v == NULL)
            {
                printf("Erro na entrada de dados -> realocacao\n");
                return NULL;
            }
        }
    }

    return v;
}
Evento *realocarMemoria(Evento *v, int *q)
{
    (*q) *= 2;

    Evento *realocador = realloc(v, sizeof(Evento) * (*q));

    if (realocador == NULL)
    {
        printf("Erro ao realocar, ponteiro nulo\n");
        free(v);
        return NULL;
    }

    return realocador;
}
Evento *verificaSLA(Evento *v, double l, int *c, int *ccla) 
{
    int qtdAlocada = RAMINICIAL;
    Evento *lViolacoes = malloc(sizeof(Evento) * qtdAlocada);

    for (int i = 0; i < (*c); i++)
    {
        if((v[i].latencia) > l)
        {
            lViolacoes[(*ccla)] = v[i];
            (*ccla)++;
        }

        else if((v[i].statusHTTP) >= 500)
        {
            lViolacoes[(*ccla)] = v[i];
            (*ccla)++;
        }

        if ((*ccla) == qtdAlocada)
        {
            lViolacoes = realocarMemoria(lViolacoes, &qtdAlocada);
            if (lViolacoes == NULL)
            {
                printf("Erro realocao lista lViolacoes do SLA\n");
                return NULL;
            }
        }
    }

    return lViolacoes;
}
Evento maiorLatencia(Evento *v, int t)
{
    Evento m = v[0];

    for (int i = 0; i < t; i++)
    {
        if (v[i].latencia > m.latencia)
        {
            m = v[i];
        }
    }

    return m;
}