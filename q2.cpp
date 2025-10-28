#include <iostream>
using namespace std;

struct No {
    int valor;
    No *esq, *dir;
};

// Função recursiva que encontra o maior valor
int maiorValor(No* raiz) {
    if (!raiz) return -1;           // árvore vazia
    if (!raiz->dir) return raiz->valor; // último nó à direita
    return maiorValor(raiz->dir);       // continua descendo à direita
}

// Exemplo
No* inserir(No* raiz, int v) {
    if (!raiz) return new No{v, nullptr, nullptr};
    if (v < raiz->valor) raiz->esq = inserir(raiz->esq, v);
    else if (v > raiz->valor) raiz->dir = inserir(raiz->dir, v);
    return raiz;
}

int main() {
    int nums[] = {20, 5, 12, 36, 27, 45, 9, 2, 6, 17, 40};
    No* raiz = nullptr;

    for (int v : nums)
        raiz = inserir(raiz, v);

    cout << "Maior valor na arvore: " << maiorValor(raiz) << "\n";
}