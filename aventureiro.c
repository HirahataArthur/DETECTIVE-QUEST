#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura de uma sala da mansão
typedef struct Sala {
    char nome[50];
    char pista[100]; // Pista opcional
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura de um nó da árvore BST de pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Função que cria dinamicamente uma sala com ou sem pista
Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória para a sala.\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função que insere uma pista na BST
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        if (novo == NULL) {
            printf("Erro ao alocar memória para a pista.\n");
            exit(1);
        }
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    return raiz;
}

// Função que exibe as pistas em ordem alfabética (in-order traversal)
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// Função que permite explorar a mansão e coletar pistas
void explorarSalasComPistas(Sala *atual, PistaNode **raizPistas) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            *raizPistas = inserirPista(*raizPistas, atual->pista);
        } else {
            printf("Nenhuma pista nesta sala.\n");
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

// Função principal que monta o mapa e inicia a exploração
int main() {
    // Criação das salas com pistas
    Sala *hall = criarSala("Hall de Entrada", "Pegadas misteriosas");
    Sala *salaEstar = criarSala("Sala de Estar", "Copo quebrado");
    Sala *cozinha = criarSala("Cozinha", "");
    Sala *biblioteca = criarSala("Biblioteca", "Livro rasgado");
    Sala *jardim = criarSala("Jardim", "Luvas sujas");
    Sala *porao = criarSala("Porão", "Lanterna caída");

    // Montagem da árvore binária da mansão
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = porao;

    // Inicializa a árvore de pistas
    PistaNode *raizPistas = NULL;

    // Início da exploração
    printf("🔍 Bem-vindo ao Detective Quest!\nExplore a mansão e colete pistas...\n");
    explorarSalasComPistas(hall, &raizPistas);

    // Exibe todas as pistas coletadas
    printf("\n📜 Pistas coletadas (em ordem alfabética):\n");
    exibirPistas(raizPistas);

    // Liberação de memória (opcional para este exemplo)
    free(hall); free(salaEstar); free(cozinha);
    free(biblioteca); free(jardim); free(porao);

    return 0;
}
