#include <iostream>

using namespace std;

// Nó da árvore
struct No {
    int chave;
    No* esquerda;
    No* direita;

    // Construtor
    No(int valor) {
        chave = valor;
        esquerda = nullptr;
        direita = nullptr;
    }
};

// Classe da Árvore Binária de Busca (BST)
class ArvoreBinariaBusca {
private:
    No* raiz; // Ponteiro para a raiz

    /**
     * F1: Insere (recursivo)
     */
    No* inserirRecursivo(No* no, int valor) {
        // Caso base: árvore vazia, cria novo nó
        if (no == nullptr) {
            return new No(valor);
        }

        // Caso recursivo: busca local de inserção
        if (valor < no->chave) {
            no->esquerda = inserirRecursivo(no->esquerda, valor);
        } else if (valor > no->chave) {
            no->direita = inserirRecursivo(no->direita, valor);
        }
        // (Ignora valores duplicados)

        return no;
    }

    /**
     * F3: Remove (recursivo)
     */
    No* removerRecursivo(No* no, int valor) {
        // Caso base: Nó não encontrado
        if (no == nullptr) {
            return no;
        }

        // 1. Encontrar o nó
        if (valor < no->chave) {
            no->esquerda = removerRecursivo(no->esquerda, valor);
        } else if (valor > no->chave) {
            no->direita = removerRecursivo(no->direita, valor);
        } else {
            // 2. Nó encontrado. Tratar os 3 casos:

            // Caso 1: Nó folha (sem filhos)
            if (no->esquerda == nullptr && no->direita == nullptr) {
                delete no;
                return nullptr;
            }
            // Caso 2: Nó com um filho
            else if (no->esquerda == nullptr) {
                No* temp = no->direita;
                delete no;
                return temp;
            } else if (no->direita == nullptr) {
                No* temp = no->esquerda;
                delete no;
                return temp;
            }

            // Caso 3: Nó com dois filhos
            // Encontra o sucessor (menor da subárvore direita)
            No* temp = encontrarMinimo(no->direita);
            // Copia o valor do sucessor
            no->chave = temp->chave;
            // Remove o sucessor da subárvore direita
            no->direita = removerRecursivo(no->direita, temp->chave);
        }
        return no;
    }

    /**
     * F2: Lista em In-Ordem (recursivo)
     * (Esquerda, Raiz, Direita) - Gera lista ordenada
     */
    void listarInOrdem(No* no) {
        if (no != nullptr) {
            listarInOrdem(no->esquerda);
            cout << no->chave << " ";
            listarInOrdem(no->direita);
        }
    }

    // Auxiliar: Encontra o menor nó (o mais à esquerda)
    No* encontrarMinimo(No* no) {
        while (no != nullptr && no->esquerda != nullptr) {
            no = no->esquerda;
        }
        return no;
    }

    // Auxiliar: Libera a memória da árvore (usado no destrutor)
    void destruirArvore(No* no) {
        if (no != nullptr) {
            destruirArvore(no->esquerda);
            destruirArvore(no->direita);
            delete no;
        }
    }

public:
    // Construtor
    ArvoreBinariaBusca() {
        raiz = nullptr;
    }

    // Destrutor (libera memória)
    ~ArvoreBinariaBusca() {
        destruirArvore(raiz);
    }

    // F1: Inserção (pública)
    void inserir(int valor) {
        raiz = inserirRecursivo(raiz, valor);
    }

    // F3: Remoção (pública)
    void remover(int valor) {
        raiz = removerRecursivo(raiz, valor);
    }

    // F2: Listagem (pública)
    void listar() {
        listarInOrdem(raiz);
        cout << endl;
    }
};

// --- Função Principal ---
int main() {
    ArvoreBinariaBusca bst;

    // --- Item 1a: Inserção ---
    int valores[] = {20, 5, 12, 36, 27, 45, 9, 2, 6, 17, 40};
    cout << "Inserindo valores..." << endl;
    for (int val : valores) {
        bst.inserir(val);
    }

    cout << "------------------------------------------------" << endl;
    cout << "Listagem (arvore 'a') completa:" << endl;
    bst.listar(); 
    cout << "------------------------------------------------" << endl;

    // --- Item 1b: Remoção ---
    cout << "Removendo 9..." << endl;
    bst.remover(9);
    bst.listar(); 

    cout << "\nRemovendo 5..." << endl;
    bst.remover(5);
    bst.listar();  

    cout << "\nRemovendo 20..." << endl;
    bst.remover(20);
    cout << "Listagem final (apos remover 20):" << endl;
    bst.listar();
    cout << "------------------------------------------------" << endl;

    return 0;
}