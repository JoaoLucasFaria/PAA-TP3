#ifndef TP_H
#define TP_H

char *ler_arquivo_texto(const char *caminho);
char **ler_padroes(const char *caminho, int *quantidade);
void liberar_padroes(char **padroes, int quantidade);
void escrever_resultado_comparativo(
    const char *caminho_saida,
    char **padroes,
    int qtd_padroes,
    int **ocorrencias_nao_comp,
    int *qtd_ocorrencias_nao_comp,
    int *comparacoes_nao_comp,
    int **ocorrencias_comp,
    int *qtd_ocorrencias_comp,
    int *comparacoes_comp);

char *carregar_bits_do_binario(const char *caminho);


#endif
