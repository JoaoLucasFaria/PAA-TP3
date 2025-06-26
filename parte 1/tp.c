#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tp.h"

char *read_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir arquivo de texto");
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *buffer = malloc(size + 1);
    if (!buffer) {
        perror("Erro de alocação");
        exit(1);
    }

    fread(buffer, 1, size, fp);
    buffer[size] = '\0';
    fclose(fp);
    return buffer;
}

char **read_lines(const char *filename, int *num_lines) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir arquivo de padrões");
        exit(1);
    }

    char **lines = NULL;
    char buffer[256];
    *num_lines = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\r\n")] = '\0';

        lines = realloc(lines, (*num_lines + 1) * sizeof(char *));
        lines[*num_lines] = strdup(buffer);
        (*num_lines)++;
    }

    fclose(fp);
    return lines;
}

void free_lines(char **lines, int num_lines) {
    for (int i = 0; i < num_lines; i++) {
        free(lines[i]);
    }
    free(lines);
}
