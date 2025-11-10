#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_HASH 10

// Estrutura da sala da mansão
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Nó da árvore BST de pistas
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Elemento da tabela hash
typedef struct HashItem {
    char pista[100];
    char suspeito[50];
    struct HashItem *prox;
} HashItem;

// Tabela hash
HashItem* tabelaHash[TAMANHO_HASH];

// Função hash simples baseada na soma dos caracteres
int hash(const char *str) {
    int soma = 0;
    for (int i = 0; str[i] != '\0'; i++)
        soma += str[i];
    return soma % TAMANHO_HASH;
}

// criaSala: cria dinamicamente uma sala com nome e pista
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// inserirPista: insere uma pista na BST
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

// inserirNaHash: associa pista a suspeito na tabela hash
void inserirNaHash(const char *pista, const char *suspeito) {
    int indice = hash(pista);
    HashItem *novo = (HashItem*) malloc(sizeof(HashItem));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[indice];
    tabelaHash[indice] = novo;
}

// encontrarSuspeito: retorna o suspeito associado à pista
char* encontrarSuspeito(const char *pista) {
    int indice = hash(pista);
    HashItem *atual = tabelaHash[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

// exibirPistas: imprime pistas coletadas em ordem alfabética
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// contarPistasDoSuspeito: conta quantas pistas apontam para o suspeito
int contarPistasDoSuspeito(PistaNode *raiz, const char *suspeito) {
    if (raiz == NULL) return 0;
    int cont = 0;
    char *s = encontrarSuspeito(raiz->pista);
    if (s != NULL && strcmp(s, suspeito) == 0)
        cont = 1;
    return cont + contarPistasDoSuspeito(raiz->esquerda, suspeito) + contarPistasDoSuspeito(raiz->direita, suspeito);
}

// explorarSalas: navega pela árvore e coleta pistas
void explorarSalas(Sala *atual, PistaNode **raizPistas) {
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
        if (atual->esquerda) printf("e - Ir para a esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita) printf("d - Ir para a direita (%s)\n", atual->direita->nome);
        printf("s - Sair da exploração\nOpção: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda) atual = atual->esquerda;
        else if (escolha == 'd' && atual->direita) atual = atual->direita;
        else if (escolha == 's') break;
        else printf("Opção inválida.\n");
    }
}

// verificarSuspeitoFinal: conduz à fase de julgamento final
void verificarSuspeitoFinal(PistaNode *raizPistas) {
    char acusacao[50];
    printf("\nDigite o nome do suspeito que deseja acusar: ");
    scanf(" %[^\n]", acusacao);

    int cont = contarPistasDoSuspeito(raizPistas, acusacao);
    printf("\n🔍 Pistas que apontam para %s: %d\n", acusacao, cont);
    if (cont >= 2)
        printf("✅ Acusação válida! %s é o culpado.\n", acusacao);
    else
        printf("❌ Acusação fraca. Poucas evidências contra %s.\n", acusacao);
}

// Programa principal
int main() {
    // Montagem do mapa da mansão
    Sala *hall = criarSala("Hall de Entrada", "Pegadas misteriosas");
    Sala *salaEstar = criarSala("Sala de Estar", "Copo quebrado");
    Sala *cozinha = criarSala("Cozinha", "");
    Sala *biblioteca = criarSala("Biblioteca", "Livro rasgado");
    Sala *jardim = criarSala("Jardim", "Luvas sujas");
    Sala *porao = criarSala("Porão", "Lanterna caída");

    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    cozinha->direita = porao;

    // Associação pista → suspeito
    inserirNaHash("Pegadas misteriosas", "Carlos");
    inserirNaHash("Copo quebrado", "Ana");
    inserirNaHash("Livro rasgado", "Carlos");
    inserirNaHash("Luvas sujas", "Bruno");
    inserirNaHash("Lanterna caída", "Carlos");

    // Início da exploração
    PistaNode *raizPistas = NULL;
    printf("🔎 Bem-vindo ao capítulo final de Detective Quest!\nExplore a mansão e colete pistas...\n");
    explorarSalas(hall, &raizPistas);

    // Exibição das pistas
    printf("\n📜 Pistas coletadas:\n");
    exibirPistas(raizPistas);

    // Julgamento final
    verificarSuspeitoFinal(raizPistas);

    return 0;
}
