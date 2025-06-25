#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shift_and.h"

#define MAX_PATTERN_LEN 64
#define MAX_TEXT_LEN 10000

static unsigned long long masks[256];

void shift_and_approximate(const char *text, const char *pattern, int k)
{
    int m = strlen(pattern);
    int n = strlen(text);

    if (m > MAX_PATTERN_LEN)
    {
        printf("Padrão muito longo! (max %d caracteres)\n", MAX_PATTERN_LEN);
        return;
    }

    // Inicializa as máscaras
    for (int i = 0; i < 256; i++)
        masks[i] = ~0ULL;

    for (int i = 0; i < m; i++)
        masks[(unsigned char)pattern[i]] &= ~(1ULL << i);

    unsigned long long R_antigo[k + 1], R_novo[k + 1];
    for (int i = 0; i <= k; i++)
        R_antigo[i] = ~0ULL;

    for (int i = 0; i < n; i++)
    {
        R_novo[0] = ((R_antigo[0] << 1) | 1ULL) & masks[(unsigned char)text[i]];

        for (int d = 1; d <= k; d++)
        {
            R_novo[d] = ((R_antigo[d] << 1) | 1ULL) & masks[(unsigned char)text[i]];
            R_novo[d] |= (R_antigo[d - 1] << 1) | 1ULL; // substituição
            R_novo[d] |= R_antigo[d - 1];               // inserção
            R_novo[d] |= R_novo[d - 1];                 // deleção
        }

        for (int d = 0; d <= k; d++)
        {
            int pos = i - m + 1;
            if (pos >= 0)
            {
                if (d == 0)
                {
                    if (strncmp(&text[pos], pattern, m) == 0)
                        printf("%d ", pos + 1);
                }
                else
                {
                    for (int bit = 0; bit < m; bit++)
                    {
                        if ((R_novo[d] & (1ULL << bit)) == 0)
                        {
                            printf("%d ", pos + 1);
                            break;
                        }
                    }
                }
            }
        }

        for (int d = 0; d <= k; d++)
            R_antigo[d] = R_novo[d];
    }
}
