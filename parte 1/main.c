#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dyn.h"
#include "shift_and.h"
#include "tp.h"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Uso: %s <algoritmo (1=DP, 2=Shift-And)> <arquivo_texto> <arquivo_padroes> <k>\n", argv[0]);
        return 1;
    }

    int algoritmo = atoi(argv[1]);
    int k = atoi(argv[4]);
    char *texto = read_file(argv[2]);
    char **padroes = NULL;
    int num_padroes = 0;
    padroes = read_lines(argv[3], &num_padroes);

    for (int i = 0; i < num_padroes; i++) {
        if (algoritmo == 1) {
            busca_aproximada_dp(texto, padroes[i], k);
        } else if (algoritmo == 2) {
            busca_aproximada_shiftand(texto, padroes[i], k);  // depois passamos k aqui também
        } else {
            fprintf(stderr, "Algoritmo inválido\n");
            break;
        }
    }

    free(texto);
    free_lines(padroes, num_padroes);
    return 0;
}
