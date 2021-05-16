#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <Imagine/Graphics.h>
using namespace Imagine;
#include <random>
#include <algorithm>    // std::random_shuffle
#include <math.h>
struct Graph{
    bool* distrib;
    int* distance;
    int nb_distrib;
    int nb_terminaux;
    int* tab_distrib;//tableau de longueur nb_distrib et qui contient les indices des distributeur dans nodes
    int N; //nombre total de sommets dans le graphe
    vector<double> coord_geo_x ,coord_geo_y;
    Graph(string file_path_distance,string file_path_nodes);
    ~Graph();
    float distance_norme2(int i,int j)const;
};
struct Point2D{
    int _x;
    int _y;
    Point2D(int x0, int y0) : _x(x0), _y(y0) {}
};

struct Cluster{
    int distribution;
    vector<Point2D > antennes; // antenne et distance a la distribution;
    void ajoutAntenne(int antenne, int dist);
    Cluster(int distribution0);
    void affiche_Cluster();
};

struct Reseau{
    vector<int> boucleStructurante; // mettre en premier le sommet point de dist
    vector<vector<vector<int>>> chainesCollecte; //premier sommet de chaque chaine = celui de la boucle
    vector<int> les_noeuds;
    Reseau(int pointDist) ;
    Reseau(){int a;};
    void affichage_graphique_Reseau(const Graph &graph);
};
double maximum_vector(vector<double> v);
double minimum_vector(vector<double> v);

int calcul_cost_total_reseau(const Reseau &reseau,const Graph &graphe) ;
int calcul_cost_total_architecture(const vector<Reseau> &a,const Graph &graphe) ;
Reseau creationReseau(const Cluster &cluster,const Graph &graphe);
vector<int> modification_boucle_structurante(Reseau &r,const Cluster &cluster,const Graph &graphe);
void creation_chaines(Reseau &r,vector<int> &pile,const Cluster &cluster,const Graph &graphe);
void creation_chaines_opti(Reseau &r,vector<int> &pile,const Cluster &cluster,const Graph &graphe);
vector<Reseau> creation_chaines_global(vector<Reseau> Architecture,const Graph &graphe);
vector<Reseau> heuristique(const Graph &graphe,vector<Cluster> &clusters);
void affichage_graphique_Architecture(const vector<Reseau> &Architecture,const Graph &graphe);
