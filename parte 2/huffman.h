#ifndef HUFFMAN_H
#define HUFFMAN_H

// Comprime um arquivo de texto e salva em binário com marcação da árvore
int huffman_comprimir_com_marcacao(const char *arquivo_entrada, const char *arquivo_saida);

// Carrega a árvore de Huffman usada para compressão (pode ser estendida para salvar e reutilizar a árvore)
int huffman_carregar_tabela(void);

// Codifica um padrão (string) usando a árvore de Huffman carregada
char *huffman_codificar_padrao(const char *padrao, int *bits_totais);

#endif
