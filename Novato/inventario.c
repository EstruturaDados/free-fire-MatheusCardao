#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Para rodar gcc inventario.c -o invetario 
// --- Definição da Struct ---
/* * Struct 'Item' para representar um objeto no inventário.
 * Campos:
 * - nome: Nome do item (máx. 29 caracteres + '\0').
 * - tipo: Categoria do item (ex: arma, munição, cura).
 * - quantidade: Número de unidades do item.
 */
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// --- Variáveis Globais para o Inventário ---
#define MAX_ITENS 10 // Capacidade máxima da mochila
Item inventario[MAX_ITENS]; // Vetor de structs que representa a mochila
int totalItens = 0; // Contador de itens atualmente na mochila

// --- Protótipos das Funções ---
void exibirMenu();
void inserirItem();
void removerItem();
void listarItens();
void buscarItem();
int buscarIndiceItem(const char *nomeBusca); // Função auxiliar para busca

// ----------------------------------------------------------------------
// --- Implementação das Funções do Inventário ---
// ----------------------------------------------------------------------

/**
 * @brief Adiciona um novo item ao inventário (mochila).
 * Requisito: Cadastro de itens.
 * Limite: MAX_ITENS (10).
 */
void inserirItem() {
    if (totalItens >= MAX_ITENS) {
        printf("\n[ATENCAO] Mochila Cheia! Nao e possivel carregar mais itens (%d/%d).\n", totalItens, MAX_ITENS);
        return;
    }

    printf("\n--- CADASTRAR NOVO ITEM ---\n");
    Item novoItem;

    // Leitura do Nome
    printf("Nome do Item: ");
    // Uso de scanf para strings simples (sem espaços)
    if (scanf("%29s", novoItem.nome) != 1) {
        printf("[ERRO] Falha na leitura do nome.\n");
        return;
    }

    // Leitura do Tipo
    printf("Tipo do Item (ex: arma, municao, cura, ferramenta): ");
    if (scanf("%19s", novoItem.tipo) != 1) {
        printf("[ERRO] Falha na leitura do tipo.\n");
        return;
    }

    // Leitura da Quantidade
    printf("Quantidade: ");
    // Loop para garantir que a quantidade seja um número positivo
    while (scanf("%d", &novoItem.quantidade) != 1 || novoItem.quantidade <= 0) {
        printf("[ERRO] Quantidade invalida. Digite um número inteiro positivo: ");
        // Limpa o buffer de entrada em caso de erro de leitura (caractere não numérico)
        while (getchar() != '\n'); 
    }

    // Adiciona o item na próxima posição livre do vetor
    inventario[totalItens] = novoItem;
    totalItens++;

    printf("\n[SUCESSO] Item '%s' adicionado a mochila! Total: %d/%d.\n", novoItem.nome, totalItens, MAX_ITENS);
    listarItens();
}

/**
 * @brief Função auxiliar que busca o índice de um item pelo nome.
 * @param nomeBusca O nome do item a ser procurado.
 * @return O índice do item no vetor ou -1 se não for encontrado.
 */
int buscarIndiceItem(const char *nomeBusca) {
    // Requisito: Busca sequencial
    for (int i = 0; i < totalItens; i++) {
        // Compara a string de busca com o nome de cada item no inventário
        if (strcmp(inventario[i].nome, nomeBusca) == 0) {
            return i; // Item encontrado
        }
    }
    return -1; // Item não encontrado
}

/**
 * @brief Remove um item do inventário pelo nome.
 * Requisito: Remoção de itens.
 */
void removerItem() {
    if (totalItens == 0) {
        printf("\n[AVISO] A mochila esta vazia. Nada para remover.\n");
        return;
    }

    char nomeRemover[30];
    printf("\n--- REMOVER ITEM ---\n");
    printf("Digite o NOME do item a ser REMOVIDO: ");
    if (scanf("%29s", nomeRemover) != 1) {
        printf("[ERRO] Falha na leitura do nome.\n");
        return;
    }

    int indice = buscarIndiceItem(nomeRemover);

    if (indice != -1) {
        // Item encontrado. Inicia a remoção.
        printf("[CONFIRMADO] Item '%s' (%d unidades) sera removido.\n", 
               inventario[indice].nome, inventario[indice].quantidade);

        // Desloca todos os itens após o removido uma posição para trás,
        // preenchendo o "buraco" deixado.
        for (int i = indice; i < totalItens - 1; i++) {
            inventario[i] = inventario[i + 1];
        }

        // Diminui o contador total de itens
        totalItens--;

        printf("\n[SUCESSO] Item '%s' removido da mochila.\n", nomeRemover);
    } else {
        printf("\n[ERRO] Item '%s' não encontrado na mochila.\n", nomeRemover);
    }

    listarItens();
}

/**
 * @brief Exibe a lista completa de todos os itens no inventário.
 * Requisito: Listagem dos itens registrados com seus dados.
 */
void listarItens() {
    printf("\n============================================\n");
    printf("--- INVENTARIO (MOCHILA) - %d/%d ITENS ---\n", totalItens, MAX_ITENS);
    printf("============================================\n");
    
    if (totalItens == 0) {
        printf("[VAZIO] A mochila está pronta para receber loot!\n");
        printf("============================================\n");
        return;
    }

    // Percorre o vetor de structs do início (0) até a quantidade de itens cadastrados
    for (int i = 0; i < totalItens; i++) {
        printf("%02d. Nome: %-15s | Tipo: %-10s | Quantidade: %d\n", 
               i + 1, inventario[i].nome, inventario[i].tipo, inventario[i].quantidade);
    }

    printf("============================================\n");
}

/**
 * @brief Localiza e exibe os dados de um item específico pelo nome.
 * Requisito: Busca sequencial.
 */
void buscarItem() {
    if (totalItens == 0) {
        printf("\n[AVISO] A mochila esta vazia. Não há itens para buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("\n--- BUSCA SEQUENCIAL DE ITEM ---\n");
    printf("Digite o NOME do item para buscar: ");
    if (scanf("%29s", nomeBusca) != 1) {
        printf("[ERRO] Falha na leitura do nome.\n");
        return;
    }

    int indice = buscarIndiceItem(nomeBusca);

    if (indice != -1) {
        // Item encontrado, exibe os detalhes.
        printf("\n[ITEM ENCONTRADO!]\n");
        printf("--------------------------------\n");
        printf("Nome:       %s\n", inventario[indice].nome);
        printf("Tipo:       %s\n", inventario[indice].tipo);
        printf("Quantidade: %d\n", inventario[indice].quantidade);
        printf("--------------------------------\n");
    } else {
        printf("\n[NAO ENCONTRADO] O item '%s' nao esta na mochila.\n", nomeBusca);
    }
}

// ----------------------------------------------------------------------
// --- Função Principal e Menu ---
// ----------------------------------------------------------------------

/**
 * @brief Exibe o menu principal de opções para o jogador.
 * Requisito: Usabilidade e interface orientativa.
 */
void exibirMenu() {
    printf("\n+------------------------------------------------+\n");
    printf("|          MOCHILA DE LOOT INICIAL (v1.0)        |\n");
    printf("+------------------------------------------------+\n");
    printf("| Escolha sua proxima ACAO ESTRATEGICA:          |\n");
    printf("| 1. Coletar Item (Cadastrar)                    |\n");
    printf("| 2. Descartar Item (Remover)                    |\n");
    printf("| 3. Verificar Inventario (Listar)               |\n");
    printf("| 4. Inspecionar Item (Buscar)                   |\n");
    printf("| 0. Sair e Continuar Jogo                       |\n");
    printf("+------------------------------------------------+\n");
    printf("Sua Opcao: ");
}

int main() {
    int opcao;

    printf("Bem-vindo ao sistema de Inventario! Colete seus recursos essenciais. (Pressione ENTER para comecarmos)\n");

    // Loop principal do jogo/menu
    do {
        // Garante que o buffer de entrada está limpo antes de chamar o menu (boa prática)
        while (getchar() != '\n'); 
        
        exibirMenu();
        
        if (scanf("%d", &opcao) != 1) {
            printf("\n[AVISO] Opcao invalida. Digite apenas números de 0 a 4.\n");
            opcao = -1; // Força uma nova iteração
            continue;
        }

        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                buscarItem();
                break;
            case 0:
                printf("\n[FIM] Inventario organizado. Boa sorte na proxima fase do jogo!\n");
                break;
            default:
                printf("\n[AVISO] Opcao invalida. Tente novamente.\n");
        }
        
    } while (opcao != 0);

    return 0;
}