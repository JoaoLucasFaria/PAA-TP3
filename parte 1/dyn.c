#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dyn.h"

// Retorna o mínimo entre 3 inteiros
int min3(int a, int b, int c)
{
    if (a < b && a < c)
        return a;
    else if (b < c)
        return b;
    else
        return c;
}

// Realiza o casamento permitindo erros
void busca_aproximada_dp(const char *texto, const char *padrao, int k)
{
    int n = strlen(texto);
    int m = strlen(padrao);

    int **dp = (int **)malloc((m + 1) * sizeof(int *));
    for (int i = 0; i <= m; i++)
        dp[i] = (int *)malloc((n + 1) * sizeof(int));

    for (int i = 0; i <= m; i++)
        dp[i][0] = i;
    for (int j = 0; j <= n; j++)
        dp[0][j] = 0;

    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            int custo = (padrao[i - 1] != texto[j - 1]);
            dp[i][j] = min3(
                dp[i - 1][j] + 1,        // deleção
                dp[i][j - 1] + 1,        // inserção
                dp[i - 1][j - 1] + custo // substituição
            );
        }
    }

    printf("%s", padrao);
    for (int j = 1; j <= n; j++)
    {
        if (dp[m][j] <= k && j >= m)
        {
            printf(" %d", j - m + 1);
        }
    }

    printf("\n");

    for (int i = 0; i <= m; i++)
        free(dp[i]);
    free(dp);
}
