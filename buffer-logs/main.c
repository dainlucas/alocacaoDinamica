#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CAPACIDADE_INICIAL 4

typedef struct
{
    int severidade;
    char *mensagem;
}Registro;

Registro *realocarMemoria(Registro *, int *, int);
void freeVetor(Registro *, int);

int main(void)
{
    int severidadeMinima;
    if (scanf("%d", &severidadeMinima) != 1) {
        printf("sem dados\n");
        return 0;
    }

    int qtdRegistro = CAPACIDADE_INICIAL;

    Registro *lista = malloc(sizeof(Registro) * qtdRegistro);
    if (lista == NULL)
    {
        free(lista);
        return 1;
    }

    int leitor1, leitor2;
    int qtdCaracteres = 0;
    char bufferTemp[256];
    int indiceLista = 0;
    int maiorCaracteres = 0;
    int indiceMaiorCaracteres = 0;

    while(1)
    {
        leitor1 = scanf("%d", &lista[indiceLista].severidade);
        if (leitor1 != 1) // verificação de entrada int
        {
            break;
        }
        
        leitor2 = scanf("%255s", bufferTemp);
        if (leitor2 != 1 || leitor2 == EOF) //verificação de entrada string
        {
            break;
        }

        qtdCaracteres = strlen(bufferTemp);
        if (qtdCaracteres > maiorCaracteres && lista[indiceLista].severidade >= severidadeMinima)
        {
            maiorCaracteres = qtdCaracteres;
            indiceMaiorCaracteres = indiceLista;
        }

        lista[indiceLista].mensagem = malloc(qtdCaracteres + 1);
        if (lista[indiceLista].mensagem == NULL) //verificação de alocação
        {
            freeVetor(lista, indiceLista);
            free(lista);
            return 1;
        } 
        lista[indiceLista].mensagem = strcpy(lista[indiceLista].mensagem, bufferTemp);

        indiceLista++;

        if (indiceLista == qtdRegistro) // verifica se o espaço lotou
        {
            lista = realocarMemoria(lista, &qtdRegistro, indiceLista);
            if (lista == NULL)
            {
                return 1;
            }
        }
    }

    if (indiceLista <= 0)
    {
        printf("sem dados\n");
        freeVetor(lista, indiceLista);
        free(lista);
        return 0;
    }

    int qtdCriticos = 0;
    int atingeSeveridadeMinima = 0;
    for (int i = 0; i < indiceLista; i++) // verifica se nenhum atingiu severidade minima
    {
        if (lista[i].severidade >= severidadeMinima)
        {
            atingeSeveridadeMinima = 1;
            qtdCriticos++;
        }
    }
    
    if (!atingeSeveridadeMinima)
    {
        printf("total: %d logs, nenhum log com severidade >= %d\n", indiceLista, severidadeMinima);
    }
    else
    {
        printf("total: %d logs, "
               "criticos: %d, "
               "maior_mensagem: %s (tam %d)\n",indiceLista, qtdCriticos, lista[indiceMaiorCaracteres].mensagem, maiorCaracteres
               );
    }

    freeVetor(lista, indiceLista);
    free(lista);
    return 0;
}

Registro *realocarMemoria(Registro *v, int *qt, int t)
{
    Registro *realocador = NULL;

    (*qt) *= 2;

    realocador = realloc(v, (sizeof(Registro) * (*qt)));
    if (realocador == NULL)
    {
        freeVetor(v, t);
        free(v);
    }

    return realocador;
}
void freeVetor(Registro *v, int t)
{
    // t = tamanho da lista de registros

    for (int i = 0; i < t; i++)
    {
        free(v[i].mensagem);
    }

    return;
}