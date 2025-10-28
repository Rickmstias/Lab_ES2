#include <iostream>

using namespace std;

// Estrutura do nó 
struct No {
    int valor;
    No *esq, *dir;
};

// Função para criar um novo nó 
No* novoNo(int v) {
    No* n = new No;
    n->valor = v;
    
    n->esq = n->dir = nullptr; 
    
    return n;
}

// Função de inserção
No* inserir(No* raiz, int v) {
    if (raiz == nullptr)
        return novoNo(v);
    if (v < raiz->valor)
        raiz->esq = inserir(raiz->esq, v);
    else
        raiz->dir = inserir(raiz->dir, v);
    return raiz;
}

/**
 * Questão 5: Verifica se uma árvore binária é cheia.
 * Cheia = Todo nó tem 0 ou 2 filhos (nunca 1).
 * Retorna 1 se for cheia, 0 se não for.
 * Árvore vazia retorna 1.
 */
int ehCheia(No* raiz) {
    // 1. Caso Base: Árvore vazia é considerada cheia.
    if (raiz == nullptr) {
        return 1;
    }

    // 2. Caso Base: Nó folha (0 filhos) é considerado cheio.
    if (raiz->esq == nullptr && raiz->dir == nullptr) {
        return 1;
    }

    // 3. Caso Válido: Nó tem 2 filhos.
    // A árvore só será cheia se AMBAS as sub-árvores também forem.
    if (raiz->esq != nullptr && raiz->dir != nullptr) {
        // A função '&&' (E lógico) garante que ambos os lados
        // devem retornar 1 para que o resultado seja 1.
        return (ehCheia(raiz->esq) && ehCheia(raiz->dir));
    }

    // 4. Caso Inválido: Nó tem apenas 1 filho.
    // (Se chegou aqui, é porque (esq != null && dir == null) 
    // ou (esq == null && dir != null))
    return 0;
}


int main() {
    // Teste 1: Árvore Vazia
    No* raizVazia = nullptr;
    cout << "Arvore Vazia eh cheia? " << ehCheia(raizVazia) << " (Esperado: 1)\n";

    // Teste 2: Árvore Cheia (Raiz com 2 filhos, que são folhas)
    No* raizCheia1 = nullptr;
    raizCheia1 = inserir(raizCheia1, 20);
    raizCheia1 = inserir(raizCheia1, 10);
    raizCheia1 = inserir(raizCheia1, 30);
    cout << "Arvore {20, 10, 30} eh cheia? " << ehCheia(raizCheia1) << " (Esperado: 1)\n";

    // Teste 3: Árvore NÃO Cheia (Nó 10 tem apenas 1 filho à esquerda)
    No* raizNaoCheia = nullptr;
    raizNaoCheia = inserir(raizNaoCheia, 20);
    raizNaoCheia = inserir(raizNaoCheia, 10);
    raizNaoCheia = inserir(raizNaoCheia, 30);
    raizNaoCheia = inserir(raizNaoCheia, 5); // <--- Isso quebra a regra
    cout << "Arvore {20, 10, 30, 5} eh cheia? " << ehCheia(raizNaoCheia) << " (Esperado: 0)\n";
    
    // Teste 4: Árvore Cheia Complexa
    No* raizCheia2 = nullptr;
    raizCheia2 = inserir(raizCheia2, 20);
    raizCheia2 = inserir(raizCheia2, 10);
    raizCheia2 = inserir(raizCheia2, 30);
    raizCheia2 = inserir(raizCheia2, 5);
    raizCheia2 = inserir(raizCheia2, 15); // <-- Agora o nó 10 tem 2 filhos
    cout << "Arvore {20, 10, 30, 5, 15} eh cheia? " << ehCheia(raizCheia2) << " (Esperado: 1)\n";

    return 0;
}