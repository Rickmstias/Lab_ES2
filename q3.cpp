#include <iostream>
#include <string>
#include <list>
#include <limits> 
using namespace std;

// Estrutura da pessoa (nó da árvore)
struct Pessoa {
    string nome;
    char sexo;
    int idade;
    float peso;
    Pessoa *esq, *dir;
};

// Insere pessoa na árvore (ordenada por nome)
Pessoa* inserir(Pessoa* raiz, string nome, char sexo, int idade, float peso) {
    if (!raiz) return new Pessoa{nome, sexo, idade, peso, nullptr, nullptr};

    if (nome < raiz->nome)
        raiz->esq = inserir(raiz->esq, nome, sexo, idade, peso);
    else if (nome > raiz->nome)
        raiz->dir = inserir(raiz->dir, nome, sexo, idade, peso);
    return raiz;
}

// Percorre a árvore e separa em listas por sexo
void gerarListas(Pessoa* raiz, list<Pessoa*>& homens, list<Pessoa*>& mulheres) {
    if (!raiz) return;
    gerarListas(raiz->esq, homens, mulheres);

    if (raiz->sexo == 'M' || raiz->sexo == 'm')
        homens.push_back(raiz);
    else if (raiz->sexo == 'F' || raiz->sexo == 'f')
        mulheres.push_back(raiz);

    gerarListas(raiz->dir, homens, mulheres);
}

// Mostra o conteúdo de uma lista
void imprimirLista(list<Pessoa*>& lst, string titulo) {
    cout << "\n" << titulo << ":\n";
    if (lst.empty()) {
        cout << "(lista vazia)\n";
        return;
    }
    for (auto p : lst)
        cout << "Nome: " << p->nome
             << ", Sexo: " << p->sexo
             << ", Idade: " << p->idade
             << ", Peso: " << p->peso << endl;
}

// Função para evitar loop infinito
void limparBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Função que lê um número inteiro
int lerInt(string msg) {
    int valor;
    while (true) {
        cout << msg;
        if (cin >> valor) break;
        cout << "Valor invalido! Digite um numero inteiro.\n";
        limparBuffer();
    }
    limparBuffer();
    return valor;
}

// Função que lê um número decimal 
float lerFloat(string msg) {
    float valor;
    while (true) {
        cout << msg;
        if (cin >> valor) break;
        cout << "Valor invalido! Digite um numero.\n";
        limparBuffer();
    }
    limparBuffer();
    return valor;
}

// Função que lê o sexo validando apenas M/F (maiúsculo ou minúsculo)
char lerSexo() {
    char sexo;
    while (true) {
        cout << "Sexo (M/F): ";
        cin >> sexo;
        limparBuffer();
        if (sexo == 'M' || sexo == 'm' || sexo == 'F' || sexo == 'f')
            return sexo;
        cout << "Entrada invalida! Digite apenas M ou F.\n";
    }
}

int main() {
    Pessoa* RAIZ = nullptr;
    int op;
    string nome;

    do {
        cout << "\n1 - Inserir pessoa\n2 - Gerar e listar por sexo\n0 - Sair\nOpcao: ";
        cin >> op;
        limparBuffer();

        if (op == 1) { // inserir nova pessoa
            cout << "Nome: ";
            getline(cin, nome);
            char sexo = lerSexo();
            int idade = lerInt("Idade: ");
            float peso = lerFloat("Peso: ");
            RAIZ = inserir(RAIZ, nome, sexo, idade, peso);
        }
        else if (op == 2) { // gerar listas e exibir
            list<Pessoa*> homens, mulheres;
            gerarListas(RAIZ, homens, mulheres);
            imprimirLista(homens, "Lista de Homens");
            imprimirLista(mulheres, "Lista de Mulheres");
        }
        else if (op != 0) {
            cout << "Opcoo invalida!\n";
        }

    } while (op != 0);

    cout << "\nEncerrando o programa...\n";
    return 0;
}