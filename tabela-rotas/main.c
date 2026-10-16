#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACIDADE_INICIAL 4

void freeVetor(char **v, int t);

int main()
{
    char **slots;
    char *rota;

    int memSlots = CAPACIDADE_INICIAL;
    slots = malloc(sizeof(char *) * memSlots); // 8 * memSlots ou seja, 4 ponteiros de rota
    if (slots == NULL)
    {
        return 1;
    }
    
    int leitor, qtdCaracteres;
    char buffer[256];
    int indiceS = 0;
    int indiceR = 0;

    while(1)
    {
        leitor = scanf("%255s", buffer);
        if (leitor != 1)
        {
            break;
        }
        qtdCaracteres = strlen(buffer);

        rota = malloc(sizeof(char) * (qtdCaracteres + 1));
        if (rota == NULL)
        {
            freeVetor(slots, indiceS);
            free(slots);
            return 1;
        }
        else
        {
            if (indiceS == memSlots)
            {
                memSlots *= 2;
                char **realocador;
                realocador = realloc(slots, sizeof(slots) * memSlots);
                
                if (realocador == NULL)
                {
                    freeVetor(slots, indiceS);
                    free(slots);
                    free(rota);
                    return 1;
                }

                slots = realocador;
            }

            slots[indiceS] = rota;
            slots[indiceS] = strcpy(slots[indiceS], buffer); // slots[i] == *(slots + i)

            indiceS++;
        }

    }

    if (indiceS == 0)
    {
        printf("sem dados\n");
        freeVetor(slots, indiceR);
        free(slots);
        return 0;
    }

    char *auxiliar;

    for (int i = 0; i < indiceS; i++)
    {
        for (int j = i+1; j < indiceS; j++)
        {
            if (strlen(slots[j]) > strlen(slots[i]))
            {
                auxiliar = slots[j];
                slots[j] = slots[i];
                slots[i] = auxiliar;
            }

            else if (strlen(slots[j]) == strlen(slots[i]))
            {
                if (strcmp(slots[j], slots[i]) < 0)
                {
                    auxiliar = slots[j];
                    slots[j] = slots[i];
                    slots[i] = auxiliar;
                }
            }
        }
    }

    int indiceMaior = 0;
    for (int i = 1; i < indiceS; i++)
    {
        if (strlen(slots[i]) > strlen(slots[indiceMaior]))
        {
            indiceMaior = i;
        }
    }

    printf("total: %d rotas, maior: %s (tam %d)\nordem: ", indiceS, slots[indiceMaior], strlen(slots[indiceMaior]));

    for (int i = 0 ; i < indiceS; i++)
    {
        printf("%s", slots[i]);
        if (i + 1 >= indiceS)
        {
            printf("\n");
        }
        else
        {
            printf(" ");
        }
    }

    freeVetor(slots, indiceS);
    free(slots);
    return 0;
}

void freeVetor(char **v, int t)
{
    for (int i = 0; i < t; i++)
    {
        free(v[i]);
    }
    return;
}