#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "bmh.h"
#include "tp.h"
#include "huffman.h"


double tempo_usuario()
{
    struct rusage uso;
    getrusage(RUSAGE_SELF, &uso);
    return uso.ru_utime.tv_sec + uso.ru_utime.tv_usec / 1e6;
}

void imprimir_memoria_utilizada()
{
    struct rusage uso;
    getrusage(RUSAGE_SELF, &uso);
    printf("Memória: %.2f MB\n", uso.ru_maxrss / 1024.0);
}

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

    double ini_usuario = tempo_usuario();

    if (huffman_comprimir_com_marcacao(caminho_texto, "texto_comp.bin") != 0)
    {
        fprintf(stderr, "Erro ao comprimir o texto.\n");
        return 1;
    }
    if (huffman_carregar_tabela() != 0)
    {
        fprintf(stderr, "Erro ao carregar a tabela de Huffman.\n");
        return 1;
    }

    int qtd_padroes = 0;
    char **padroes = ler_padroes(caminho_padroes, &qtd_padroes);
    if (!padroes)
    {
        fprintf(stderr, "Erro ao ler o arquivo de padrões.\n");
        return 1;
    }

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

    char *bits_comprimidos = carregar_bits_do_binario("texto_comp.bin");
    if (!bits_comprimidos)
    {
        fprintf(stderr, "Erro ao carregar bits do arquivo comprimido.\n");
        return 1;
    }

    int **ocorrencias_comp = malloc(qtd_padroes * sizeof(int *));
    int *qtd_ocorrencias_comp = malloc(qtd_padroes * sizeof(int));
    int *comparacoes_comp = malloc(qtd_padroes * sizeof(int));


    for (int i = 0; i < qtd_padroes; i++)
    {
        ResultadoBMH res = bmh_binario(bits_comprimidos, padroes_codificados[i]);
        ocorrencias_comp[i] = res.ocorrencias;
        qtd_ocorrencias_comp[i] = res.quantidade;
        comparacoes_comp[i] = res.comparacoes;
    }
    char *texto_original = ler_arquivo_texto(caminho_texto);
    if (!texto_original)
    {
        fprintf(stderr, "Erro ao ler o texto original.\n");
        return 1;
    }

    int **ocorrencias_nao_comp = malloc(qtd_padroes * sizeof(int *));
    int *qtd_ocorrencias_nao_comp = malloc(qtd_padroes * sizeof(int));
    int *comparacoes_nao_comp = malloc(qtd_padroes * sizeof(int));

    for (int i = 0; i < qtd_padroes; i++)
    {
        ResultadoBMH resultado = bmh(texto_original, padroes[i]);
        ocorrencias_nao_comp[i] = resultado.ocorrencias;
        qtd_ocorrencias_nao_comp[i] = resultado.quantidade;
        comparacoes_nao_comp[i] = resultado.comparacoes;
    }

    escrever_resultado_comparativo(
        caminho_saida,
        padroes,
        qtd_padroes,
        ocorrencias_nao_comp,
        qtd_ocorrencias_nao_comp,
        comparacoes_nao_comp,
        ocorrencias_comp,
        qtd_ocorrencias_comp,
        comparacoes_comp);

    printf("Compressão e codificação dos padrões realizadas com sucesso.\n");

    double fim_usuario = tempo_usuario();

    imprimir_memoria_utilizada();
    printf("Tempo de usuário: %.6f segundos\n", fim_usuario - ini_usuario);

    // Libera padrões codificados
    for (int i = 0; i < qtd_padroes; i++)
        free(padroes_codificados[i]);
    free(padroes_codificados);
    free(bits_padroes);

    // Libera padrões originais
    liberar_padroes(padroes, qtd_padroes);

    // Libera texto original
    free(texto_original);

    // Libera dados de busca não comprimida
    free(comparacoes_nao_comp);
    free(qtd_ocorrencias_nao_comp);
    for (int i = 0; i < qtd_padroes; i++)
        free(ocorrencias_nao_comp[i]);
    free(ocorrencias_nao_comp);

    // Libera bits comprimidos e dados de busca comprimida
    free(bits_comprimidos);
    for (int i = 0; i < qtd_padroes; i++)
        free(ocorrencias_comp[i]);
    free(ocorrencias_comp);
    free(qtd_ocorrencias_comp);
    free(comparacoes_comp);

    return 0;
}
