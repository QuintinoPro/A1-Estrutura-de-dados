#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Definição da estrutura de produto
typedef struct {
    int codigo;
    char numero_serie[51];
    char data_validade[11]; // formato: DD/MM/AAAA
} Produto;

// Definição da estrutura de nó da heap mínimo
typedef struct {
    Produto produto;
    int prioridade; // Prioridade = data de validade
} HeapNode;

// Definição da estrutura de heap mínimo
typedef struct {
    HeapNode *elementos;
    int tamanho;
    int capacidade;
} MinHeap;

// Protótipos das funções
MinHeap* criarHeap(int capacidade);
void destruirHeap(MinHeap *heap);
void inserir(MinHeap *heap, Produto produto, int prioridade);
Produto remover(MinHeap *heap);
void listarProdutos(MinHeap *heap);
void incluirProduto(MinHeap *heap);
void alterarProduto(MinHeap *heap);
void excluirProduto(MinHeap *heap);
void consultarProduto(MinHeap *heap);
int menu();

int main() {
    
    setlocale(LC_ALL,"Portuguese_Brazil");
    
    MinHeap *heap = criarHeap(10); // Inicializa heap com capacidade inicial

    int escolha;
    do {
        escolha = menu();

        switch (escolha) {
            case 1:
                incluirProduto(heap);
                break;
            case 2:
                alterarProduto(heap);
                break;
            case 3:
                excluirProduto(heap);
                break;
            case 4:
                consultarProduto(heap);
                break;
            case 5:
                listarProdutos(heap);
                break;
            case 9:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (escolha != 9);

    destruirHeap(heap); // Libera a memória alocada pela heap
    return 0;
}

MinHeap* criarHeap(int capacidade) {
    MinHeap *heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->elementos = (HeapNode*)malloc(capacidade * sizeof(HeapNode));
    heap->tamanho = 0;
    heap->capacidade = capacidade;
    return heap;
}

void destruirHeap(MinHeap *heap) {
    free(heap->elementos);
    free(heap);
}

void minHeapify(MinHeap *heap, int indice) {
    int menor = indice;
    int esquerda = 2 * indice + 1;
    int direita = 2 * indice + 2;

    if (esquerda < heap->tamanho && strcmp(heap->elementos[esquerda].produto.data_validade, heap->elementos[menor].produto.data_validade) < 0)
        menor = esquerda;

    if (direita < heap->tamanho && strcmp(heap->elementos[direita].produto.data_validade, heap->elementos[menor].produto.data_validade) < 0)
        menor = direita;

    if (menor != indice) {
        // Troca os elementos
        HeapNode temp = heap->elementos[indice];
        heap->elementos[indice] = heap->elementos[menor];
        heap->elementos[menor] = temp;

        minHeapify(heap, menor);
    }
}

void inserir(MinHeap *heap, Produto produto, int prioridade) {
    if (heap->tamanho == heap->capacidade) {
        printf("Erro: Heap cheia.\n");
        return;
    }

    int indice = heap->tamanho;
    heap->elementos[indice].produto = produto;
    heap->elementos[indice].prioridade = prioridade;
    heap->tamanho++;

    // Reajusta a heap para manter a propriedade de heap mínimo
    while (indice > 0 && strcmp(heap->elementos[(indice - 1) / 2].produto.data_validade, heap->elementos[indice].produto.data_validade) > 0) {
        // Troca os elementos
        HeapNode temp = heap->elementos[indice];
        heap->elementos[indice] = heap->elementos[(indice - 1) / 2];
        heap->elementos[(indice - 1) / 2] = temp;

        indice = (indice - 1) / 2;
    }
}

Produto remover(MinHeap *heap) {
    if (heap->tamanho == 0) {
        printf("Erro: Heap vazia.\n");
        Produto produto_vazio = {-1, "", ""}; // Retorna um produto vazio
        return produto_vazio;
    }

    Produto raiz = heap->elementos[0].produto;
    heap->tamanho--;

    // Move o último elemento para a raiz
    heap->elementos[0] = heap->elementos[heap->tamanho];

    // Reajusta a heap para manter a propriedade de heap mínimo
    minHeapify(heap, 0);

    return raiz;
}

void incluirProduto(MinHeap *heap) {
    Produto novo_produto;
    int prioridade;

    printf("Digite o código do produto: ");
    scanf("%d", &novo_produto.codigo);
    printf("Digite o número de série do produto (até 50 caracteres): ");
    scanf("%s", novo_produto.numero_serie);
    printf("Digite a data de validade do produto (formato DD/MM/AAAA): ");
    scanf("%s", novo_produto.data_validade);
    printf("Digite a prioridade do produto: ");
    scanf("%d", &prioridade);

    inserir(heap, novo_produto, prioridade);
    printf("Produto incluído com sucesso!\n");
}

void alterarProduto(MinHeap *heap) {
    int codigo;

    printf("Digite o código do produto que deseja alterar: ");
    scanf("%d", &codigo);

    // Procurar o índice do produto com o código fornecido
    int indice = -1;
    for (int i = 0; i < heap->tamanho; i++) {
        if (heap->elementos[i].produto.codigo == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Produto com código %d não encontrado.\n", codigo);
        return;
    }

    // Solicitar ao usuário as novas informações para o produto
    printf("Digite o novo número de série do produto (até 50 caracteres): ");
    scanf("%s", heap->elementos[indice].produto.numero_serie);
    printf("Digite a nova data de validade do produto (formato DD/MM/AAAA): ");
    scanf("%s", heap->elementos[indice].produto.data_validade);

    printf("Produto com código %d alterado com sucesso.\n", codigo);
}

void excluirProduto(MinHeap *heap) {
    int codigo;

    printf("Digite o código do produto que deseja excluir: ");
    scanf("%d", &codigo);
    
    // Procurar o índice do produto com o código fornecido
    int indice = -1;
    for (int i = 0; i < heap->tamanho; i++) {
        if (heap->elementos[i].produto.codigo == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Produto com código %d não encontrado.\n", codigo);
    } else {
        // Remover o produto do heap
        heap->elementos[indice] = heap->elementos[heap->tamanho - 1];
        heap->tamanho--;

        printf("Produto com código %d excluído com sucesso.\n", codigo);
    }

}

void consultarProduto(MinHeap *heap) {
    int codigo;

    printf("Digite o código do produto que deseja consultar: ");
    scanf("%d", &codigo);
    
     // Procurar o índice do produto com o código fornecido
    int indice = -1;
    for (int i = 0; i < heap->tamanho; i++) {
        if (heap->elementos[i].produto.codigo == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Produto com código %d não encontrado.\n", codigo);
    } else {
        printf("Produto encontrado:\n");
        printf("Código: %d\n", heap->elementos[indice].produto.codigo);
        printf("Número de Série: %s\n", heap->elementos[indice].produto.numero_serie);
        printf("Data de Validade: %s\n", heap->elementos[indice].produto.data_validade);
    }

}

void listarProdutos(MinHeap *heap) {
    printf("Listando todos os produtos:\n");

    for (int i = 0; i < heap->tamanho; i++) {
        printf("Código: %d, Número de Série: %s, Data de Validade: %s\n", heap->elementos[i].produto.codigo, heap->elementos[i].produto.numero_serie, heap->elementos[i].produto.data_validade);
    }
}

int menu() {
    int escolha;

    printf("\nMenu:\n");
    printf("1 - Incluir Produto\n");
    printf("2 - Alterar Produto\n");
    printf("3 - Excluir Produto\n");
    printf("4 - Consultar Produto\n");
    printf("5 - Listar Produtos\n");
    printf("9 - Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &escolha);

    return escolha;
}
