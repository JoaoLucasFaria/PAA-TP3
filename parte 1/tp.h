#ifndef TP_H
#define TP_H

// Lê todo o conteúdo de um arquivo como string única
char *read_file(const char *filename);

// Lê todas as linhas de um arquivo, uma por linha
char **read_lines(const char *filename, int *num_lines);

// Libera a memória alocada por read_lines
void free_lines(char **lines, int num_lines);

#endif
