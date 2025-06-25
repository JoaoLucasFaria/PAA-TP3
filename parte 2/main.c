#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bmh.h"
#include "tp.h"
#include "huffman.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Uso: %s <arquivo_texto> <arquivo_padroes> <arquivo_saida>\n", argv[0]);
        return 1;
    }

    const char *caminho_texto = argv[1];
    const char *caminho_padroes = argv[2];
    const char *caminho_saida = argv[3];

    // Etapa 1: Comprimir o texto
    if (huffman_comprimir_com_marcacao(caminho_texto, "texto_comp.bin") != 0)
    {
        fprintf(stderr, "Erro ao comprimir o texto.\n");
        return 1;
    }

    // Etapa 2: Carregar a tabela de Huffman (necessário para codificar os padrões)
    if (huffman_carregar_tabela() != 0)
    {
        fprintf(stderr, "Erro ao carregar a tabela de Huffman.\n");
        return 1;
    }

    // Etapa 3: Ler os padrões do arquivo
    int qtd_padroes = 0;
    char **padroes = ler_padroes(caminho_padroes, &qtd_padroes);
    if (!padroes)
    {
        fprintf(stderr, "Erro ao ler o arquivo de padrões.\n");
        return 1;
    }

    // Etapa 4: Codificar os padrões com a árvore de Huffman
    char **padroes_codificados = malloc(qtd_padroes * sizeof(char *));
    int *bits_padroes = malloc(qtd_padroes * sizeof(int));

    if (!padroes_codificados || !bits_padroes)
    {
        fprintf(stderr, "Erro de alocação de memória.\n");
        return 1;
    }

    for (int i = 0; i < qtd_padroes; i++)
    {
        padroes_codificados[i] = huffman_codificar_padrao(padroes[i], &bits_padroes[i]);
        if (!padroes_codificados[i])
        {
            fprintf(stderr, "Erro ao codificar o padrão: %s\n", padroes[i]);
            return 1;
        }
    }

    // Etapa 5: Buscar padrões em "texto_comp.bin" usando BMH adaptado para bits (ainda a implementar)
    char *bits_comprimidos = carregar_bits_do_binario("texto_comp.bin");
    if (!bits_comprimidos)
    {
        fprintf(stderr, "Erro ao carregar bits do arquivo comprimido.\n");
        return 1;
    }

    int **ocorrencias_comp = malloc(qtd_padroes * sizeof(int *));
    int *qtd_ocorrencias_comp = malloc(qtd_padroes * sizeof(int));
    int *comparacoes_comp = malloc(qtd_padroes * sizeof(int));

    clock_t ini2 = clock();
    for (int i = 0; i < qtd_padroes; i++)
    {
        ResultadoBMH res = bmh_binario(bits_comprimidos, padroes_codificados[i]);
        ocorrencias_comp[i] = res.ocorrencias;
        qtd_ocorrencias_comp[i] = res.quantidade;
        comparacoes_comp[i] = res.comparacoes;
    }
    clock_t fim2 = clock();
    double tempo_comp = (double)(fim2 - ini2) / CLOCKS_PER_SEC;

    // Etapa 6: Buscar padrões no texto original e medir desempenho
    char *texto_original = ler_arquivo_texto(caminho_texto);
    if (!texto_original)
    {
        fprintf(stderr, "Erro ao ler o texto original.\n");
        return 1;
    }

    int **ocorrencias_nao_comp = malloc(qtd_padroes * sizeof(int *));
    int *qtd_ocorrencias_nao_comp = malloc(qtd_padroes * sizeof(int));
    int *comparacoes_nao_comp = malloc(qtd_padroes * sizeof(int));

    clock_t ini = clock();
    for (int i = 0; i < qtd_padroes; i++)
    {
        ResultadoBMH resultado = bmh(texto_original, padroes[i]);
        ocorrencias_nao_comp[i] = resultado.ocorrencias;
        qtd_ocorrencias_nao_comp[i] = resultado.quantidade;
        comparacoes_nao_comp[i] = resultado.comparacoes;
    }
    clock_t fim = clock();
    double tempo_nao_comp = (double)(fim - ini) / CLOCKS_PER_SEC;

    // Imprimir
    escrever_resultado_comparativo(
        caminho_saida,
        padroes,
        qtd_padroes,
        ocorrencias_nao_comp,
        qtd_ocorrencias_nao_comp,
        comparacoes_nao_comp,
        tempo_nao_comp,
        ocorrencias_comp,
        qtd_ocorrencias_comp,
        comparacoes_comp,
        tempo_comp);

    printf("Compressão e codificação dos padrões realizadas com sucesso.\n");

    // Liberação de memória
    for (int i = 0; i < qtd_padroes; i++)
        free(padroes_codificados[i]);
    free(padroes_codificados);
    free(bits_padroes);
    liberar_padroes(padroes, qtd_padroes);
    free(texto_original);
    free(comparacoes_nao_comp);
    free(qtd_ocorrencias_nao_comp);
    for (int i = 0; i < qtd_padroes; i++)
        free(ocorrencias_nao_comp[i]);
    free(ocorrencias_nao_comp);
    free(bits_comprimidos);
    for (int i = 0; i < qtd_padroes; i++)
        free(ocorrencias_comp[i]);
    free(ocorrencias_comp);
    free(qtd_ocorrencias_comp);
    free(comparacoes_comp);

    return 0;
}
