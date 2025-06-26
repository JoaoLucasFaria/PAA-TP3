#include "tp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_LINHA 1024

char *ler_arquivo_texto(const char *caminho)
{
    FILE *fp = fopen(caminho, "r");
    if (!fp)
        return NULL;

    fseek(fp, 0, SEEK_END);
    long tamanho = ftell(fp);
    rewind(fp);

    char *buffer = (char *)malloc(tamanho + 1);
    if (!buffer)
        return NULL;

    fread(buffer, 1, tamanho, fp);
    buffer[tamanho] = '\0';

    fclose(fp);
    return buffer;
}

char **ler_padroes(const char *caminho, int *quantidade)
{
    FILE *fp = fopen(caminho, "r");
    if (!fp)
        return NULL;

    int capacidade = 10;
    int count = 0;
    char **padroes = (char **)malloc(capacidade * sizeof(char *));
    char linha[TAM_LINHA];

    while (fgets(linha, TAM_LINHA, fp))
    {
        linha[strcspn(linha, "\r\n")] = '\0'; // remove \n ou \r\n

        if (count >= capacidade)
        {
            capacidade *= 2;
            padroes = realloc(padroes, capacidade * sizeof(char *));
        }

        padroes[count] = strdup(linha);
        count++;
    }

    fclose(fp);
    *quantidade = count;
    return padroes;
}

void escrever_resultado_comparativo(
    const char *caminho_saida,
    char **padroes,
    int qtd_padroes,
    int **ocorrencias_nao_comp,
    int *qtd_ocorrencias_nao_comp,
    int *comparacoes_nao_comp,
    int **ocorrencias_comp,
    int *qtd_ocorrencias_comp,
    int *comparacoes_comp)
{
    FILE *fp = fopen(caminho_saida, "w");
    if (!fp)
    {
        fprintf(stderr, "Erro ao abrir o arquivo de saída: %s\n", caminho_saida);
        return;
    }

    fprintf(fp, "==== Desempenho BMH em texto NÃO comprimido ====\n");

    for (int i = 0; i < qtd_padroes; i++)
    {
        fprintf(fp, "%s", padroes[i]);
        for (int j = 0; j < qtd_ocorrencias_nao_comp[i]; j++)
        {
            fprintf(fp, " %d", ocorrencias_nao_comp[i][j]+1);
        }
        fprintf(fp, "\n(%d comparações)\n\n", comparacoes_nao_comp[i]);
    }

    fprintf(fp, "==== Desempenho BMH em arquivo COMPRIMIDO ====\n");

    for (int i = 0; i < qtd_padroes; i++)
    {
        fprintf(fp, "%s", padroes[i]);
        for (int j = 0; j < qtd_ocorrencias_comp[i]; j++)
        {
            fprintf(fp, " %d", ocorrencias_comp[i][j]+1);
        }
        fprintf(fp, "\n(%d comparações)\n\n", comparacoes_comp[i]);
    }

    fclose(fp);
}

char *carregar_bits_do_binario(const char *caminho)
{
    FILE *fp = fopen(caminho, "rb");
    if (!fp)
        return NULL;

    // Pular a árvore Huffman até o marcador '\0'
    int c;
    while ((c = fgetc(fp)) != EOF && c != '\0')
        ;

    // Agora estamos no início dos dados comprimidos
    long pos_dados = ftell(fp);
    fseek(fp, 0, SEEK_END);
    long fim = ftell(fp);
    long bytes_dados = fim - pos_dados;
    fseek(fp, pos_dados, SEEK_SET);

    char *bits = malloc(bytes_dados * 8 + 1);
    if (!bits)
    {
        fclose(fp);
        return NULL;
    }

    int i = 0;
    while ((c = fgetc(fp)) != EOF)
    {
        for (int j = 7; j >= 0; j--)
            bits[i++] = ((c >> j) & 1) ? '1' : '0';
    }
    bits[i] = '\0';

    fclose(fp);
    return bits;
}

void liberar_padroes(char **padroes, int quantidade)
{
    for (int i = 0; i < quantidade; i++)
        free(padroes[i]);
    free(padroes);
}
