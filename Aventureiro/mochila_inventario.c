#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definindo o tamanho máximo da mochila baseada em vetor
#define TAMANHO_MAX_VETOR 10
#define TAM_NOME 30
#define TAM_TIPO 20

// Variáveis globais para contagem de operações
long long comparacoes_busca_sequencial = 0;
long long comparacoes_busca_binaria = 0;
long long comparacoes_ordenacao = 0;

// =========================================================================
// 1. CRIAÇÃO DE STRUCTS
// =========================================================================

// Struct Item: Com os dados do inventário
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade;
} Item;

// Struct No: Para a Lista Encadeada
typedef struct No {
    Item dados;
    struct No *proximo;
} No;


// =========================================================================
// FUNÇÕES AUXILIARES GERAIS
// =========================================================================

/**
 * @brief Remove o caractere de nova linha (\n) lido por fgets.
 * @param str A string a ser processada.
 */
void limparNovaLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

/**
 * @brief Solicita os dados de um novo item ao usuário.
 * @param novoItem Ponteiro para a struct Item a ser preenchida.
 */
void obterDadosItem(Item *novoItem) {
    char input[10];

    printf("Nome do Item (max %d caracteres): ", TAM_NOME - 1);
    if (fgets(novoItem->nome, TAM_NOME, stdin) == NULL) return;
    limparNovaLinha(novoItem->nome);

    printf("Tipo do Item (max %d caracteres): ", TAM_TIPO - 1);
    if (fgets(novoItem->tipo, TAM_TIPO, stdin) == NULL) return;
    limparNovaLinha(novoItem->tipo);

    printf("Quantidade: ");
    if (fgets(input, 10, stdin) == NULL) return;
    novoItem->quantidade = atoi(input);

    if (novoItem->quantidade <= 0) {
        novoItem->quantidade = 1;
        printf("Quantidade invalida. Definida como 1.\n");
    }
}

/**
 * @brief Exibe um item formatado.
 * @param item O item a ser exibido.
 * @param indice O índice/posição do item.
 */
void exibirItem(Item item, int indice) {
    printf("| %-2d | %-28s | %-18s | %-10d |\n", 
           indice + 1, item.nome, item.tipo, item.quantidade);
}


// =========================================================================
// VETOR (MOCHILA SEQUENCIAL)
// =========================================================================

// O vetor em si é declarado na main, esta parte contém as operações.

/**
 * @brief 3. Inserir novo item no vetor (no final).
 * @param vetor O array de itens.
 * @param numItens Ponteiro para o número atual de itens.
 */
void inserirItemVetor(Item vetor[], int *numItens) {
    if (*numItens >= TAMANHO_MAX_VETOR) {
        printf("\nALERTA: A mochila (vetor) esta cheia. Max: %d itens.\n", TAMANHO_MAX_VETOR);
        return;
    }

    Item novoItem;
    obterDadosItem(&novoItem);
    
    vetor[*numItens] = novoItem;
    (*numItens)++;
    printf("\nSUCESSO: Item '%s' inserido na mochila (vetor).\n", novoItem.nome);
}

/**
 * @brief 3. Remover item do vetor por nome (busca sequencial implícita).
 * @param vetor O array de itens.
 * @param numItens Ponteiro para o número atual de itens.
 * @param nomeRemover O nome do item a ser removido.
 */
void removerItemVetor(Item vetor[], int *numItens, char *nomeRemover) {
    int i, j;
    int encontrado = 0;

    for (i = 0; i < *numItens; i++) {
        if (strcmp(vetor[i].nome, nomeRemover) == 0) {
            encontrado = 1;
            // Move todos os itens subsequentes uma posição para a frente
            for (j = i; j < *numItens - 1; j++) {
                vetor[j] = vetor[j + 1];
            }
            (*numItens)--;
            printf("\nSUCESSO: Item '%s' removido da mochila (vetor).\n", nomeRemover);
            return;
        }
    }

    if (!encontrado) {
        printf("\nFALHA: Item '%s' nao encontrado na mochila (vetor).\n", nomeRemover);
    }
}

/**
 * @brief 3. Listar todos os itens do vetor.
 * @param vetor O array de itens.
 * @param numItens O número atual de itens.
 */
void listarItensVetor(Item vetor[], int numItens) {
    if (numItens == 0) {
        printf("\nMochila (vetor) vazia.\n");
        return;
    }

    printf("\n--- Mochila (Vetor - %d/%d) ---\n", numItens, TAMANHO_MAX_VETOR);
    printf("-------------------------------------------------------------------------\n");
    printf("| %-2s | %-28s | %-18s | %-10s |\n", "ID", "NOME", "TIPO", "QUANTIDADE");
    printf("-------------------------------------------------------------------------\n");
    for (int i = 0; i < numItens; i++) {
        exibirItem(vetor[i], i);
    }
    printf("-------------------------------------------------------------------------\n");
}

/**
 * @brief 3. Buscar item por nome no vetor (Busca Sequencial).
 * @param vetor O array de itens.
 * @param numItens O número atual de itens.
 * @param chave O nome a ser buscado.
 * @return int O índice do item encontrado, ou -1 se não for encontrado.
 */
int buscarSequencialVetor(Item vetor[], int numItens, char chave[]) {
    comparacoes_busca_sequencial = 0;
    for (int i = 0; i < numItens; i++) {
        comparacoes_busca_sequencial++; // Conta a comparação
        if (strcmp(vetor[i].nome, chave) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief 4. Ordenação no vetor: Selection Sort por nome.
 * @param vetor O array de itens.
 * @param numItens O número atual de itens.
 */
void ordenarVetor(Item vetor[], int numItens) {
    int i, j, min_idx;
    Item temp;
    comparacoes_ordenacao = 0;

    for (i = 0; i < numItens - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < numItens; j++) {
            comparacoes_ordenacao++; // Conta a comparação de strings
            // strcmp retorna > 0 se vetor[j] for lexicograficamente "maior" que vetor[min_idx]
            if (strcmp(vetor[j].nome, vetor[min_idx].nome) < 0) {
                min_idx = j;
            }
        }
        
        // Troca o elemento mínimo com o primeiro elemento não ordenado
        if (min_idx != i) {
            temp = vetor[min_idx];
            vetor[min_idx] = vetor[i];
            vetor[i] = temp;
        }
    }
    printf("\nSUCESSO: Vetor ordenado por nome (Selection Sort).\n");
    printf("Total de comparacoes para ordenacao: %lld\n", comparacoes_ordenacao);
}

/**
 * @brief 5. Busca binária (apenas no vetor), por nome.
 * @param vetor O array de itens.
 * @param numItens O número atual de itens.
 * @param chave O nome a ser buscado.
 * @return int O índice do item encontrado, ou -1 se não for encontrado.
 */
int buscarBinariaVetor(Item vetor[], int numItens, char chave[]) {
    int esq = 0;
    int dir = numItens - 1;
    comparacoes_busca_binaria = 0;

    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        int resultado;

        comparacoes_busca_binaria++; // Conta a comparação
        resultado = strcmp(vetor[meio].nome, chave);

        if (resultado == 0) {
            // Item encontrado
            return meio;
        }
        
        if (resultado < 0) {
            // Chave é "maior" (depois), ignora metade esquerda
            esq = meio + 1;
        } else {
            // Chave é "menor" (antes), ignora metade direita
            dir = meio - 1;
        }
    }

    return -1; // Item não encontrado
}


// =========================================================================
// LISTA ENCADEADA (MOCHILA DINÂMICA)
// =========================================================================

// O ponteiro de cabeça 'No *mochila_lista' é declarado na main.

/**
 * @brief 3. Inserir novo item na lista encadeada (no início, por eficiência).
 * @param cabeca Ponteiro para o ponteiro do nó cabeça da lista.
 * @param item O item a ser inserido.
 */
void inserirItemLista(No **cabeca) {
    Item novoItem;
    obterDadosItem(&novoItem);

    No *novoNo = (No *)malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("ERRO: Falha ao alocar memoria para o novo item.\n");
        return;
    }

    novoNo->dados = novoItem;
    // O novo nó aponta para o antigo cabeça
    novoNo->proximo = *cabeca; 
    // O ponteiro cabeça agora aponta para o novo nó
    *cabeca = novoNo; 

    printf("\nSUCESSO: Item '%s' inserido na mochila (lista encadeada).\n", novoItem.nome);
}

/**
 * @brief 3. Remover item da lista encadeada por nome.
 * @param cabeca Ponteiro para o ponteiro do nó cabeça da lista.
 * @param nomeRemover O nome do item a ser removido.
 */
void removerItemLista(No **cabeca, char *nomeRemover) {
    No *atual = *cabeca;
    No *anterior = NULL;
    int encontrado = 0;

    // Percorre a lista para encontrar o nó com o nome
    while (atual != NULL) {
        if (strcmp(atual->dados.nome, nomeRemover) == 0) {
            encontrado = 1;
            break;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    if (!encontrado) {
        printf("\nFALHA: Item '%s' nao encontrado na mochila (lista encadeada).\n", nomeRemover);
        return;
    }

    // Se o nó a ser removido for o cabeça
    if (anterior == NULL) {
        *cabeca = atual->proximo;
    } else {
        // Ignora o nó atual, ligando o anterior ao próximo
        anterior->proximo = atual->proximo;
    }

    free(atual); // Libera a memória do nó removido
    printf("\nSUCESSO: Item '%s' removido da mochila (lista encadeada).\n", nomeRemover);
}

/**
 * @brief 3. Listar todos os itens da lista encadeada.
 * @param cabeca O ponteiro do nó cabeça da lista.
 */
void listarItensLista(No *cabeca) {
    if (cabeca == NULL) {
        printf("\nMochila (lista encadeada) vazia.\n");
        return;
    }

    No *atual = cabeca;
    int contador = 0;

    printf("\n--- Mochila (Lista Encadeada) ---\n");
    printf("-------------------------------------------------------------------------\n");
    printf("| %-2s | %-28s | %-18s | %-10s |\n", "ID", "NOME", "TIPO", "QUANTIDADE");
    printf("-------------------------------------------------------------------------\n");
    while (atual != NULL) {
        exibirItem(atual->dados, contador++);
        atual = atual->proximo;
    }
    printf("-------------------------------------------------------------------------\n");
}

/**
 * @brief 3. Buscar item por nome na lista encadeada (Busca Sequencial).
 * @param cabeca O ponteiro do nó cabeça da lista.
 * @param chave O nome a ser buscado.
 * @return No* O nó encontrado, ou NULL se não for encontrado.
 */
No* buscarSequencialLista(No *cabeca, char chave[]) {
    No *atual = cabeca;
    comparacoes_busca_sequencial = 0;
    
    while (atual != NULL) {
        comparacoes_busca_sequencial++; // Conta a comparação
        if (strcmp(atual->dados.nome, chave) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}


// =========================================================================
// MENU E FUNÇÃO PRINCIPAL
// =========================================================================

/**
 * @brief Função principal do menu de operações para uma estrutura.
 * @param estrutura Tipo de estrutura ("Vetor" ou "Lista").
 * @param vetor O array de itens (NULL se for lista).
 * @param numItens Ponteiro para o número de itens no vetor (NULL se for lista).
 * @param lista Ponteiro para o ponteiro do nó cabeça da lista (NULL se for vetor).
 */
void menuOperacoes(const char *estrutura, Item vetor[], int *numItens, No **lista) {
    int opcao = 0;
    char input[10];
    char nomeBusca[TAM_NOME];
    int vetor_ordenado = 0;
    
    // Medir o tempo é opcional, mas garante o requisito de Eficiência
    clock_t inicio, fim;
    double tempo_execucao;

    while (opcao != 9) {
        printf("\n=== OPERACOES DA MOCHILA (%s) ===\n", estrutura);
        printf("1. Inserir Item\n");
        printf("2. Remover Item por Nome\n");
        printf("3. Listar Itens\n");
        printf("4. Buscar Item (Busca Sequencial)\n");
        
        // Opções exclusivas do vetor
        if (strcmp(estrutura, "Vetor") == 0) {
            printf("5. Ordenar por Nome (Selection Sort)\n");
            printf("6. Buscar Item Critico (Busca Binaria - Requer Ordenacao)\n");
        } else {
             printf("5. (Opcao Indisponivel na Lista Encadeada)\n");
             printf("6. (Opcao Indisponivel na Lista Encadeada)\n");
        }
        printf("9. Voltar ao Menu Principal\n");
        printf("Escolha a operacao: ");
        
        if (fgets(input, 10, stdin) == NULL) continue;
        opcao = atoi(input);

        inicio = clock(); // Inicia a contagem do tempo

        switch (opcao) {
            case 1: // Inserir
                if (strcmp(estrutura, "Vetor") == 0) {
                    inserirItemVetor(vetor, numItens);
                    vetor_ordenado = 0; // Se inseriu, a ordenação pode ter sido perdida
                } else {
                    inserirItemLista(lista);
                }
                break;

            case 2: // Remover
                printf("Nome do item a remover: ");
                if (fgets(nomeBusca, TAM_NOME, stdin) == NULL) break;
                limparNovaLinha(nomeBusca);
                if (strcmp(estrutura, "Vetor") == 0) {
                    removerItemVetor(vetor, numItens, nomeBusca);
                    vetor_ordenado = 0;
                } else {
                    removerItemLista(lista, nomeBusca);
                }
                break;

            case 3: // Listar
                if (strcmp(estrutura, "Vetor") == 0) {
                    listarItensVetor(vetor, *numItens);
                } else {
                    listarItensLista(*lista);
                }
                break;

            case 4: // Busca Sequencial
                printf("Nome do item para Busca Sequencial: ");
                if (fgets(nomeBusca, TAM_NOME, stdin) == NULL) break;
                limparNovaLinha(nomeBusca);
                
                if (strcmp(estrutura, "Vetor") == 0) {
                    int idx = buscarSequencialVetor(vetor, *numItens, nomeBusca);
                    printf("\n--- Resultado da Busca Sequencial (Vetor) ---\n");
                    printf("Comparacoes: %lld\n", comparacoes_busca_sequencial);
                    if (idx != -1) {
                        printf("Item encontrado na posicao %d.\n", idx + 1);
                        exibirItem(vetor[idx], idx);
                    } else {
                        printf("Item nao encontrado.\n");
                    }
                } else {
                    No *no_encontrado = buscarSequencialLista(*lista, nomeBusca);
                    printf("\n--- Resultado da Busca Sequencial (Lista) ---\n");
                    printf("Comparacoes: %lld\n", comparacoes_busca_sequencial);
                    if (no_encontrado != NULL) {
                        printf("Item encontrado.\n");
                        // O índice é difícil de obter na lista, então exibimos só o item
                        printf("| %-28s | %-18s | %-10s |\n", "NOME", "TIPO", "QUANTIDADE");
                        printf("| %-28s | %-18s | %-10d |\n", no_encontrado->dados.nome, no_encontrado->dados.tipo, no_encontrado->dados.quantidade);
                    } else {
                        printf("Item nao encontrado.\n");
                    }
                }
                break;
            
            case 5: // Ordenar (Apenas Vetor)
                if (strcmp(estrutura, "Vetor") == 0) {
                    if (*numItens == 0) { printf("Vetor vazio. Cadastre itens primeiro.\n"); break; }
                    ordenarVetor(vetor, *numItens);
                    vetor_ordenado = 1;
                    listarItensVetor(vetor, *numItens);
                } else {
                    printf("ERRO: A ordenacao nao e uma operacao eficiente e tipica para Listas Encadeadas.\n");
                }
                break;
            
            case 6: // Busca Binária (Apenas Vetor)
                if (strcmp(estrutura, "Vetor") == 0) {
                    if (!vetor_ordenado) {
                        printf("ERRO: A busca binaria requer que o vetor esteja ordenado. Use a Opcao 5 primeiro.\n");
                        break;
                    }
                    printf("Nome do item para Busca Binaria (Critico): ");
                    if (fgets(nomeBusca, TAM_NOME, stdin) == NULL) break;
                    limparNovaLinha(nomeBusca);
                    
                    int idx = buscarBinariaVetor(vetor, *numItens, nomeBusca);
                    printf("\n--- Resultado da Busca Binaria (Vetor) ---\n");
                    printf("Comparacoes: %lld\n", comparacoes_busca_binaria);
                    if (idx != -1) {
                        printf("ITEM CRITICO ENCONTRADO! Posicao %d.\n", idx + 1);
                        exibirItem(vetor[idx], idx);
                    } else {
                        printf("Item nao encontrado.\n");
                    }
                } else {
                    printf("ERRO: Busca Binaria e exclusiva para o Vetor apos ordenacao.\n");
                }
                break;

            case 9:
                // Sair
                break;
                
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }
        
        fim = clock(); // Finaliza a contagem do tempo
        tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

        // Requisito: Eficiência (tempo de resposta)
        printf("\n[PERFORMANCE] Tempo de resposta: %.4f segundos.\n", tempo_execucao);
        if (tempo_execucao > 2.0) {
            printf("[AVISO DE DESEMPENHO] O tempo de execucao excedeu o limite de 2 segundos.\n");
        }
    }
}


/**
 * @brief Menu principal para selecionar a estrutura de dados.
 */
int main() {
    // Alocação das estruturas principais
    Item mochila_vetor[TAMANHO_MAX_VETOR];
    int numItensVetor = 0;
    No *mochila_lista = NULL; // Ponteiro cabeça da lista

    int opcao = 0;
    char input[10];
    
    // Para visualização clara em alguns ambientes
    setbuf(stdout, NULL);

    printf("Bem-vindo ao Sistema de Inventario da Mochila (Nivel Aventureiro)!\n");

    while (opcao != 3) {
        printf("\n==================================================\n");
        printf("ESCOLHA SUA ESTRUTURA DE DADOS\n");
        printf("1. Mochila (Vetor - Estatica, Melhor para Busca)\n");
        printf("2. Mochila (Lista Encadeada - Dinamica, Melhor para Insercao/Remocao)\n");
        printf("3. Sair do Jogo\n");
        printf("Sua escolha: ");

        if (fgets(input, 10, stdin) == NULL) continue;
        opcao = atoi(input);

        switch (opcao) {
            case 1:
                menuOperacoes("Vetor", mochila_vetor, &numItensVetor, NULL);
                break;
            case 2:
                menuOperacoes("Lista Encadeada", NULL, NULL, &mochila_lista);
                break;
            case 3:
                printf("\nEncerrando a simulacao da mochila. Boa sorte na zona de perigo!\n");
                
                // Libera a memória da lista encadeada ao sair
                No *atual = mochila_lista;
                No *proximo;
                while (atual != NULL) {
                    proximo = atual->proximo;
                    free(atual);
                    atual = proximo;
                }
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }
    }

    return 0;
}