#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "shift_and.h"

#define WORD_SIZE 64

void shift_left(uint64_t *dest, const uint64_t *src, int blocks) {
    uint64_t carry = 0;
    for (int i = 0; i < blocks; i++) {
        uint64_t next_carry = src[i] >> (WORD_SIZE - 1);
        dest[i] = (src[i] << 1) | carry;
        carry = next_carry;
    }
}

int bit_is_zero(const uint64_t *bitset, int bit_pos) {
    int block = bit_pos / WORD_SIZE;
    int bit = bit_pos % WORD_SIZE;
    return !(bitset[block] & (1ULL << bit));
}

void copy_bits(uint64_t *dest, const uint64_t *src, int blocks) {
    for (int i = 0; i < blocks; i++)
        dest[i] = src[i];
}

void or_bits(uint64_t *dest, const uint64_t *src, int blocks) {
    for (int i = 0; i < blocks; i++)
        dest[i] |= src[i];
}

void and_bits(uint64_t *dest, const uint64_t *src, int blocks) {
    for (int i = 0; i < blocks; i++)
        dest[i] &= src[i];
}

void set_all_ones(uint64_t *vec, int blocks) {
    for (int i = 0; i < blocks; i++)
        vec[i] = ~0ULL;
}

void busca_aproximada_shiftand(const char *texto, const char *padrao, int k) {
    int n = strlen(texto);
    int m = strlen(padrao);
    int blocks = (m + WORD_SIZE - 1) / WORD_SIZE;

    // Construção da máscara
    uint64_t **mask = malloc(256 * sizeof(uint64_t *));
    for (int i = 0; i < 256; i++) {
        mask[i] = malloc(blocks * sizeof(uint64_t));
        set_all_ones(mask[i], blocks);
    }

    for (int i = 0; i < m; i++) {
        int block = i / WORD_SIZE;
        int bit = i % WORD_SIZE;
        mask[(unsigned char)padrao[i]][block] &= ~(1ULL << bit);
    }

    printf("%s", padrao);

    uint64_t *shl = malloc(blocks * sizeof(uint64_t));

    // Caso especial para k == 0 (busca exata)
    if (k == 0) {
        uint64_t *R = malloc(blocks * sizeof(uint64_t));
        set_all_ones(R, blocks);

        for (int j = 0; j < n; j++) {
            shift_left(shl, R, blocks);
            for (int b = 0; b < blocks; b++) {
                if (b == 0)
                    R[b] = (shl[b] | 1ULL) & mask[(unsigned char)texto[j]][b];
                else
                    R[b] = shl[b] & mask[(unsigned char)texto[j]][b];
            }

            if (bit_is_zero(R, m - 1) && j >= m - 1)
                printf(" %d", j - m + 1);
        }

        free(R);
        free(shl);
        for (int i = 0; i < 256; i++) free(mask[i]);
        free(mask);
        printf("\n");
        return;
    }

    // Caso geral k > 0
    uint64_t **R = malloc((k + 1) * sizeof(uint64_t *));
    for (int i = 0; i <= k; i++) {
        R[i] = malloc(blocks * sizeof(uint64_t));
        set_all_ones(R[i], blocks);
    }

    uint64_t *old = malloc(blocks * sizeof(uint64_t));
    uint64_t *tmp = malloc(blocks * sizeof(uint64_t));

    for (int j = 0; j < n; j++) {
        copy_bits(old, R[0], blocks);
        shift_left(shl, R[0], blocks);
        for (int b = 0; b < blocks; b++) {
            if (b == 0)
                R[0][b] = (shl[b] | 1ULL) & mask[(unsigned char)texto[j]][b];
            else
                R[0][b] = shl[b] & mask[(unsigned char)texto[j]][b];
        }

        for (int e = 1; e <= k; e++) {
            copy_bits(tmp, R[e], blocks);
            shift_left(shl, R[e], blocks);
            for (int b = 0; b < blocks; b++) {
                if (b == 0)
                    R[e][b] = (shl[b] | 1ULL) & mask[(unsigned char)texto[j]][b];
                else
                    R[e][b] = shl[b] & mask[(unsigned char)texto[j]][b];
            }

            shift_left(shl, old, blocks);
            or_bits(R[e], shl, blocks);
            or_bits(R[e], old, blocks);
            or_bits(R[e], R[e - 1], blocks);
            copy_bits(old, tmp, blocks);
        }

        for (int e = 0; e <= k; e++) {
            if (bit_is_zero(R[e], m - 1) && j >= m - 1) {
                printf(" %d", j - m + 1);
                break;
            }
        }
    }

    printf("\n");

    for (int i = 0; i <= k; i++) free(R[i]);
    free(R);
    for (int i = 0; i < 256; i++) free(mask[i]);
    free(mask);
    free(old);
    free(tmp);
    free(shl);
}
