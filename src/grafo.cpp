#include "grafo.hpp"
#include <iostream>
#include <fstream>
#include <queue>
#include <climits>
#include <algorithm>
#include <cfloat> 

using namespace std;

// Implementações de GrafoMatriz
GrafoMatriz::GrafoMatriz(int tam, bool dir, bool pond_arestas) : 
    capacidade(tam), n(0), direcionado(dir), ponderado_arestas(pond_arestas) {
    matriz = new int*[capacidade];
    for (int i = 0; i < capacidade; i++) {
        matriz[i] = new int[capacidade];
        fill(matriz[i], matriz[i] + capacidade, 0);
    }
}

void GrafoMatriz::redimensionar() {
    int nova_capacidade = capacidade * 2;
    int **nova_matriz = new int*[nova_capacidade];
    for (int i = 0; i < nova_capacidade; i++) {
        nova_matriz[i] = new int[nova_capacidade];
        fill(nova_matriz[i], nova_matriz[i] + nova_capacidade, 0);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            nova_matriz[i][j] = matriz[i][j];
        }
        delete[] matriz[i];
    }
    delete[] matriz;
    matriz = nova_matriz;
    capacidade = nova_capacidade;
}

void GrafoMatriz::novo_no() {
    if (n == capacidade) redimensionar();
    n++;
}

void GrafoMatriz::nova_aresta(int u, int v, double peso) {
    cout << "Adicionando aresta: " << u << " -> " << v << " com peso " << peso << endl;
    if (u < n && v < n) {
        matriz[u][v] = peso;
        if (!direcionado) matriz[v][u] = peso;
    }
}

void GrafoMatriz::deleta_no(int id) {
    if (id >= n) return;
    for (int i = id; i < n - 1; i++) {
        for (int j = 0; j < n; j++) {
            matriz[i][j] = matriz[i + 1][j];
            matriz[j][i] = matriz[j][i + 1];
        }
    }
    n--;
}

void GrafoMatriz::deleta_aresta(int u, int v) {
    if (u < n && v < n) {
        matriz[u][v] = 0;
        if (!direcionado) matriz[v][u] = 0;
    }
}

int GrafoMatriz::menor_distancia(int origem, int destino) {
    vector<int> dist(n, INT_MAX);
    dist[origem] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, origem});
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        for (int v = 0; v < n; v++) {
            if (matriz[u][v] && dist[v] > dist[u] + matriz[u][v]) {
                dist[v] = dist[u] + matriz[u][v];
                pq.push({dist[v], v});
            }
        }
    }
    return (dist[destino] == INT_MAX) ? -1 : dist[destino];
}

bool GrafoMatriz::eh_completo() const {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (!matriz[i][j]) return false;
        }
    }
    return true;
}

// Implementações de GrafoLista
GrafoLista::GrafoLista(bool dir, bool pond_arestas) : 
    n(0), direcionado(dir), ponderado_arestas(pond_arestas) {}

void GrafoLista::novo_no() {
    lista.push_back(vector<pair<int, double>>()); // Corrigido para double
    n++;
}

void GrafoLista::nova_aresta(int u, int v, double peso) {
    cout << "Adicionando aresta: " << u << " -> " << v << " com peso " << peso << endl;
    if (u < n && v < n) {
        lista[u].push_back({v, peso});
        if (!direcionado) lista[v].push_back({u, peso});
    }
}

void GrafoLista::deleta_no(int id) {
    if (id >= lista.size()) return;

    // Atualiza os IDs dos nós nas arestas
    for (auto& vizinhos : lista) {
        auto it = vizinhos.begin();
        while (it != vizinhos.end()) {
            if (it->first == id) {
                it = vizinhos.erase(it); // Remove referências ao nó excluído
            } else {
                if (it->first > id) it->first--; // Reindexa os nós
                ++it;
            }
        }
    }

    // Remove o nó
    lista.erase(lista.begin() + id);

    // Atualiza a ordem
    n = lista.size();
}

void GrafoLista::deleta_aresta(int u, int v) {
    if (u < n && v < n) {
        // Corrigir o tipo para pair<int, double>
        lista[u].erase(remove_if(lista[u].begin(), lista[u].end(), [v](pair<int, double> p) { 
            return p.first == v; 
        }), lista[u].end());
        
        if (!direcionado) {
            lista[v].erase(remove_if(lista[v].begin(), lista[v].end(), [u](pair<int, double> p) { 
                return p.first == u; 
            }), lista[v].end());
        }
    }
}

int GrafoLista::menor_distancia(int origem, int destino) {
    if (origem >= n || destino >= n) return -1;

    vector<double> dist(n, DBL_MAX); // Alterado para DBL_MAX
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;

    dist[origem] = 0;
    pq.push({0, origem});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        for (auto& viz : lista[u]) {
            int v = viz.first;
            double peso = viz.second;

            if (dist[v] > dist[u] + peso) {
                dist[v] = dist[u] + peso;
                pq.push({dist[v], v});
            }
        }
    }

    return (dist[destino] == DBL_MAX) ? -1 : dist[destino];
}

bool GrafoLista::eh_completo() const {
    if (n == 0) return false;

    int total_nao_dir = (n * (n - 1)) / 2;
    int total_dir = n * (n - 1);
    int count = 0;

    for (const auto& vizinhos : lista) {
        count += vizinhos.size();
    }

    if (direcionado) {
        return count == total_dir;
    } else {
        return count == 2 * total_nao_dir;
    }
}

// Implementações das funções auxiliares
int calcularGrauMaximo(const GrafoLista& grafo) {
    int grauMaximo = 0;
    if (grafo.direcionado) {
        vector<int> grauSaida(grafo.lista.size(), 0);
        vector<int> grauEntrada(grafo.lista.size(), 0);

        for (int i = 0; i < grafo.lista.size(); i++) {
            grauSaida[i] = grafo.lista[i].size();
            for (auto& viz : grafo.lista[i]) {
                grauEntrada[viz.first]++;
            }
        }

        for (int i = 0; i < grafo.lista.size(); i++) {
            grauMaximo = max(grauMaximo, grauSaida[i] + grauEntrada[i]);
        }
    } else {
        for (const auto& vizinhos : grafo.lista) {
            grauMaximo = max(grauMaximo, (int)vizinhos.size());
        }
    }
    return grauMaximo;
}

//Algoritmo de Dijkstra é usado para menor distância entre dois nós em um grafo ponderado

double maiorMenorDistancia(const GrafoLista& grafo) {
    int n = grafo.lista.size();
    if (n == 0) return 0; // Grafo vazio

    double maiorDistancia = 0;
    for (int i = 0; i < n; i++) {
        vector<double> dist(n, INT_MAX);
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
        dist[i] = 0;
        pq.push({0, i});
        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            for (const auto& viz : grafo.lista[u]) {
                int v = viz.first;
                double peso = viz.second;
                if (dist[v] > dist[u] + peso) {
                    dist[v] = dist[u] + peso;
                    pq.push({dist[v], v});
                }
            }
        }
        for (int j = 0; j < n; j++) {
            if (dist[j] > maiorDistancia && dist[j] != INT_MAX) {
                maiorDistancia = dist[j];
            }
        }
    }
    return maiorDistancia;
}

void imprimirGrafo(const GrafoLista& grafo) {
    for (int i = 0; i < grafo.lista.size(); i++) {
        cout << "No " << i << ": ";
        for (auto viz : grafo.lista[i]) {
            cout << "(" << viz.first << ", " << viz.second << ") ";
        }
        cout << endl;
    }
}

void processarComandos(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Uso: " << argv[0] << " -d <-m/-l> arquivo.txt" << endl;
        exit(1);
    }

    bool usarMatriz = (string(argv[2]) == "-m");
    string arquivo = argv[3];

    ifstream input(arquivo);
    if (!input.is_open()) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        exit(1);
    }

    // Ler cabeçalho
    int direcionado, verticesPonderados, arestasPonderadas;
    input >> direcionado >> verticesPonderados >> arestasPonderadas;

    // Ler ordem (número de nós)
    int ordem;
    input >> ordem;

    // Pular linha dos pesos dos vértices
    string linha;
    getline(input, linha); // Pular a linha atual (10 20 30 40)
    getline(input, linha); // Pular linha em branco

    // Ler número de arestas
    int numArestas;
    input >> numArestas;

    // Criar grafo
    GrafoLista grafoLista(direcionado, arestasPonderadas);
    for (int i = 0; i < ordem; i++) {
        grafoLista.novo_no();
    }

   // Ler arestas
    for (int i = 0; i < numArestas; i++) {
    int u, v;
    double peso = 1.0; // Valor padrão para grafos não ponderados
    if (arestasPonderadas) {
        input >> u >> v >> peso;
    } else {
        input >> u >> v;
    }
    grafoLista.nova_aresta(u, v, peso);
    }

    input.close();

    cout << "Apos carregar o arquivo: " << endl;
    imprimirGrafo(grafoLista);

    // Excluir no 1
    if (grafoLista.lista.size() > 1) {
        cout << "Excluindo no 1..." << endl;
        grafoLista.deleta_no(1);
    } else {
        cerr << "Erro: Nao ha nos suficientes para excluir o no 1!" << endl;
        exit(1);
    }

    cout << "Apos excluir o no 1: " << endl;
    imprimirGrafo(grafoLista);

   // Excluir primeira aresta do nó 2 (agora ID 1)
    if (grafoLista.lista.size() > 1 && !grafoLista.lista[1].empty()) {
    cout << "Excluindo primeira aresta do no 2..." << endl;
    grafoLista.deleta_aresta(1, grafoLista.lista[1][0].first); // Novo ID 1
    } else {
    cerr << "Aviso: O no 2 nao tem arestas, pulando exclusao da aresta!" << endl;
    }

    cout << "Apos excluir a primeira aresta do no 2: " << endl;
    imprimirGrafo(grafoLista);

    // Verificar se o grafo está vazio
    if (grafoLista.lista.empty()) {
        cerr << "Erro: O grafo esta vazio apos as exclusoes!" << endl;
        exit(1);
    }

    // Imprimir propriedades
    cout << "Grau: " << calcularGrauMaximo(grafoLista) << endl;
    cout << "Ordem: " << grafoLista.lista.size() << endl;
    cout << "Direcionado: " << (direcionado ? "Sim" : "Nao") << endl;
    cout << "Vertices ponderados: " << (verticesPonderados ? "Sim" : "Nao") << endl;
    cout << "Arestas ponderadas: " << (arestasPonderadas ? "Sim" : "Nao") << endl;
    cout << "Completo: " << (grafoLista.eh_completo() ? "Sim" : "Nao") << endl;

    // Calcular maior menor distância
    double maiorDistancia = maiorMenorDistancia(grafoLista);
    cout << "Maior menor distancia: " << maiorDistancia << endl;
}