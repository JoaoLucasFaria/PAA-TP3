#include "bmh.h"
#include <string.h>
#include <stdlib.h>

#define MAX_CHAR 256

// Cria a tabela de saltos para o algoritmo BMH
static void preprocess_shift_table(const char *padrao, int m, int *shift)
{
    for (int i = 0; i < MAX_CHAR; i++)
        shift[i] = m;

    for (int i = 0; i < m - 1; i++)
        shift[(unsigned char)padrao[i]] = m - i - 1;
}

ResultadoBMH bmh(const char *texto, const char *padrao)
{
    ResultadoBMH resultado;
    resultado.ocorrencias = NULL;
    resultado.quantidade = 0;
    resultado.comparacoes = 0;

    int n = strlen(texto);
    int m = strlen(padrao);

    if (m == 0 || m > n)
        return resultado;

    int shift[MAX_CHAR];
    preprocess_shift_table(padrao, m, shift);

    int capacidade = 10;
    resultado.ocorrencias = (int *)malloc(capacidade * sizeof(int));

    int i = 0;
    while (i <= n - m)
    {
        int j = m - 1;

        // Contagem de comparações e verificação
        while (j >= 0 && padrao[j] == texto[i + j])
        {
            resultado.comparacoes++;
            j--;
        }

        if (j < 0)
        {
            if (resultado.quantidade >= capacidade)
            {
                capacidade *= 2;
                resultado.ocorrencias = (int *)realloc(resultado.ocorrencias, capacidade * sizeof(int));
            }
            resultado.ocorrencias[resultado.quantidade++] = i;
        }
        else
        {
            resultado.comparacoes++;
        }

        i += shift[(unsigned char)texto[i + m - 1]];
    }

    return resultado;
}

// Versão adaptada de BMH para buscar padrões binários em string de bits
ResultadoBMH bmh_binario(const char *texto_bits, const char *padrao_bits)
{
    ResultadoBMH resultado;
    resultado.ocorrencias = NULL;
    resultado.quantidade = 0;
    resultado.comparacoes = 0;

    int n = strlen(texto_bits);
    int m = strlen(padrao_bits);

    if (m == 0 || m > n)
        return resultado;

    int *shift = malloc(256 * sizeof(int));
    for (int i = 0; i < 256; i++)
        shift[i] = m;

    for (int i = 0; i < m - 1; i++)
    {
        shift[(unsigned char)padrao_bits[i]] = m - i - 1;
    }

    int capacidade = 10;
    resultado.ocorrencias = malloc(capacidade * sizeof(int));

    int i = 0;
    while (i <= n - m)
    {
        int j = m - 1;

        while (j >= 0 && padrao_bits[j] == texto_bits[i + j])
        {
            resultado.comparacoes++;
            j--;
        }

        if (j < 0)
        {
            if (resultado.quantidade >= capacidade)
            {
                capacidade *= 2;
                resultado.ocorrencias = realloc(resultado.ocorrencias, capacidade * sizeof(int));
            }
            resultado.ocorrencias[resultado.quantidade++] = i;
        }
        else
        {
            resultado.comparacoes++;
        }

        i += shift[(unsigned char)texto_bits[i + m - 1]];
    }

    free(shift);
    return resultado;
}