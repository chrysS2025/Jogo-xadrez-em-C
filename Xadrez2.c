#include "stdio.h"
#include "stdlib.h"
#include "math.h" // Para o uso de abs()

// Definição de números para as peças
#define PEAO    1
#define CAVALO  2
#define BISPO   3
#define TORRE   4
#define RAINHA  5
#define REI     6

typedef struct Jogada {
    int deLinha, deColuna, paraLinha, paraColuna;
    struct Jogada *prox, *ant;
} Jogada;

void ResetCor(){
    printf("\033[0m");
}

void AjustaCor(int peca){
    switch(abs(peca)){
    case PEAO: printf("\033[0;36m"); break;
    case CAVALO: printf("\033[1;33m"); break;
    case BISPO: printf("\033[0;35m"); break;
    case TORRE: printf("\033[0;32m"); break;
    case RAINHA: printf("\033[0;34m"); break;
    case REI: printf("\033[1;31m"); break;
    }
}

struct Peca{
    int codigo;
    int linha, coluna;
    int ataques;
    struct Peca *prox, *ant;
};

struct Posicao{
    int qtdBrancos;
    struct Peca *brancas;
    int qtdPretas;
    struct Peca *pretas;
    int jogVez;
    struct Peca *tab[8][8];
};

struct Peca *CriaPeca(int codigo, int linha, int coluna){
    struct Peca *auxPeca = (struct Peca*) malloc(sizeof(struct Peca));
    auxPeca->codigo = codigo;
    auxPeca->linha  = linha;
    auxPeca->coluna = coluna;
    auxPeca->ataques = 0;
    return auxPeca;
}

struct Peca *CriaLista(){
    struct Peca *sentinela = (struct Peca*) malloc(sizeof(struct Peca));
    sentinela->prox = sentinela;
    sentinela->ant = sentinela;
    return sentinela;
}

void InserePecas(struct Peca *lista, struct Peca *auxPeca){
    struct Peca *novo = auxPeca;
    novo->prox = lista->prox;
    novo->ant = lista;
    lista->prox->ant = novo;
    lista->prox = novo;
}

struct Jogada *CriaJogada(int deLinha, int deColuna, int paraLinha, int paraColuna){
    struct Jogada *sentinela = (struct Jogada*) malloc(sizeof(struct Jogada));
    sentinela->deLinha = deLinha;
    sentinela->deColuna = deColuna;
    sentinela->paraLinha = paraLinha;
    sentinela->paraColuna = paraColuna;
    sentinela->prox = sentinela;
    sentinela->ant = sentinela;
    return sentinela;
}

struct Jogada *CriaListaJogada(){
    struct Jogada *listaJogada = (struct Jogada*) malloc(sizeof(struct Jogada));
    listaJogada->prox = listaJogada;
    listaJogada->ant = listaJogada;
    return listaJogada;
}

void DesenhaTabuleiro(struct Posicao posAtual){
    int i, j;
    system("clear");

    printf("    +---+---+---+---+---+---+---+---+\n");
    for (i=7;i>=0;i--){
        printf("%d   |",i+1);
        for (j=0;j<8;j++){
            if(posAtual.tab[i][j]!=NULL){
              AjustaCor(posAtual.tab[i][j]->codigo);
              if (posAtual.tab[i][j]->codigo>=0)
                printf(" %d",posAtual.tab[i][j]->codigo);
              else 
                printf("%d",posAtual.tab[i][j]->codigo);
            }else 
                printf(" 0");
            ResetCor();
            printf(" |");
        }
        printf("\n    +---+---+---+---+---+---+---+---+\n");
    }
    printf("      1   2   3   4   5   6   7   8 \n");
}

// Funções de cálculo de movimentos
int CalculaPeao(struct Posicao PosAtual, int linha, int coluna, struct Jogada *listaJogada){
    int qtd = 0, peca = PosAtual.tab[linha][coluna]->codigo;

    // Verificar avanço do peão
    if (linha + peca >= 0 && linha + peca <= 7) {  // Verifica se está dentro do tabuleiro
        if (PosAtual.tab[linha+peca][coluna] == NULL) { // Pode avançar
            listaJogada = CriaJogada(linha, coluna, linha+peca, coluna);
            qtd++;
        }
        if (coluna > 0 && PosAtual.tab[linha+peca][coluna-1]->codigo * peca < 0) { // Captura à esquerda
            listaJogada = CriaJogada(linha, coluna, linha+peca, coluna-1);
            qtd++;
            PosAtual.tab[linha+peca][coluna-1]->ataques += 1;
        }
        if (coluna < 7 && PosAtual.tab[linha+peca][coluna+1]->codigo * peca < 0) { // Captura à direita
            listaJogada = CriaJogada(linha, coluna, linha+peca, coluna+1);
            qtd++;
            PosAtual.tab[linha+peca][coluna+1]->ataques += 1;
        }
    }
    return qtd;
}

int CalculaCavalo(struct Posicao PosAtual, int linha, int coluna, struct Jogada *listaJogada) {
    int qtd = 0, peca = PosAtual.tab[linha][coluna]->codigo;

    // Verificar todos os movimentos possíveis para o cavalo
    int movimentos[8][2] = {
        {-2, -1}, {-2, 1}, {2, -1}, {2, 1},
        {-1, -2}, {1, -2}, {-1, 2}, {1, 2}
    };
    
    for (int i = 0; i < 8; i++) {
        int novaLinha = linha + movimentos[i][0];
        int novaColuna = coluna + movimentos[i][1];
        
        if (novaLinha >= 0 && novaLinha < 8 && novaColuna >= 0 && novaColuna < 8) {
            if (PosAtual.tab[novaLinha][novaColuna] == NULL || PosAtual.tab[novaLinha][novaColuna]->codigo * peca < 0) {
                listaJogada = CriaJogada(linha, coluna, novaLinha, novaColuna);
                qtd++;
            }
        }
    }
    
    return qtd;
}

struct Posicao IniciaTabuleiro(){
    struct Posicao auxPos;
    auxPos.qtdBrancos = 16;
    auxPos.qtdPretas = 16;
    auxPos.jogVez = 1;
    struct Peca *auxPeca;
    auxPos.brancas = CriaLista();
    auxPos.pretas = CriaLista();

    // Inicializando as peças no tabuleiro
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            auxPos.tab[i][j] = NULL;
        }
  
      }
}      

