#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Defini��o da estrutura de produto
typedef struct {
    int codigo;
    char numero_serie[51];
    char data_validade[11]; // formato: DD/MM/AAAA
} Produto;

// Defini��o da estrutura de n� da heap m�nimo
typedef struct {
    Produto produto;
    int prioridade; // Prioridade = data de validade
} HeapNode;

// Defini��o da estrutura de heap m�nimo
typedef struct {
    HeapNode *elementos;
    int tamanho;
    int capacidade;
} MinHeap;

// Prot�tipos das fun��es
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
                printf("Op��o inv�lida. Tente novamente.\n");
        }
    } while (escolha != 9);

    destruirHeap(heap); // Libera a mem�ria alocada pela heap
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

    // Reajusta a heap para manter a propriedade de heap m�nimo
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

    // Move o �ltimo elemento para a raiz
    heap->elementos[0] = heap->elementos[heap->tamanho];

    // Reajusta a heap para manter a propriedade de heap m�nimo
    minHeapify(heap, 0);

    return raiz;
}

void incluirProduto(MinHeap *heap) {
    Produto novo_produto;
    int prioridade;

    printf("Digite o c�digo do produto: ");
    scanf("%d", &novo_produto.codigo);
    printf("Digite o n�mero de s�rie do produto (at� 50 caracteres): ");
    scanf("%s", novo_produto.numero_serie);
    printf("Digite a data de validade do produto (formato DD/MM/AAAA): ");
    scanf("%s", novo_produto.data_validade);
    printf("Digite a prioridade do produto: ");
    scanf("%d", &prioridade);

    inserir(heap, novo_produto, prioridade);
    printf("Produto inclu�do com sucesso!\n");
}

void alterarProduto(MinHeap *heap) {
    int codigo;

    printf("Digite o c�digo do produto que deseja alterar: ");
    scanf("%d", &codigo);

    // Procurar o �ndice do produto com o c�digo fornecido
    int indice = -1;
    for (int i = 0; i < heap->tamanho; i++) {
        if (heap->elementos[i].produto.codigo == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Produto com c�digo %d n�o encontrado.\n", codigo);
        return;
    }

    // Solicitar ao usu�rio as novas informa��es para o produto
    printf("Digite o novo n�mero de s�rie do produto (at� 50 caracteres): ");
    scanf("%s", heap->elementos[indice].produto.numero_serie);
    printf("Digite a nova data de validade do produto (formato DD/MM/AAAA): ");
    scanf("%s", heap->elementos[indice].produto.data_validade);

    printf("Produto com c�digo %d alterado com sucesso.\n", codigo);
}

void excluirProduto(MinHeap *heap) {
    int codigo;

    printf("Digite o c�digo do produto que deseja excluir: ");
    scanf("%d", &codigo);
    
    // Procurar o �ndice do produto com o c�digo fornecido
    int indice = -1;
    for (int i = 0; i < heap->tamanho; i++) {
        if (heap->elementos[i].produto.codigo == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Produto com c�digo %d n�o encontrado.\n", codigo);
    } else {
        // Remover o produto do heap
        heap->elementos[indice] = heap->elementos[heap->tamanho - 1];
        heap->tamanho--;

        printf("Produto com c�digo %d exclu�do com sucesso.\n", codigo);
    }

}

void consultarProduto(MinHeap *heap) {
    int codigo;

    printf("Digite o c�digo do produto que deseja consultar: ");
    scanf("%d", &codigo);
    
     // Procurar o �ndice do produto com o c�digo fornecido
    int indice = -1;
    for (int i = 0; i < heap->tamanho; i++) {
        if (heap->elementos[i].produto.codigo == codigo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Produto com c�digo %d n�o encontrado.\n", codigo);
    } else {
        printf("Produto encontrado:\n");
        printf("C�digo: %d\n", heap->elementos[indice].produto.codigo);
        printf("N�mero de S�rie: %s\n", heap->elementos[indice].produto.numero_serie);
        printf("Data de Validade: %s\n", heap->elementos[indice].produto.data_validade);
    }

}

void listarProdutos(MinHeap *heap) {
    printf("Listando todos os produtos:\n");

    for (int i = 0; i < heap->tamanho; i++) {
        printf("C�digo: %d, N�mero de S�rie: %s, Data de Validade: %s\n", heap->elementos[i].produto.codigo, heap->elementos[i].produto.numero_serie, heap->elementos[i].produto.data_validade);
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
    printf("Escolha uma op��o: ");
    scanf("%d", &escolha);

    return escolha;
}
