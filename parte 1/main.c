
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "dyn.h"
#include "shift_and.h"
#include "tp.h"

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        printf("Uso: %s <algoritmo (1=DP, 2=Shift-And)> <arquivo_texto> <arquivo_padroes> <arquivo_saida> <k>\n", argv[0]);
        return 1;
    }

    int algoritmo = atoi(argv[1]);
    char *arquivo_saida = argv[4];
    int k = atoi(argv[5]);

    if (k < 0 || k > 3)
    {
        printf("Erro: K deve estar entre 0 e 3.\n");
        return 1;
    }

    char *texto = read_file(argv[2]);
    int num_padroes = 0;
    char **padroes = read_lines(argv[3], &num_padroes);

    struct timeval inicio_real, fim_real;
    gettimeofday(&inicio_real, NULL);
    clock_t inicio_cpu = clock();

    FILE *saida = freopen(arquivo_saida, "w", stdout);
    if (!saida)
    {
        perror("Erro ao abrir arquivo de saída");
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
        return 1;
    }

    fflush(stdout);
    fclose(stdout);
    freopen("/dev/tty", "w", stdout);

    clock_t fim_cpu = clock();
    gettimeofday(&fim_real, NULL);

    double tempo_cpu = (double)(fim_cpu - inicio_cpu) / CLOCKS_PER_SEC;
    double tempo_real = (fim_real.tv_sec - inicio_real.tv_sec) + (fim_real.tv_usec - inicio_real.tv_usec) / 1000000.0;

    printf("Tempo de relógio: %.6f segundos\n", tempo_real);
    printf("Tempo de usuário: %.6f segundos\n", tempo_cpu);

    free(texto);
    free_lines(padroes, num_padroes);
    return 0;
}
