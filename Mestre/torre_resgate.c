#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definindo o número máximo de componentes
#define MAX_COMPONENTES 20
#define TAM_NOME 30
#define TAM_TIPO 20

// 1. Criação de struct: Componente
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int prioridade;
} Componente;

// Variáveis globais para armazenar as comparações
long long comparacoes_globais = 0;

// Protótipos das funções
void menuPrincipal();
int cadastrarComponentes(Componente componentes[], int *numComponentes);
void mostrarComponentes(Componente componentes[], int numComponentes);
void bubbleSortNome(Componente componentes[], int numComponentes);
void insertionSortTipo(Componente componentes[], int numComponentes);
void selectionSortPrioridade(Componente componentes[], int numComponentes);
int buscaBinariaPorNome(Componente componentes[], int numComponentes, char chave[]);
void medirDesempenho(void (*algoritmo)(Componente[], int), Componente componentes[], int numComponentes);

// =========================================================================
// FUNÇÕES AUXILIARES
// =========================================================================

/**
 * @brief Remove o caractere de nova linha (\n) lido por fgets.
 * * @param str A string a ser processada.
 */
void limparNovaLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

/**
 * @brief Troca dois componentes de posição.
 * * @param a Ponteiro para o primeiro componente.
 * @param b Ponteiro para o segundo componente.
 */
void trocar(Componente *a, Componente *b) {
    Componente temp = *a;
    *a = *b;
    *b = temp;
}

// =========================================================================
// 2. ENTRADA DE DADOS: Cadastro de Componentes
// =========================================================================

/**
 * @brief Permite ao usuário cadastrar os componentes.
 * * @param componentes O array de structs Componente.
 * @param numComponentes Ponteiro para a variável que armazena o total de componentes.
 * @return int O novo número de componentes.
 */
int cadastrarComponentes(Componente componentes[], int *numComponentes) {
    char input[10];
    int i = *numComponentes;
    
    printf("\n--- Cadastro de Componentes ---\n");
    printf("Voce pode cadastrar ate %d componentes.\n", MAX_COMPONENTES);
    
    // Continua o loop enquanto houver espaço e o usuário desejar cadastrar
    while (i < MAX_COMPONENTES) {
        printf("\nCadastrando Componente %d/%d:\n", i + 1, MAX_COMPONENTES);
        
        // Nome
        printf("Nome do componente (max %d caracteres): ", TAM_NOME - 1);
        if (fgets(componentes[i].nome, TAM_NOME, stdin) == NULL) break;
        limparNovaLinha(componentes[i].nome);
        
        // Tipo
        printf("Tipo do componente (ex: controle, suporte, propulsao) (max %d caracteres): ", TAM_TIPO - 1);
        if (fgets(componentes[i].tipo, TAM_TIPO, stdin) == NULL) break;
        limparNovaLinha(componentes[i].tipo);

        // Prioridade
        printf("Prioridade (de 1 a 10): ");
        if (fgets(input, 10, stdin) == NULL) break;
        componentes[i].prioridade = atoi(input);

        // Validação básica da prioridade
        if (componentes[i].prioridade < 1 || componentes[i].prioridade > 10) {
            printf("Prioridade invalida. Definida para 1. Por favor, ajuste.\n");
            componentes[i].prioridade = 1;
        }

        i++; // Incrementa o número de componentes cadastrados

        if (i < MAX_COMPONENTES) {
            printf("\nDeseja cadastrar outro componente? (s/n): ");
            if (fgets(input, 10, stdin) == NULL || (input[0] != 's' && input[0] != 'S')) {
                break;
            }
        }
    }
    
    *numComponentes = i;
    printf("\n--- %d Componentes cadastrados ---\n", *numComponentes);
    return i;
}

// =========================================================================
// 6. MONTAGEM FINAL: Exibir Componentes
// =========================================================================

/**
 * @brief Exibe o array de componentes formatado.
 * * @param componentes O array de structs Componente.
 * @param numComponentes O número de componentes no array.
 */
void mostrarComponentes(Componente componentes[], int numComponentes) {
    if (numComponentes == 0) {
        printf("Nenhum componente cadastrado para exibir.\n");
        return;
    }
    
    printf("\n--- Lista de Componentes (%d) ---\n", numComponentes);
    printf("----------------------------------------------------------------\n");
    printf("| %-30s | %-18s | %-10s |\n", "NOME", "TIPO", "PRIORIDADE");
    printf("----------------------------------------------------------------\n");
    for (int i = 0; i < numComponentes; i++) {
        printf("| %-30s | %-18s | %-10d |\n", componentes[i].nome, componentes[i].tipo, componentes[i].prioridade);
    }
    printf("----------------------------------------------------------------\n");
}

// =========================================================================
// 3. OPÇÕES DE ORDENAÇÃO E 5. MEDIÇÃO DE DESEMPENHO
// =========================================================================

/**
 * @brief 3. Selection Sort: Ordena os componentes por prioridade (int).
 * @param componentes O array de structs Componente.
 * @param numComponentes O número de componentes no array.
 */
void selectionSortPrioridade(Componente componentes[], int numComponentes) {
    int i, j, min_idx;
    comparacoes_globais = 0;

    // Um por um, move a fronteira do sub-array não ordenado
    for (i = 0; i < numComponentes - 1; i++) {
        // Encontra o elemento com a menor prioridade no sub-array não ordenado
        min_idx = i;
        for (j = i + 1; j < numComponentes; j++) {
            // Conta a comparação: prioridade atual vs. prioridade mínima
            comparacoes_globais++; 
            if (componentes[j].prioridade < componentes[min_idx].prioridade) {
                min_idx = j;
            }
        }

        // Troca o menor elemento encontrado com o primeiro elemento do sub-array
        if (min_idx != i) {
            trocar(&componentes[min_idx], &componentes[i]);
        }
    }
}

/**
 * @brief 3. Insertion Sort: Ordena os componentes por tipo (string).
 * @param componentes O array de structs Componente.
 * @param numComponentes O número de componentes no array.
 */
void insertionSortTipo(Componente componentes[], int numComponentes) {
    int i, j;
    Componente chave;
    comparacoes_globais = 0;

    for (i = 1; i < numComponentes; i++) {
        chave = componentes[i];
        j = i - 1;

        // Move os elementos de componentes[0..i-1], que são maiores que a chave,
        // para uma posição à frente de sua posição atual
        while (j >= 0) {
            // Conta a comparação: tipo do elemento anterior vs. tipo da chave
            comparacoes_globais++;
            // strcmp retorna > 0 se o primeiro argumento for "maior" (depois)
            if (strcmp(componentes[j].tipo, chave.tipo) > 0) { 
                componentes[j + 1] = componentes[j];
                j = j - 1;
            } else {
                // Se a comparação for <= 0, o loop para
                break; 
            }
        }
        // Se o loop parou por comparação ou j < 0, a chave é inserida na posição correta
        componentes[j + 1] = chave; 
    }
}


/**
 * @brief 3. Bubble Sort: Ordena os componentes por nome (string).
 * @param componentes O array de structs Componente.
 * @param numComponentes O número de componentes no array.
 */
void bubbleSortNome(Componente componentes[], int numComponentes) {
    int i, j;
    int trocou;
    comparacoes_globais = 0;

    for (i = 0; i < numComponentes - 1; i++) {
        trocou = 0;
        for (j = 0; j < numComponentes - 1 - i; j++) {
            // Conta a comparação: nome do componente atual vs. nome do próximo
            comparacoes_globais++; 
            // strcmp retorna > 0 se o primeiro nome for "maior" (depois na ordem lexicográfica)
            if (strcmp(componentes[j].nome, componentes[j+1].nome) > 0) { 
                trocar(&componentes[j], &componentes[j+1]);
                trocou = 1;
            }
        }
        // Otimização: se nenhuma troca ocorreu na passagem interna, o array está ordenado
        if (trocou == 0) {
            break;
        }
    }
}

/**
 * @brief Função genérica para medir o tempo de execução e chamar o algoritmo.
 * * @param algoritmo Ponteiro para a função de ordenação a ser executada.
 * @param componentes O array de structs Componente.
 * @param numComponentes O número de componentes no array.
 */
void medirDesempenho(void (*algoritmo)(Componente[], int), Componente componentes[], int numComponentes) {
    clock_t inicio, fim;
    double tempo_execucao;

    // Inicia a contagem do tempo
    inicio = clock(); 
    
    // Chama o algoritmo de ordenação
    algoritmo(componentes, numComponentes);

    // Finaliza a contagem do tempo
    fim = clock(); 
    
    // Calcula o tempo em segundos
    tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC; 

    // 5. Medição de desempenho: Exibe o feedback numérico
    printf("\n--- Desempenho do Algoritmo ---\n");
    printf("Numero de comparacoes: %lld\n", comparacoes_globais);
    printf("Tempo de execucao (segundos): %.6f\n", tempo_execucao);
    printf("-------------------------------\n");

    // 6. Montagem final: Exibe o resultado da ordenação
    mostrarComponentes(componentes, numComponentes);
}


// =========================================================================
// 4. BUSCA BINÁRIA
// =========================================================================

/**
 * @brief 4. Busca Binária Otimizada: Localiza o componente-chave por nome (string).
 * @note Deve ser aplicada APENAS após a ordenação por nome.
 * * @param componentes O array de structs Componente.
 * @param numComponentes O número de componentes no array.
 * @param chave A string (nome) a ser buscada.
 * @return int O índice do componente encontrado, ou -1 se não for encontrado.
 */
int buscaBinariaPorNome(Componente componentes[], int numComponentes, char chave[]) {
    int esq = 0;
    int dir = numComponentes - 1;
    int comparacoes_busca = 0;
    
    printf("Iniciando Busca Binaria por nome: '%s'\n", chave);

    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        int resultado;

        // Conta a comparação
        comparacoes_busca++;
        
        // Compara a chave com o nome do componente no meio
        resultado = strcmp(componentes[meio].nome, chave);

        if (resultado == 0) {
            // Componente encontrado
            printf("Busca Binaria finalizada. Comparacoes: %d\n", comparacoes_busca);
            return meio;
        }
        
        if (resultado < 0) {
            // Chave é "maior" (depois), então ignora a metade esquerda
            esq = meio + 1;
        } else {
            // Chave é "menor" (antes), então ignora a metade direita
            dir = meio - 1;
        }
    }

    // Componente não encontrado
    printf("Busca Binaria finalizada. Comparacoes: %d\n", comparacoes_busca);
    return -1; 
}


// =========================================================================
// INTERFACE E FUNÇÃO PRINCIPAL
// =========================================================================

/**
 * @brief Menu principal e lógica de interação.
 */
void menuPrincipal() {
    Componente torre[MAX_COMPONENTES];
    int numComponentes = 0;
    int opcao = 0;
    char input[10];
    int estado_ordenacao = 0; // 0: Não ordenado, 1: Ordenado por Nome, 2: Por Tipo, 3: Por Prioridade
    
    // Tentativa de cadastro inicial de alguns dados de exemplo (se necessário)
    // Para simplificar, vou deixar o cadastro manual no menu.

    // 2. Entrada dos dados: Cadastro de componentes de exemplo (apenas para teste rápido)
    numComponentes = 5;
    strcpy(torre[0].nome, "Chip Central");
    strcpy(torre[0].tipo, "Controle");
    torre[0].prioridade = 8;
    
    strcpy(torre[1].nome, "Placa de Propulsao");
    strcpy(torre[1].tipo, "Propulsao");
    torre[1].prioridade = 9;

    strcpy(torre[2].nome, "Estrutura de Suporte");
    strcpy(torre[2].tipo, "Suporte");
    torre[2].prioridade = 5;

    strcpy(torre[3].nome, "Cabo de Energia");
    strcpy(torre[3].tipo, "Controle");
    torre[3].prioridade = 3;

    strcpy(torre[4].nome, "Sistema de Navegacao");
    strcpy(torre[4].tipo, "Controle");
    torre[4].prioridade = 10;
    
    printf("Bem-vindo ao Modulo Avancado de Organizacao da Torre de Resgate!\n");
    printf("%d Componentes de exemplo carregados.\n", numComponentes);

    // Loop principal do menu
    while (opcao != 6) {
        printf("\n=================================================================\n");
        printf("ESTRATEGIA DE FUGA - MISSAO FINAL\n");
        printf("Componentes cadastrados: %d. Estado atual: ", numComponentes);
        switch (estado_ordenacao) {
            case 1: printf("ORDENADO POR NOME (Bubble Sort)\n"); break;
            case 2: printf("ORDENADO POR TIPO (Insertion Sort)\n"); break;
            case 3: printf("ORDENADO POR PRIORIDADE (Selection Sort)\n"); break;
            default: printf("NAO ORDENADO\n"); break;
        }
        printf("-----------------------------------------------------------------\n");
        printf("1. Cadastrar Componentes (Max %d)\n", MAX_COMPONENTES);
        printf("2. Ordenar por NOME (Bubble Sort + Busca Binaria)\n");
        printf("3. Ordenar por TIPO (Insertion Sort)\n");
        printf("4. Ordenar por PRIORIDADE (Selection Sort)\n");
        printf("5. Buscar Componente-Chave (Apenas apos Opcao 2)\n");
        printf("6. Sair\n");
        printf("Escolha sua estrategia: ");

        if (fgets(input, 10, stdin) == NULL) continue;
        opcao = atoi(input);

        switch (opcao) {
            case 1:
                cadastrarComponentes(torre, &numComponentes);
                estado_ordenacao = 0; // Se houver novos dados, o array deve ser considerado não ordenado
                break;

            case 2:
                if (numComponentes == 0) { printf("Cadastre componentes primeiro.\n"); break; }
                printf("\n--- Estrategia de Ordenacao: NOME (Bubble Sort) ---\n");
                medirDesempenho(bubbleSortNome, torre, numComponentes);
                estado_ordenacao = 1;
                break;

            case 3:
                if (numComponentes == 0) { printf("Cadastre componentes primeiro.\n"); break; }
                printf("\n--- Estrategia de Ordenacao: TIPO (Insertion Sort) ---\n");
                medirDesempenho(insertionSortTipo, torre, numComponentes);
                estado_ordenacao = 2;
                break;

            case 4:
                if (numComponentes == 0) { printf("Cadastre componentes primeiro.\n"); break; }
                printf("\n--- Estrategia de Ordenacao: PRIORIDADE (Selection Sort) ---\n");
                medirDesempenho(selectionSortPrioridade, torre, numComponentes);
                estado_ordenacao = 3;
                break;

            case 5:
                if (estado_ordenacao != 1) {
                    printf("\nERRO: A busca binaria so pode ser aplicada apos a ordenacao por NOME (Opcao 2).\n");
                    printf("Por favor, execute a Opcao 2 primeiro.\n");
                    break;
                }
                
                // Exemplo de componente-chave. Pode ser alterado.
                char chave_busca[TAM_NOME] = "Chip Central"; 
                printf("\n--- Busca por Componente-Chave ---\n");
                
                int indice_encontrado = buscaBinariaPorNome(torre, numComponentes, chave_busca);

                // 6. Montagem final: Confirmação visual da presença
                if (indice_encontrado != -1) {
                    printf("\nATIVACAO DA TORRE DESBLOQUEADA!\n");
                    printf("Componente-chave encontrado na posicao %d:\n", indice_encontrado + 1);
                    printf("-> NOME: %s, TIPO: %s, PRIORIDADE: %d\n", 
                           torre[indice_encontrado].nome, 
                           torre[indice_encontrado].tipo, 
                           torre[indice_encontrado].prioridade);
                } else {
                    printf("\nALERTA: O componente-chave '%s' NAO foi encontrado.\n", chave_busca);
                    printf("A torre de resgate NAO pode ser ativada!\n");
                }
                break;

            case 6:
                printf("\nMissao encerrada. Ate a proxima!\n");
                break;

            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                break;
        }
    }
}

/**
 * @brief Função principal do programa.
 */
int main() {
    // Seta a localização para garantir que o clock() funcione corretamente
    // e possibilita o uso de setbuf(stdout, NULL) para melhor visualização em alguns ambientes.
    setbuf(stdout, NULL); 
    menuPrincipal();
    return 0;
}