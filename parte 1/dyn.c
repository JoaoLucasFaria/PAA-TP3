#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dyn.h"

int min3(int a, int b, int c)
{
    if (a < b && a < c)
        return a;
    else if (b < c)
        return b;
    else
        return c;
}

void busca_aproximada_dp(const char *texto, const char *padrao, int k)
{
    int n = strlen(texto);
    int m = strlen(padrao);

    int **dp = (int **)malloc((m + 1) * sizeof(int *));
    for (int i = 0; i <= m; i++)
        dp[i] = (int *)malloc((n + 1) * sizeof(int));

    // Inicialização
    for (int i = 0; i <= m; i++)
        dp[i][0] = i; // deletar todos os caracteres
    for (int j = 0; j <= n; j++)
        dp[0][j] = 0; // inserções gratuitas (busca parcial)

    // Preenchimento da matriz
    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            int custo = (padrao[i - 1] != texto[j - 1]);
            dp[i][j] = min3(
                dp[i - 1][j] + 1,        // deleção
                dp[i][j - 1] + 1,        // inserção
                dp[i - 1][j - 1] + custo // substituição/match
            );
        }
    }

    // Verifica todas as colunas da última linha
    printf("%s", padrao);
    for (int j = 1; j <= n; j++)
    {
        if (dp[m][j] <= k && j >= m)
        {
            printf(" %d", j - m + 1);
            // Debug opcional:
            // printf(" (trecho='%.*s', dist=%d)", m, &texto[j - m], dp[m][j]);
        }
    }

    printf("\n");

    for (int i = 0; i <= m; i++)
        free(dp[i]);
    free(dp);
}
