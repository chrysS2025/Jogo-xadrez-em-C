Resumo do Trabalho – Jogo de Xadrez em C

O trabalho tem como objetivo desenvolver, em linguagem C, um programa que simule um jogo de xadrez completo, desde a representação das peças e do tabuleiro até a implementação de uma inteligência artificial (IA) para jogar automaticamente com as peças pretas.

Parte 1 – Modelagem do Tabuleiro e das Peças

O tabuleiro será representado por uma matriz 8x8 de números inteiros.
Cada peça possui um código que identifica seu tipo e sua cor:

Peça	Branca	Preta
Peão	1	-1
Cavalo	2	-2
Bispo	3	-3
Torre	4	-4
Rainha	5	-5
Rei	6	-6

O tabuleiro segue a mesma disposição inicial de um jogo real de xadrez, considerando que a linha 0 representa o lado das peças brancas e a linha 7, o das pretas.

Cada jogada é representada pela estrutura:

struct jogada {
    int linhaDe, colunaDe, linhaPara, colunaPara;
};

Parte 2 – Representação da Posição das Peças

A posição do jogo é o estado atual das peças no tabuleiro.
Cada peça é representada pela estrutura:

struct Peca {
    int codigo;
    int linha, coluna;
    int ataques;
    struct Peca *prox, *ant;
};


E o conjunto de todas as peças é armazenado em uma estrutura de posição:

struct Posicao {
    int qtdBrancas;
    struct Peca *brancas;
    int qtdPretas;
    struct Peca *pretas;
    int jogVez;
    struct Peca *tab[8][8];
};


A lista de peças é circular, duplamente encadeada e com sentinela.

Cada casa do tabuleiro aponta para a peça correspondente ou para NULL, caso esteja vazia.

O campo jogVez indica o jogador atual (1 = brancas, -1 = pretas).

Parte 3 – Funções de Manipulação e Avaliação

Nessa etapa, devem ser implementadas três funções principais:

CopiaPosicao
Cria uma cópia completa de uma struct Posicao, incluindo listas de peças e matriz de ponteiros, sem alterar a posição original.

LiberaMemoria
Libera toda a memória alocada para uma posição, destruindo as listas de peças.

AvaliaPosicao
Retorna um valor double que representa a vantagem das pretas em relação às brancas.
Quanto maior o valor, melhor está a posição para as pretas.
A avaliação pode se basear no valor total das peças ou em outro critério definido pelo grupo.

Parte 4 – Implementação da Inteligência Artificial (IA)

A função principal da IA é ExecutaIA, que utiliza o algoritmo MiniMax com poda alfa-beta para determinar a melhor jogada das peças pretas.

Assinatura da função:

struct Jogada ExecutaIA(struct Posicao posAtual, int nivel, double alfa, double beta);


A função deve:

Calcular todas as jogadas possíveis das peças.

Copiar a posição atual e simular jogadas recursivamente.

Avaliar cada posição com a função AvaliaPosicao.

Utilizar poda alfa-beta para eliminar jogadas ruins e otimizar o processo.

Retornar a melhor jogada encontrada.

O programa também deve salvar cada jogada feita (do jogador ou da IA) em um arquivo binário chamado “jogadas.bin”, usando a função:

void SalvaJogada(char *nomeArquivo, struct Jogada *j);


Essa função grava as posições de origem e destino de cada jogada no arquivo, para posterior análise ou replay do jogo.

Conclusão

O projeto integra conceitos de estrutura de dados, recursividade, alocação dinâmica de memória e inteligência artificial.
Ao final, o programa será capaz de:

Simular um jogo completo de xadrez;

Avaliar posições automaticamente;

Tomar decisões estratégicas com base no algoritmo MiniMax.
