#include <iostream>
#include <fstream>   // Para leitura de arquivo (ifstream)
#include <string>
#include <iomanip>   // Para formatar a saída (setw)
#include <limits>    

#define vertex int 

using namespace std;

// --- DEFINIÇÃO DA CLASSE GRAPH ---
class Graph {
private:
    int V; // Número de vértices
    int A; // Número de arcos (arestas)
    // Método auxiliar para limpar a memória
    void limpar();

public:
    int **adj;  // Matriz de adjacência (0 ou 1)
    int **dist; // Matriz de pesos (distância)
    int *grau;  // Grau de SAÍDA (out-degree) de cada vértice

    // Métodos 
    Graph();
    ~Graph();

    // Funções implementadas
    void carregarDoArquivo(string nomeArquivo);
    void inserirArco(vertex u, vertex v, int w);
    void removerArco(vertex u, vertex v);
    void listarGrafo();
    void exibirMatrizes();
    void exibirGrau();
    void floydWarshall();
    bool estaVazio();
};

// --- IMPLEMENTAÇÃO DOS MÉTODOS ---

// Método construtor
Graph::Graph() {
    // Inicializa o grafo como vazio
    V = 0;
    A = 0;
    adj = nullptr;
    dist = nullptr;
    grau = nullptr;
}

// Método destrutor
Graph::~Graph() {
    limpar();
    cout << "\nInfo: Grafo destruido e memoria liberada." << endl;
}

// Método auxiliar para limpar a memória (usado pelo destrutor e carregar)
void Graph::limpar() {
    if (adj != nullptr) { // Se o grafo não estiver vazio
        for (int i = 0; i < V; i++) {
            delete[] adj[i];  // Deleta colunas da matriz adj
            delete[] dist[i]; // Deleta colunas da matriz dist
        }
        delete[] adj;  // Deleta array de linhas adj
        delete[] dist; // Deleta array de linhas dist
        delete[] grau; // Deleta array de graus
    }
    // Reseta os ponteiros e contadores
    V = 0;
    A = 0;
    adj = nullptr;
    dist = nullptr;
    grau = nullptr;
}

/**
 * Método auxiliar para checagem
 */
bool Graph::estaVazio() {
    if (adj == nullptr) {
        cout << "Erro: Nenhum grafo carregado. Use a Opcao 1 primeiro." << endl;
        return true;
    }
    return false;
}

/**
 * Observação 1: Lê o grafo de um arquivo "*.txt".
 */
void Graph::carregarDoArquivo(string nomeArquivo) {
    limpar(); // Limpa qualquer grafo existente antes de carregar um novo
    
    ifstream arq(nomeArquivo);
    if (!arq.is_open()) {
        cout << "Erro: Nao foi possivel abrir o arquivo '" << nomeArquivo << "'" << endl;
        return;
    }

    // Lê o número de vértices
    arq >> this->V;
    if (this->V <= 0) {
        cout << "Erro: Numero de vertices invalido no arquivo." << endl;
        return;
    }

    // Aloca memória para as matrizes e o array de grau
    adj = new int*[V];
    dist = new int*[V];
    grau = new int[V];

    for (int i = 0; i < V; i++) {
        adj[i] = new int[V];
        dist[i] = new int[V];
        grau[i] = 0; // Inicializa grau de saída como 0
        for (int j = 0; j < V; j++) {
            adj[i][j] = 0;  // Inicializa matrizes com 0
            dist[i][j] = 0;
        }
    }

    // Lê os arcos (u, v, w)
    vertex u, v;
    int w;
    this->A = 0; // Reseta contador de arcos
    while (arq >> u >> v >> w) {
        // Usa a função de inserção para preencher os dados
        inserirArco(u, v, w); 
    }

    arq.close();
    cout << "Info: Grafo carregado com " << V << " vertices e " << A << " arcos." << endl;
}

/**
 * Função: Inserção de arcos
 */
void Graph::inserirArco(vertex u, vertex v, int w) {
    if (u < 0 || u >= V || v < 0 || v >= V) {
        cout << "Erro: Vertice invalido (" << u << " ou " << v << ")." << endl;
        return;
    }

    // Se o arco ainda não existe, atualiza contadores
    if (adj[u][v] == 0) {
        this->A++;      // Incrementa número de arcos
        this->grau[u]++; // Incrementa grau de saída de 'u'
    }

    // Define a adjacência e o peso
    adj[u][v] = 1;
    dist[u][v] = w;
    
    // Não imprime msg se estiver sendo chamado pelo carregarDoArquivo
    // (A == 0 é um truque para saber se é a carga inicial, 
    // embora agora A seja incrementado antes, vamos manter a lógica de não poluir o log)
    if (A > 0 && !estaVazio()) { 
        cout << "Info: Arco " << u << " -> " << v << " (Peso: " << w << ") inserido/atualizado." << endl;
    }
}

/**
 * Função: Remoção de arcos
 */
void Graph::removerArco(vertex u, vertex v) {
    if (estaVazio()) return;
    if (u < 0 || u >= V || v < 0 || v >= V) {
        cout << "Erro: Vertice invalido." << endl;
        return;
    }

    if (adj[u][v] == 1) {
        adj[u][v] = 0;
        dist[u][v] = 0;
        this->A--;      // Decrementa número de arcos
        this->grau[u]--; // Decrementa grau de saída de 'u'
        cout << "Info: Arco " << u << " -> " << v << " removido." << endl;
    } else {
        cout << "Info: Arco " << u << " -> " << v << " nao existe." << endl;
    }
}

/**
 * Função: Listagem do grafo
 */
void Graph::listarGrafo() {
    if (estaVazio()) return;

    cout << "\n--- Listagem do Grafo (V=" << V << ", A=" << A << ") ---" << endl;
    for (int i = 0; i < V; i++) {
        cout << "Vertice " << i << " (Grau Saida: " << grau[i] << "):" << endl;
        bool temArco = false;
        for (int j = 0; j < V; j++) {
            if (adj[i][j] == 1) {
                cout << "  -> " << j << " (Peso: " << dist[i][j] << ")" << endl;
                temArco = true;
            }
        }
        if (!temArco) {
            cout << "  (Nenhum arco de saida)" << endl;
        }
    }
}

/**
 * Função: Exibição das matrizes de representação
 */
void Graph::exibirMatrizes() {
    if (estaVazio()) return;

    cout << "\n--- Matriz de Adjacencia (0/1) ---" << endl;
    cout << "     ";
    for (int j = 0; j < V; j++) cout << setw(4) << j;
    cout << "\n     ";
    for (int j = 0; j < V; j++) cout << "----";
    cout << endl;
    
    for (int i = 0; i < V; i++) {
        cout << setw(3) << i << " | ";
        for (int j = 0; j < V; j++) {
            cout << setw(3) << adj[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\n--- Matriz de Pesos (Distancia) ---" << endl;
    cout << "     ";
    for (int j = 0; j < V; j++) cout << setw(4) << j;
    cout << "\n     ";
    for (int j = 0; j < V; j++) cout << "----";
    cout << endl;
    
    for (int i = 0; i < V; i++) {
        cout << setw(3) << i << " | ";
        for (int j = 0; j < V; j++) {
            cout << setw(3) << dist[i][j] << " ";
        }
        cout << endl;
    }
}

/**
 * Função: Exibição do grau dos vértices
 */
void Graph::exibirGrau() {
    if (estaVazio()) return;

    cout << "\n--- Grau dos Vertices ---" << endl;
    for (int i = 0; i < V; i++) {
        // Calcula o grau de entrada (in-degree)
        int inDegree = 0;
        for (int j = 0; j < V; j++) {
            if (adj[j][i] == 1) { // Verifica a coluna 'i'
                inDegree++;
            }
        }
        cout << "Vertice " << i << ":"
             << " Grau de Saida (Out): " << this->grau[i]
             << " | Grau de Entrada (In): " << inDegree << endl;
    }
}

// ==================================================================
// =====           IMPLEMENTAÇÃO DE FLOYD-WARSHALL              =====
// ==================================================================

/**
 * Função: Executa o algoritmo de Floyd-Warshall para encontrar
 * o caminho mínimo entre todos os pares de vértices.
 */
void Graph::floydWarshall() {
    if (estaVazio()) return;

    // 1. Definir "Infinito"
    // Usamos o valor máximo de um 'int'.
    const int INF = numeric_limits<int>::max();

    // 2. Alocar e Inicializar a matriz de distâncias (fw_dist)
    // Esta é uma matriz temporária que conterá os caminhos mínimos.
    int **fw_dist = new int*[V];
    for (int i = 0; i < V; i++) {
        fw_dist[i] = new int[V];
        for (int j = 0; j < V; j++) {
            if (i == j) {
                fw_dist[i][j] = 0; // Distância para si mesmo é 0
            } else if (adj[i][j] == 1) {
                fw_dist[i][j] = dist[i][j]; // Distância do arco original
            } else {
                fw_dist[i][j] = INF; // Sem caminho direto conhecido
            }
        }
    }

    // 3. Executar o Algoritmo de Floyd-Warshall (O(V^3))
    // 'k' é o vértice intermediário
    for (int k = 0; k < V; k++) {
        // 'i' é o vértice de origem
        for (int i = 0; i < V; i++) {
            // 'j' é o vértice de destino
            for (int j = 0; j < V; j++) {
                
                // Checa se o caminho i -> k -> j existe
                // (Evita overflow de INF + INF)
                if (fw_dist[i][k] != INF && fw_dist[k][j] != INF) {
                    
                    // Calcula a nova distância passando por 'k'
                    int novaDistancia = fw_dist[i][k] + fw_dist[k][j];
                    
                    // Se o caminho por 'k' for menor, atualiza
                    if (novaDistancia < fw_dist[i][j]) {
                        fw_dist[i][j] = novaDistancia;
                    }
                }
            }
        }
    }

    // 4. Exibir a matriz resultante
    cout << "\n--- Matriz de Caminhos Minimos (Floyd-Warshall) ---" << endl;
    cout << " (INF = Sem caminho)" << endl;
    cout << "     ";
    // Header (colunas)
    for (int j = 0; j < V; j++) cout << setw(5) << j;
    cout << "\n    ";
    for (int j = 0; j < V; j++) cout << "-----";
    cout << endl;
    
    // Conteúdo da matriz
    for (int i = 0; i < V; i++) {
        cout << setw(3) << i << " | ";
        for (int j = 0; j < V; j++) {
            if (fw_dist[i][j] == INF) {
                cout << setw(4) << "INF" << " ";
            } else {
                cout << setw(4) << fw_dist[i][j] << " ";
            }
        }
        cout << endl;
    }
    
    // 5. [Opcional] Verificar ciclos negativos
    for (int i = 0; i < V; i++) {
        if (fw_dist[i][i] < 0) {
            cout << "\nAviso: Ciclo negativo detectado no grafo!" << endl;
            break; 
        }
    }

    // 6. Limpar a memória alocada para fw_dist
    for (int i = 0; i < V; i++) {
        delete[] fw_dist[i];
    }
    delete[] fw_dist;
}

// --- FUNÇÕES DE VALIDAÇÃO DE ENTRADA (MENU) ---

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

// --- FUNÇÃO PRINCIPAL (MENU) ---

int main() {
    Graph meuGrafo;
    int op;
    string nomeArquivo;
    vertex u, v;
    int w;

    do {
        cout << "\n--- MENU DE GERENCIAMENTO DE GRAFO ---\n"
             << "1. Carregar grafo do arquivo\n"
             << "2. Inserir Arco (Aresta)\n"
             << "3. Remover Arco (Aresta)\n"
             << "4. Listar Grafo (Lista de Adjacencia)\n"
             << "5. Exibir Matrizes (Adjacencia e Pesos)\n"
             << "6. Exibir Graus dos Vertices\n"
             << "7. Calcular Caminhos Minimos (Floyd-Warshall)\n"
             << "0. Sair\n"
             << "--------------------------------------\n";
        op = lerInt("Escolha uma opcao: ");

        switch (op) {
            case 1:
                cout << "Digite o nome do arquivo (ex: grafo.txt): ";
                cin >> nomeArquivo;
                limparBuffer();
                meuGrafo.carregarDoArquivo(nomeArquivo);
                break;
            case 2:
                if (meuGrafo.estaVazio()) break; 
                u = lerInt("Vertice de origem (u): ");
                v = lerInt("Vertice de destino (v): ");
                w = lerInt("Peso (w): ");
                meuGrafo.inserirArco(u, v, w);
                break;
            case 3:
                if (meuGrafo.estaVazio()) break;
                u = lerInt("Vertice de origem (u): ");
                v = lerInt("Vertice de destino (v): ");
                meuGrafo.removerArco(u, v);
                break;
            case 4:
                meuGrafo.listarGrafo();
                break;
            case 5:
                meuGrafo.exibirMatrizes();
                break;
            case 6:
                meuGrafo.exibirGrau();
                break;
            case 7: 
                meuGrafo.floydWarshall();
                break;
            case 0:
                break;
            default:
                cout << "Opcao invalida. Tente novamente." << endl;
                break;
        }

    } while (op != 0);

    return 0; 
}