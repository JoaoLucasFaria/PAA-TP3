#ifndef BMH_H
#define BMH_H

#include <stddef.h>

typedef struct
{
    int *ocorrencias;
    int quantidade;
    int comparacoes;
} ResultadoBMH;

ResultadoBMH bmh(const char *texto, const char *padrao);
ResultadoBMH bmh_binario(const char *texto_bits, const char *padrao_bits);


#endif
