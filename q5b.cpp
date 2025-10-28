#include <iostream>
#include <string>
#include <limits>   
#include <algorithm> // Para std::max

using namespace std;

// 1. ESTRUTURA DO NÓ
// 'altura' para o balanceamento AVL
struct Pessoa {
    string nome;
    char sexo;
    int idade;
    float peso;
    Pessoa *esq, *dir;
    int altura; 
};

// --- FUNÇÕES AUXILIARES DA AVL ---

// Retorna a altura de um nó (0 se for nulo)
int altura(Pessoa* no) {
    if (no == nullptr)
        return 0;
    return no->altura;
}

// Calcula o Fator de Balanceamento (FB) de um nó
int fatorBalanceamento(Pessoa* no) {
    if (no == nullptr)
        return 0;
    return altura(no->esq) - altura(no->dir);
}

// Atualiza a altura de um nó com base em seus filhos
void atualizarAltura(Pessoa* no) {
    if (no != nullptr) {
        no->altura = 1 + max(altura(no->esq), altura(no->dir));
    }
}

// Cria um novo nó Pessoa (com altura inicial 1)
Pessoa* novoNo(string nome, char sexo, int idade, float peso) {
    Pessoa* p = new Pessoa;
    p->nome = nome;
    p->sexo = sexo;
    p->idade = idade;
    p->peso = peso;
    p->esq = nullptr;
    p->dir = nullptr;
    p->altura = 1; // Nó folha tem altura 1
    return p;
}

// --- ROTAÇÕES DA AVL ---

// Rotação Simples à Direita (Caso Esquerda-Esquerda)
Pessoa* rotDireita(Pessoa* y) {
    Pessoa* x = y->esq;
    Pessoa* T2 = x->dir;

    // Realiza a rotação
    x->dir = y;
    y->esq = T2;

    // Atualiza alturas (ordem importa: primeiro y, depois x)
    atualizarAltura(y);
    atualizarAltura(x);

    // Retorna a nova raiz da sub-árvore
    return x;
}

// Rotação Simples à Esquerda (Caso Direita-Direita)
Pessoa* rotEsquerda(Pessoa* x) {
    Pessoa* y = x->dir;
    Pessoa* T2 = y->esq;

    // Realiza a rotação
    y->esq = x;
    x->dir = T2;

    // Atualiza alturas (ordem importa: primeiro x, depois y)
    atualizarAltura(x);
    atualizarAltura(y);

    // Retorna a nova raiz da sub-árvore
    return y;
}

// --- FUNÇÕES PRINCIPAIS (F1, F2, F3, F4) ---

/**
 * F1: INSERÇÃO (AVL)
 */
Pessoa* inserir(Pessoa* no, string nome, char sexo, int idade, float peso) {
    // 1. Inserção padrão da BST
    if (no == nullptr)
        return novoNo(nome, sexo, idade, peso);

    if (nome < no->nome)
        no->esq = inserir(no->esq, nome, sexo, idade, peso);
    else if (nome > no->nome)
        no->dir = inserir(no->dir, nome, sexo, idade, peso);
    else {
        // Nomes iguais não são permitidos (ou atualize os dados)
        cout << "Erro: Pessoa com nome '" << nome << "' ja existe.\n";
        return no;
    }

    // 2. Atualiza a altura do nó ancestral
    atualizarAltura(no);

    // 3. Calcula o Fator de Balanceamento
    int fb = fatorBalanceamento(no);

    // 4. Rebalanceamento (4 casos)

    // Caso Esquerda-Esquerda (LL)
    if (fb > 1 && nome < no->esq->nome) {
        return rotDireita(no);
    }

    // Caso Direita-Direita (RR)
    if (fb < -1 && nome > no->dir->nome) {
        return rotEsquerda(no);
    }

    // Caso Esquerda-Direita (LR)
    if (fb > 1 && nome > no->esq->nome) {
        no->esq = rotEsquerda(no->esq);
        return rotDireita(no);
    }

    // Caso Direita-Esquerda (RL)
    if (fb < -1 && nome < no->dir->nome) {
        no->dir = rotDireita(no->dir);
        return rotEsquerda(no);
    }

    // Retorna o nó (potencialmente balanceado)
    return no;
}

// Função auxiliar para encontrar o menor nó (sucessor in-order)
Pessoa* encontrarMenor(Pessoa* no) {
    Pessoa* atual = no;
    while (atual->esq != nullptr)
        atual = atual->esq;
    return atual;
}

/**
 * F3: REMOÇÃO (AVL)
 */
Pessoa* remover(Pessoa* raiz, string nome) {
    // 1. Remoção padrão da BST
    if (raiz == nullptr) return raiz;

    if (nome < raiz->nome)
        raiz->esq = remover(raiz->esq, nome);
    else if (nome > raiz->nome)
        raiz->dir = remover(raiz->dir, nome);
    else {
        // Nó encontrado
        if (raiz->esq == nullptr || raiz->dir == nullptr) {
            Pessoa* temp = raiz->esq ? raiz->esq : raiz->dir;
            if (temp == nullptr) { // 0 filhos
                temp = raiz;
                raiz = nullptr;
            } else { // 1 filho
                *raiz = *temp; // Copia o conteúdo
            }
            delete temp;
        } else {
            // 2 filhos: Pega sucessor in-order (menor da sub-árvore direita)
            Pessoa* temp = encontrarMenor(raiz->dir);
            // Copia os dados do sucessor para este nó
            raiz->nome = temp->nome;
            raiz->sexo = temp->sexo;
            raiz->idade = temp->idade;
            raiz->peso = temp->peso;
            // Remove o sucessor
            raiz->dir = remover(raiz->dir, temp->nome);
        }
    }

    // Se a árvore ficou vazia (só tinha 1 nó)
    if (raiz == nullptr) return raiz;

    // 2. Atualiza a altura
    atualizarAltura(raiz);

    // 3. Calcula o Fator de Balanceamento
    int fb = fatorBalanceamento(raiz);

    // 4. Rebalanceamento (4 casos)

    // Caso Esquerda-Esquerda (LL)
    if (fb > 1 && fatorBalanceamento(raiz->esq) >= 0) {
        return rotDireita(raiz);
    }

    // Caso Esquerda-Direita (LR)
    if (fb > 1 && fatorBalanceamento(raiz->esq) < 0) {
        raiz->esq = rotEsquerda(raiz->esq);
        return rotDireita(raiz);
    }

    // Caso Direita-Direita (RR)
    if (fb < -1 && fatorBalanceamento(raiz->dir) <= 0) {
        return rotEsquerda(raiz);
    }

    // Caso Direita-Esquerda (RL)
    if (fb < -1 && fatorBalanceamento(raiz->dir) > 0) {
        raiz->dir = rotDireita(raiz->dir);
        return rotEsquerda(raiz);
    }

    return raiz;
}


/**
 * F2: LISTAGEM (Percurso In-Order)
 */
void listarInOrder(Pessoa* raiz) {
    if (raiz != nullptr) {
        listarInOrder(raiz->esq);
        cout << "  - Nome: " << raiz->nome
             << ", Sexo: " << raiz->sexo
             << ", Idade: " << raiz->idade
             << ", Peso: " << raiz->peso
             << ", (Altura: " << raiz->altura << ")" << endl;
        listarInOrder(raiz->dir);
    }
}

/**
 * F4: CONSULTA (Busca Padrão da BST)
 */
Pessoa* consultar(Pessoa* raiz, string nome) {
    if (raiz == nullptr || raiz->nome == nome) {
        return raiz;
    }

    if (nome < raiz->nome) {
        return consultar(raiz->esq, nome);
    }
    
    return consultar(raiz->dir, nome);
}


// --- FUNÇÕES DE VALIDAÇÃO DE ENTRADA ---

void limparBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int lerInt(string msg) {
    int valor;
    while (true) {
        cout << msg;
        if (cin >> valor) {
            limparBuffer(); 
            break;
        }
        cout << "Valor invalido! Digite um numero inteiro.\n";
        limparBuffer();
    }
    return valor;
}

float lerFloat(string msg) {
    float valor;
    while (true) {
        cout << msg;
        if (cin >> valor) {
            limparBuffer(); 
            break;
        }
        cout << "Valor invalido! Digite um numero.\n";
        limparBuffer();
    }
    return valor;
}

char lerSexo() {
    char sexo;
    while (true) {
        cout << "Sexo (M/F): ";
        cin >> sexo;
        limparBuffer(); 
        sexo = toupper(sexo); // Converte para maiúsculo
        if (sexo == 'M' || sexo == 'F')
            return sexo;
        cout << "Entrada invalida! Digite apenas M ou F.\n";
    }
}

string lerNome(string msg) {
    string nome;
    cout << msg;
    getline(cin, nome); // Pega a linha inteira (nomes compostos)
    return nome;
}


// --- 2. MENU DE OPÇÕES ---

int main() {
    Pessoa* RAIZ = nullptr;
    int op;
    string nome;

    do {
        cout << "\n--- MENU ARVORE AVL DE PESSOAS ---\n"
             << "F1: 1 - Inserir pessoa\n"
             << "F2: 2 - Listar todas (em ordem)\n"
             << "F3: 3 - Remover pessoa\n"
             << "F4: 4 - Consultar pessoa\n"
             << "0 - Sair\n"
             << "----------------------------------\n";
        op = lerInt("Opcao: "); 

        if (op == 1) { // F1: Inserir
            nome = lerNome("Nome: ");
            char sexo = lerSexo();
            int idade = lerInt("Idade: ");
            float peso = lerFloat("Peso: ");
            RAIZ = inserir(RAIZ, nome, sexo, idade, peso);
            cout << "Info: Pessoa inserida com sucesso.\n";
        }
        else if (op == 2) { // F2: Listar
            if (RAIZ == nullptr) {
                cout << "Arvore vazia.\n";
            } else {
                cout << "\nListando todas as pessoas (em ordem de nome):\n";
                listarInOrder(RAIZ);
            }
        }
        else if (op == 3) { // F3: Remover
            if (RAIZ == nullptr) {
                cout << "Arvore vazia. Nada para remover.\n";
            } else {
                nome = lerNome("Nome da pessoa a remover: ");
                Pessoa* p = consultar(RAIZ, nome); // Verifica se existe
                if (p != nullptr) {
                    RAIZ = remover(RAIZ, nome);
                    cout << "Info: Pessoa '" << nome << "' removida.\n";
                } else {
                    cout << "Erro: Pessoa '" << nome << "' nao encontrada.\n";
                }
            }
        }
        else if (op == 4) { // F4: Consultar
             if (RAIZ == nullptr) {
                cout << "Arvore vazia. Nada para consultar.\n";
            } else {
                nome = lerNome("Nome da pessoa a consultar: ");
                Pessoa* p = consultar(RAIZ, nome);
                if (p != nullptr) {
                    cout << "Pessoa encontrada:\n"
                         << "  - Nome: " << p->nome << "\n"
                         << "  - Sexo: " << p->sexo << "\n"
                         << "  - Idade: " << p->idade << "\n"
                         << "  - Peso: " << p->peso << "\n"
                         << "  - (Altura na Arvore: " << p->altura << ")\n";
                } else {
                    cout << "Erro: Pessoa '" << nome << "' nao encontrada.\n";
                }
            }
        }
        else if (op != 0) {
            cout << "Opcao invalida!\n";
        }

    } while (op != 0);

    cout << "\nEncerrando o programa...\n";
    return 0;
}