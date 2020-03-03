//
// Created by esteban on 1/3/20.
//

#include <iostream>
#include <queue>
#include <vector>
#include <fstream>  // Libreria para archivos
#include <array>

#define MAXV 100            // Maxima cantidad de vertices.
#define oo 0x3f3f3f3f       // Nuestro valor infinito.
using namespace std;

//************************************************//
class Edge {
public:
    int node;            // El nodo destino de la arista.
    int cost;            // El costo de la arista.
    Edge(int, int);      // Constructor parametrizado.
    Edge();              // Constructor por defecto.
};

Edge :: Edge(int _node, int _cost){
    node = _node;
    cost = _cost;
}

Edge :: Edge(){
    node = -1;
    cost = -1;
}

//************************************************//

//************************************************//
class Graph {
public:
    vector<Edge> G[MAXV];       // Lista de adyacencias.
    int V{};                    // Cantidad de vertices.
    int E{};                    // Cantidad de aristas.
};

//************************************************//

//************************************************//

class State{
public:
    int node;                                    // El nodo actual.
    int cost;                                    // El costo del camino.
    State(int, int);                             // Constructor parametrizado.
    bool operator <(const State &b) const ;      // Sobrecarga del operador de prioridad <.
};

State :: State(int _node, int _cost){
    node = _node;
    cost = _cost;
}

bool State::operator<(const State &b) const {
    return cost > b.cost;
}

//************************************************//

//************************************************//

class Programa{
public:
    int V;
    int E;
    int comienzo, fin;
    void definirGrafo(Graph& graph, int, int);
    void cargarGrafo(Graph & graph, int, int, int);
    int algoritmo(int, int, const Graph&);
    void Dijkstra(const Graph& graph);
    static vector<string> split (string, string);
};

void Programa:: definirGrafo(Graph& graph, int _V, int _E){
    V = _V;
    E = _E;
    graph.V = V;
    cout << "Vertices: "<< graph.V << endl;

    graph.E = E;
    cout << "Aristas: "<< graph.E << endl;
}

void Programa :: cargarGrafo(Graph & graph, int orig, int dest, int pe){
    for (int i = 0; i < E; ++i){
        int Origen = orig, Destino = dest, Peso = pe;

        // Insertamos la arista dos veces, ya que nuestro grafo es un grafo no dirigido.
        graph.G[Origen].emplace_back(Destino, Peso);
        graph.G[Destino].emplace_back(Origen, Peso);
    }
}

int Programa :: algoritmo(const int begin, const int end, const Graph& graph){
    priority_queue<State> pq;                                               // La cola de prioridad.
    vector<int> Dist(graph.V, oo);                                          // La distancia hacia todos los vertices. Inicialmente para cada vertice su valor es infinito.
    vector<bool> mark(graph.V, false);                                // Este arreglo nos permitira determinar los nodos procesados.

    Dist[begin] = 0;                                                        // Valor inicial del vertice de partida.
    pq.push(State(begin, 0));                                      // Agregamos el primer elemento, que no es mas que el vertice de partida.
    while(!pq.empty()) {                                                   // Mientras existan vertices por procesar.
        State st = pq.top(); pq.pop();                                     // Se desencola el elemento minimo.
        mark[st.node] = true;                                              // Se marca el nodo como visitado.
        if (st.node == end)
            return st.cost;                                                // Retornamos el valor del camino, hemos llegado al vertice destino.

        int T = (int)graph.G[st.node].size();
        for(int i = 0; i < T; ++i) {                                       // Se recorren las adyacencias de "a".
            // Si no ha sido procesado el vertice "vi" y la distancia hacia "vi" es menor a la distancia
            // en Dist entonces hemos encontrado un camino mas corto a "vi".
            if (!mark[graph.G[st.node][i].node] && ((Dist[st.node] + graph.G[st.node][i].cost) < Dist[graph.G[st.node][i].node])){
                Dist[graph.G[st.node][i].node] = st.cost + graph.G[st.node][i].cost;
                pq.push(State(graph.G[st.node][i].node, st.cost + graph.G[st.node][i].cost));
            }
        }
    }
    return -1;
}

void Programa :: Dijkstra(const Graph& graph){
    cout << "Ingrese Vertice Inicial: " << endl;
    cin >> comienzo;
    cout << "Ingrese Vertice Final: " << endl;
    cin >> fin;
    int n = algoritmo(comienzo, fin, graph);  //Accediendo al algoritmo de Dijkstra
    if (n > 0){
        cout << "El valor de la ruta es: " + to_string(n) << endl;
    } else {
        cout << "No se pudo encontrar la ruta porque el vertice ingresado no existe o no hay ruta entre vertices" << endl;
        cout << "Intente de nuevo!" << endl << endl;
    }
}

// for string delimiter
vector<string> Programa :: split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

void lecturaArchivo(Graph& graph){
    ifstream archivo;
    string texto;
    string delimitador = "-";
    archivo.open("grafo.txt", ios::in);    //Leyendo el archivo

    if (archivo.fail()){
        cout << "No se pudo abrir el archivo";
        exit(1);   // Salir del programa
    }

    Programa programa1{};
    int valor = 0;
    while(!archivo.eof()){    //Mientras no se alcance el final del archivo

        getline(archivo, texto);
        string delimiter = " - ";
        string datos[3];

        vector<string> v = Programa :: split(texto, delimiter);

        // Se definen los vertices y aristas
        if(valor == 0) {
            int j = 0;
            for (auto i : v) {
                //cout << " v: " << i << endl;
                datos[j] = i;
                j++;
            }
            valor++;
            programa1.definirGrafo(graph, stoi(datos[0]), stoi(datos[1]));
            cout << endl;
        }

        //Se crean las rutas entre vertices
        else {
            int j = 0;
            for (auto i : v) {
                //cout << " v2: " << i << endl;
                datos[j] = i;
                j++;
            }
            if(archivo.eof()){
                break;
            } else {
                programa1.cargarGrafo(graph, stoi(datos[0]), stoi(datos[1]), stoi(datos[2]));
                cout << "Origen: " << datos[0] << "     Destino: " << datos[1] << "      Peso: " << datos[2] << endl;

                cout << endl;
            }
        }
    }

    archivo.close();
}


void escribirArchivo(){
    ofstream archivo;
    archivo.open("prueba.txt", ios::out);  //Abriendo el archivo
    if (archivo.fail()){
        cout << "No se pudo abrir el archivo";
        exit(1);   // Salir del programa
    }
    archivo << "Hola que tal";  // Escribir en el archivo
    archivo.close();
}




//************************************************//


int main(){
    bool out = false;
    char salir;

    Programa programa{}; //TAD
    Graph graph; // Grafo.

    cout << "Algoritmo de Dijkstra en C++" << endl;

    while (!out){
        lecturaArchivo(graph);
        programa.Dijkstra(graph); //Se aplica el algoritmo de Dijkstra

        //Desea Salir?
        cout << "Salir? (S/N)" << endl;
        cin >> salir;
        if (salir == 'S'){
            out = true;
        }
    }
}
