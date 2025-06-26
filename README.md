# TP3

Trabalho pratico 3 - Projeto e Análise de Algorítmos (2025.1)

Os comandos para as execuções são: 
Para a parte 1: ./tp3_parte1 <algoritmo (1=DP, 2=Shift-And)> <arquivo_texto> <arquivo_padroes> <arquivo_saida> <k>
Para a parte 2: ./tp3_parte2 <arquivo_texto> <arquivo_padroes> <arquivo_saida>

O programa irá imprimir no arquivo de saída escolhido os seguintes:
Parte 1: As ocorrências dos padrões (arquivo_padroes) no arquivo de texto (arquivo_texto)
Parte 2: As ocorrências dos padrões (arquivo_padroes) no arquivo de texto (arquivo_texto), junto ao número de comparações feitas. Em ambas as formas comprimidas e originais.

Exemplo de entrada:
arquivo_texto: 
Texto exemplo, texto tem palavras, palavras exercem fascínio.
arquivo_padroes:
palavras
exemplo

Exemplo de Saída:
Parte 1:

palavras 26 36
exemplo 7

Parte 2:
==== Desempenho BMH em texto NÃO comprimido ====
palavras 26 36
(23 comparações)

exemplo 7
(17 comparações)

==== Desempenho BMH em arquivo COMPRIMIDO ====
palavras 96 135
(218 comparações)

exemplo 24
(383 comparações)

