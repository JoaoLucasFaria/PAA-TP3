#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

#define BYTE_SIZE 8

typedef struct HuffmanNode
{
    unsigned char data;
    unsigned freq;
    struct HuffmanNode *left, *right;
} HuffmanNode;

typedef struct MinHeap
{
    unsigned size;
    unsigned capacity;
    HuffmanNode **array;
} MinHeap;

static HuffmanNode *huffman_raiz = NULL;
static char *tabela_codigos[256] = {NULL};

static HuffmanNode *novo_no(unsigned char data, unsigned freq)
{
    HuffmanNode *node = malloc(sizeof(HuffmanNode));
    node->data = data;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

static MinHeap *criar_fila_min(unsigned capacidade)
{
    MinHeap *heap = malloc(sizeof(MinHeap));
    heap->size = 0;
    heap->capacity = capacidade;
    heap->array = malloc(capacidade * sizeof(HuffmanNode *));
    return heap;
}

static void trocar_nos(HuffmanNode **a, HuffmanNode **b)
{
    HuffmanNode *t = *a;
    *a = *b;
    *b = t;
}

static void heapify(MinHeap *heap, int i)
{
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;
    if (esq < heap->size && heap->array[esq]->freq < heap->array[menor]->freq)
        menor = esq;
    if (dir < heap->size && heap->array[dir]->freq < heap->array[menor]->freq)
        menor = dir;
    if (menor != i)
    {
        trocar_nos(&heap->array[i], &heap->array[menor]);
        heapify(heap, menor);
    }
}

static void inserir_heap(MinHeap *heap, HuffmanNode *node)
{
    int i = heap->size++;
    while (i && node->freq < heap->array[(i - 1) / 2]->freq)
    {
        heap->array[i] = heap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->array[i] = node;
}

static MinHeap *construir_heap(unsigned freq[])
{
    MinHeap *heap = criar_fila_min(256);
    for (int i = 0; i < 256; i++)
        if (freq[i])
            heap->array[heap->size++] = novo_no(i, freq[i]);
    for (int i = (heap->size - 2) / 2; i >= 0; i--)
        heapify(heap, i);
    return heap;
}

static int eh_folha(HuffmanNode *no)
{
    return !no->left && !no->right;
}

static HuffmanNode *construir_arvore(unsigned freq[])
{
    MinHeap *heap = construir_heap(freq);
    while (heap->size > 1)
    {
        HuffmanNode *esq = heap->array[0];
        heap->array[0] = heap->array[--heap->size];
        heapify(heap, 0);
        HuffmanNode *dir = heap->array[0];
        heap->array[0] = heap->array[--heap->size];
        heapify(heap, 0);
        HuffmanNode *novo = novo_no('$', esq->freq + dir->freq);
        novo->left = esq;
        novo->right = dir;
        inserir_heap(heap, novo);
    }
    return heap->array[0];
}

static void gerar_codigos(HuffmanNode *no, char *codigo, int altura)
{
    if (no->left)
    {
        codigo[altura] = '0';
        gerar_codigos(no->left, codigo, altura + 1);
    }
    if (no->right)
    {
        codigo[altura] = '1';
        gerar_codigos(no->right, codigo, altura + 1);
    }
    if (eh_folha(no))
    {
        codigo[altura] = '\0';
        tabela_codigos[no->data] = strdup(codigo);
    }
}

static void escrever_arvore(FILE *out, HuffmanNode *no)
{
    if (eh_folha(no))
    {
        fputc('1', out);
        fputc(no->data, out);
    }
    else
    {
        fputc('0', out);
        escrever_arvore(out, no->left);
        escrever_arvore(out, no->right);
    }
}

int huffman_comprimir_com_marcacao(const char *entrada, const char *saida)
{

    // Limpa a tabela de códigos anterior
    for (int i = 0; i < 256; i++)
    {
        if (tabela_codigos[i])
        {
            free(tabela_codigos[i]);
            tabela_codigos[i] = NULL;
        }
    }

    FILE *in = fopen(entrada, "rb");
    if (!in)
        return 1;

    unsigned freq[256] = {0};
    int c;
    while ((c = fgetc(in)) != EOF)
        freq[(unsigned char)c]++;

    huffman_raiz = construir_arvore(freq);
    char codigo[256];
    gerar_codigos(huffman_raiz, codigo, 0);

    rewind(in);
    FILE *out = fopen(saida, "wb");
    if (!out)
    {
        fclose(in);
        return 1;
    }

    escrever_arvore(out, huffman_raiz);
    fputc('\0', out);

    unsigned char buffer = 0;
    int bits = 0;
    while ((c = fgetc(in)) != EOF)
    {
        char *code = tabela_codigos[(unsigned char)c];
        for (int i = 0; code[i]; i++)
        {
            buffer <<= 1;
            if (code[i] == '1')
                buffer |= 1;
            bits++;
            if (bits == BYTE_SIZE)
            {
                fputc(buffer, out);
                bits = 0;
                buffer = 0;
            }
        }
    }
    if (bits > 0)
    {
        buffer <<= (BYTE_SIZE - bits);
        fputc(buffer, out);
    }

    fclose(in);
    fclose(out);
    return 0;
}

int huffman_carregar_tabela(void)
{
    if (!huffman_raiz)
        return 1;

    int tabela_vazia = 1;
    for (int i = 0; i < 256; i++)
    {
        if (tabela_codigos[i])
        {
            tabela_vazia = 0;
            break;
        }
    }

    if (tabela_vazia)
    {
        char codigo[256];
        gerar_codigos(huffman_raiz, codigo, 0);
    }

    return 0;
}

char *huffman_codificar_padrao(const char *padrao, int *bits_totais)
{
    *bits_totais = 0;
    int capacidade = 1024;
    char *resultado = malloc(capacidade);
    resultado[0] = '\0';

    for (int i = 0; padrao[i]; i++)
    {
        char *cod = tabela_codigos[(unsigned char)padrao[i]];
        if (!cod)
            return NULL;
        int len = strlen(cod);
        *bits_totais += len;
        if (strlen(resultado) + len + 1 >= capacidade)
        {
            capacidade *= 2;
            resultado = realloc(resultado, capacidade);
        }
        strcat(resultado, cod);
    }

    return resultado;
}
