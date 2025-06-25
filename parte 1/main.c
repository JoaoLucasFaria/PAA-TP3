#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dyn.h"
#include "shift_and.h"
#include "tp.h"

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("Uso: %s <algoritmo (1=DP, 2=Shift-And)> <arquivo_texto> <arquivo_padroes> <k>\n", argv[0]);
        return 1;
    }

    int algoritmo = atoi(argv[1]);
    int k = atoi(argv[4]);
    char *texto = read_file(argv[2]);
    char **padroes = NULL;
    int num_padroes = 0;
    padroes = read_lines(argv[3], &num_padroes);

    if (k < 0 || k > 3)
    {
        printf("Erro: K deve estar entre 0 e 3.\n");
        return 1;
    }

    if (algoritmo == 1)
    {
        for (int i = 0; i < num_padroes; i++)
            busca_aproximada_dp(texto, padroes[i], k);
    }
    else if (algoritmo == 2)
    {
        for (int i = 0; i < num_padroes; i++)
        {
            printf("%s ", padroes[i]);
            shift_and_approximate(texto, padroes[i], k);
            printf("\n");
        }
    }
    else
    {
        fprintf(stderr, "Algoritmo inválido\n");
    }

    free(texto);
    free_lines(padroes, num_padroes);
    return 0;
}
