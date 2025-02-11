#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>
#include <utility> // Para pair

using namespace std;

// Declaração da GrafoMatriz
struct GrafoMatriz {
    int **matriz;
    int capacidade;
    int n;
    bool direcionado;
    bool ponderado_arestas;

    GrafoMatriz(int tam = 10, bool dir = false, bool pond_arestas = false);
    void redimensionar();
    void novo_no();
    void nova_aresta(int u, int v, double peso = 1.0);
    void deleta_no(int id);
    void deleta_aresta(int u, int v);
    int menor_distancia(int origem, int destino);
    bool eh_completo() const;
};

// Declaração da GrafoLista
struct GrafoLista {
    vector<vector<pair<int, double>>> lista;
    int n;
    bool direcionado;
    bool ponderado_arestas;

    GrafoLista(bool dir = false, bool pond_arestas = false);
    void novo_no();
    void nova_aresta(int u, int v, double peso = 1.0);
    void deleta_no(int id);
    void deleta_aresta(int u, int v);
    int menor_distancia(int origem, int destino);
    bool eh_completo() const;
};

// Declarações das funções auxiliares (APENAS DECLARAÇÕES!)
int calcularGrauMaximo(const GrafoLista& grafo);
double maiorMenorDistancia(const GrafoLista& grafo);
void processarComandos(int argc, char* argv[]);

#endif // GRAFO_HPP