#include <iostream>
#include <limits> 

using namespace std;

struct No {
    int valor;
    int cont;      // contador de ocorrências
    No *esq, *dir;
};

// Cria novo nó
No* novoNo(int v) {
    No* n = new No;
    n->valor = v;
    n->cont = 1;
    n->esq = n->dir = nullptr;
    return n;
}

// Inserção com tratamento de repetidos
No* inserir(No* raiz, int v) {
    if (raiz == nullptr)
        return novoNo(v);

    if (v < raiz->valor)
        raiz->esq = inserir(raiz->esq, v);
    else if (v > raiz->valor)
        raiz->dir = inserir(raiz->dir, v);
    else
        raiz->cont++; // valor repetido → incrementa contador

    return raiz;
}

// Remoção com contador de repetidos
No* remover(No* raiz, int v) {
    if (raiz == nullptr) return raiz;

    if (v < raiz->valor)
        raiz->esq = remover(raiz->esq, v);
    else if (v > raiz->valor)
        raiz->dir = remover(raiz->dir, v);
    else {
        if (raiz->cont > 1) { // ainda tem repetições
            raiz->cont--;
            return raiz;
        }
        // Caso padrão da remoção (contador == 1)
        if (raiz->esq == nullptr) {
            No* temp = raiz->dir;
            delete raiz;
            return temp;
        } else if (raiz->dir == nullptr) {
            No* temp = raiz->esq;
            delete raiz;
            return temp;
        } else {
            // menor da subárvore direita (sucessor)
            No* temp = raiz->dir;
            while (temp->esq != nullptr)
                temp = temp->esq;
            
            // Copia os dados do sucessor
            raiz->valor = temp->valor;
            raiz->cont = temp->cont;
            
            // Seta o contador do sucessor original para 1
            temp->cont = 1; 
            
            // Remove o nó sucessor da sub-árvore direita
            raiz->dir = remover(raiz->dir, temp->valor);
        }
    }
    return raiz;
}

// Listagem (em ordem)
void listar(No* raiz) {
    if (raiz != nullptr) {
        listar(raiz->esq);
        cout << raiz->valor << "(" << raiz->cont << ") ";
        listar(raiz->dir);
    }
}


int main() {
    No* raiz = nullptr;
    int opc, v;

    do {
        cout << "\n1 - Inserir\n2 - Remover\n3 - Listar\n0 - Sair\nEscolha: ";
        
        // --- Validação da Opção (opc) ---
        while (!(cin >> opc)) {
            cout << "Erro: Por favor, insira um numero (0-3).\n";
            cout << "Escolha: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        }

        switch (opc) {
            case 1:
                cout << "Valor: ";
                // --- Validação do Valor (v) ---
                while (!(cin >> v)) {
                    cout << "Erro: Por favor, insira um valor numerico.\n";
                    cout << "Valor: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                raiz = inserir(raiz, v);
                cout << "Info: Valor " << v << " inserido.\n";
                break;
            case 2:
                cout << "Valor: ";
                // --- Validação do Valor (v) ---
                while (!(cin >> v)) {
                    cout << "Erro: Por favor, insira um valor numerico.\n";
                    cout << "Valor: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                raiz = remover(raiz, v);
                cout << "Info: Valor " << v << " removido.\n";
                break;
            case 3:
                if (raiz == nullptr) {
                    cout << "Arvore vazia." << endl;
                } else {
                    listar(raiz);
                    cout << endl;
                }
                break;
            case 0:
                cout << "Saindo..." << endl;
                break;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
                break;
        }
    } while (opc != 0);

    return 0;
}