/* #include "grafo.hpp"
#include <iostream>

using namespace std;

int main() {
    GrafoMatriz grafoMatriz(10, false, true); // Grafo não direcionado e ponderado
    GrafoLista grafoLista(false, true);       // Grafo não direcionado e ponderado

    // Adicionando nós
    for (int i = 0; i < 5; i++) {
        grafoMatriz.novo_no();
        grafoLista.novo_no();
    }

    // Adicionando arestas com pesos
    grafoMatriz.nova_aresta(0, 1, 2);
    grafoMatriz.nova_aresta(1, 2, 3);
    grafoMatriz.nova_aresta(2, 3, 1);
    grafoMatriz.nova_aresta(3, 4, 4);
    grafoMatriz.nova_aresta(4, 0, 5);

    grafoLista.nova_aresta(0, 1, 2);
    grafoLista.nova_aresta(1, 2, 3);
    grafoLista.nova_aresta(2, 3, 1);
    grafoLista.nova_aresta(3, 4, 4);
    grafoLista.nova_aresta(4, 0, 5);

    // Teste da menor distância
    int origem = 0, destino = 3;
    int distanciaMatriz = grafoMatriz.menor_distancia(origem, destino);
    int distanciaLista = grafoLista.menor_distancia(origem, destino);
    cout << "Menor distancia (Matriz) entre " << origem << " e " << destino << ": " << distanciaMatriz << endl;
    cout << "Menor distancia (Lista) entre " << origem << " e " << destino << ": " << distanciaLista << endl;

    // Verificar se o grafo é completo
    cout << "GrafoMatriz e completo? " << (grafoMatriz.eh_completo() ? "Sim" : "Nao") << endl;
    cout << "GrafoLista e completo? " << (grafoLista.eh_completo() ? "Sim" : "Nao") << endl;

    return 0;
} */

#include "../include/grafo.hpp"

int main(int argc, char* argv[]) {
    processarComandos(argc, argv);
    return 0;
}