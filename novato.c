#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma sala da mansão
typedef struct Sala {
    char nome[50];           // Nome da sala
    struct Sala *esquerda;   // Caminho à esquerda
    struct Sala *direita;    // Caminho à direita
} Sala;

// Função que cria uma sala com nome e retorna ponteiro para ela
Sala* criarSala(const char *nome) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função que permite o jogador explorar a mansão
void explorarSalas(Sala *atual) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Verifica se é um nó-folha (sem caminhos)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho. Você chegou a um cômodo sem saídas.\n");
            break;
        }

        printf("Escolha um caminho:\n");
        if (atual->esquerda != NULL) printf("e - Ir para a esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita != NULL) printf("d - Ir para a direita (%s)\n", atual->direita->nome);
        printf("s - Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else if (escolha == 's') {
            printf("Exploração encerrada.\n");
            break;
        } else {
            printf("Opção inválida ou caminho inexistente. Tente novamente.\n");
        }
    }
}

// Função principal que monta o mapa da mansão e inicia a exploração
int main() {
    // Criação das salas
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim");
    Sala *porao = criarSala("Porão");

    // Montagem da árvore binária (mapa da mansão)
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = porao;

    // Início da exploração
    printf("Bem-vindo ao Detective Quest!\nExplore a mansão e descubra pistas...\n");
    explorarSalas(hall);

    // Liberação de memória
    free(hall);
    free(salaEstar);
    free(cozinha);
    free(biblioteca);
    free(jardim);
    free(porao);

    return 0;
}

