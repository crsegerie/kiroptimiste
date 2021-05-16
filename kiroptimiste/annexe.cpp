#include "annexe.h"

Graph::Graph(string file_path_distance,string file_path_nodes){
    ifstream file(file_path_distance.c_str(),ios::in);
    if(file) {
        string letter;
        int number;
        int i=1;
        bool condition=true;
        file>>number;
        file>>number;
        //une premiere boucle juste pour trouver le nombre ville
        while(condition) {
            file>>number;
            if (number==0){condition=false;}
            i++;
        }
        N=i-1;
        cout<<N<<endl;
        file.close();
    }
    else{
        cout<<"Error while loading file to read"<<endl;
    }
    distance=new int[(N*N)+1];
    ifstream file2(file_path_distance.c_str(),ios::in);
    if(file2) {
        int number;
        for(int i=1;i<(N*N)+1;i++){
            file2>>number;
            distance[i]=number;
        }
        //cout<<"sa doit faire zero"<<distance[1+((N-1)*N)+N-1]<<endl;
        file2.close();
    }
    else{
        cout<<"Error while loading file to read"<<endl;
    }
    nb_distrib=0;
    nb_terminaux=0;
    distrib=new bool[N];
    tab_distrib=new int[N];//on le creer de taille N mais on utilisera que les nb_distri premiere cases

    ifstream file3(file_path_nodes.c_str(),ios::in);
    if(file3) {
        long double number_x;
        long double number_y;
        string letter;
        for(int i=0;i<N;i++){
            file3>>number_x>>number_y>>letter;
            coord_geo_x.push_back(number_x);
            coord_geo_y.push_back(number_y);
            if(letter=="distribution"){
                tab_distrib[nb_distrib]=i;
                nb_distrib++;
                distrib[i]=true;
            }
            else{nb_terminaux++;distrib[i]=false;}

        }
        file3.close();
    }
    else{
        cout<<"Error while loading file to read"<<endl;
    }
}
Graph::~Graph(){
    delete[] distrib;
    delete[] distance;
    delete[] tab_distrib;
}
float Graph:: distance_norme2(int i,int j) const{
    float d_carre=(coord_geo_x[i]-coord_geo_x[j])*(coord_geo_x[i]-coord_geo_x[j]);
    d_carre+=(coord_geo_y[i]-coord_geo_y[j])*(coord_geo_y[i]-coord_geo_y[j]);
    return sqrt(d_carre);
}

void Cluster::ajoutAntenne(int antenne, int dist){
    Point2D newAnt(antenne, dist);
    antennes.push_back(newAnt);
}

Cluster::Cluster(int distribution0){
    distribution = distribution0;

}
void Cluster::affiche_Cluster(){
    cout<<"distributeur numero "<<distribution<<endl;
    for(int numero_antenne=0;numero_antenne<antennes.size();numero_antenne++){
        cout<<antennes[numero_antenne]._x<<endl;
    }
    cout<<"fin affichage Cluster"<<endl;
}
Reseau::Reseau(int pointDist){
    boucleStructurante.push_back(pointDist);
}
double maximum_vector(vector<double> v){
    double max=v[0];
    for(int i=0;i<v.size();i++){
        if (v[i]>max){
            max=v[i];
        }
    }
    return max;
}
double minimum_vector(vector<double> v){
    double min=v[0];
    for(int i=0;i<v.size();i++){
        if (v[i]<min){
            min=v[i];
        }
    }
    return min;
}
void Reseau::affichage_graphique_Reseau(const Graph &graphe){
    Window W=openWindow(1010,1010);
    double x_max=maximum_vector(graphe.coord_geo_x);
    double y_max=maximum_vector(graphe.coord_geo_y);
    double x_min=minimum_vector(graphe.coord_geo_x);
    double y_min=minimum_vector(graphe.coord_geo_y);

    double a_x=(1000-10)/(x_max-x_min);
    double b_x=10-(a_x*x_min);
    double a_y=(1000-10)/(y_max-y_min);
    double b_y=10-(a_y*y_min);
//    for (int i=0;i<graphe.N-1;i++){
//        double x_brut=graphe.coord_geo_x[i];
//        double y_brut=graphe.coord_geo_y[i];
//        double x=(a_x*x_brut)+b_x;
//        double y=(a_y*y_brut)+b_y;
//        Color C;
//        if (graphe.distrib[i]==true){C=RED;}
//        else{C=BLACK;}
//        //std::string s = std::to_string(i);
//        //drawString((int)x,(int)y,s,C,8);
//        drawRect((int)x,(int)y,5,5,C);
//    }

    for (int i=0;i<les_noeuds.size();i++){
        double x_brut=graphe.coord_geo_x[les_noeuds[i]];
        double y_brut=graphe.coord_geo_y[les_noeuds[i]];
        double x=(a_x*x_brut)+b_x;
        double y=(a_y*y_brut)+b_y;
        Color C;
        if (graphe.distrib[les_noeuds[i]]==true){C=RED;}
        else{C=BLACK;}
        std::string s = std::to_string(les_noeuds[i]);
        drawString((int)x,(int)y,s,C,8);
        drawRect((int)x,(int)y,5,5,C);
    }

    double x0_brut=graphe.coord_geo_x[boucleStructurante[0]];
    double y0_brut=graphe.coord_geo_y[boucleStructurante[0]];
    double x0=(a_x*x0_brut)+b_x;
    double y0=(a_y*y0_brut)+b_y;
    double x0_memoire=x0;
    double y0_memoire=y0;
    double x,y;
    for (int i=1;i<boucleStructurante.size();i++){
        double x_brut=graphe.coord_geo_x[boucleStructurante[i]];
        double y_brut=graphe.coord_geo_y[boucleStructurante[i]];
        x=(a_x*x_brut)+b_x;
        y=(a_y*y_brut)+b_y;
        drawLine((int)x0,(int)y0,(int)x,(int)y,RED);
        x0=x;
        y0=y;
    }
    if(1<boucleStructurante.size()){drawLine((int)x,(int)y,(int)x0_memoire,(int)y0_memoire,RED);}
    //faut encore ecrire laffichage des  chaines de collectes...
    for (int i=0;i<chainesCollecte.size();i++){
        vector<vector<int>> chaine_de_chaine=chainesCollecte[i];
        for(int j=0;j<chaine_de_chaine.size();j++){
            vector<int> une_chaine=chaine_de_chaine[j];
            for(int z=0;z<une_chaine.size()-1;z++){
                double x1_brut=graphe.coord_geo_x[une_chaine[z]];
                double y1_brut=graphe.coord_geo_y[une_chaine[z]];
                double x1=(a_x*x1_brut)+b_x;
                double y1=(a_y*y1_brut)+b_y;
                double x2_brut=graphe.coord_geo_x[une_chaine[z+1]];
                double y2_brut=graphe.coord_geo_y[une_chaine[z+1]];
                double x2=(a_x*x2_brut)+b_x;
                double y2=(a_y*y2_brut)+b_y;
                drawLine((int)x1,(int)y1,(int)x2,(int)y2,BLACK);
            }
        }

    }
    click();
    closeWindow(W);
}
void affichage_graphique_Architecture(const vector<Reseau> &Architecture,const Graph &graphe){
    Window W=openWindow(2010,2010);
    double x_max=maximum_vector(graphe.coord_geo_x);
    double y_max=maximum_vector(graphe.coord_geo_y);
    double x_min=minimum_vector(graphe.coord_geo_x);
    double y_min=minimum_vector(graphe.coord_geo_y);

    double a_x=(2000-10)/(x_max-x_min);
    double b_x=10-(a_x*x_min);
    double a_y=(2000-10)/(y_max-y_min);
    double b_y=10-(a_y*y_min);
    for (int i=0;i<graphe.N;i++){
        double x_brut=graphe.coord_geo_x[i];
        double y_brut=graphe.coord_geo_y[i];
        double x=(a_x*x_brut)+b_x;
        double y=(a_y*y_brut)+b_y;
        Color C;
        if (graphe.distrib[i]==true){C=RED;}
        else{C=BLACK;}
        //std::string s = std::to_string(i);
        //drawString((int)x,(int)y,s,C,8);
        //drawRect((int)x,(int)y,5,5,C);
        std::string s = std::to_string(i);
        drawString((int)x,(int)y,s,C,8);
    }

//    for (int i=0;i<les_noeuds.size();i++){
//        double x_brut=graphe.coord_geo_x[les_noeuds[i]];
//        double y_brut=graphe.coord_geo_y[les_noeuds[i]];
//        double x=(a_x*x_brut)+b_x;
//        double y=(a_y*y_brut)+b_y;
//        Color C;
//        if (graphe.distrib[les_noeuds[i]]==true){C=RED;}
//        else{C=BLACK;}
//        std::string s = std::to_string(les_noeuds[i]);
//        drawString((int)x,(int)y,s,C,8);
//        drawRect((int)x,(int)y,5,5,C);
//    }
    for(int nbr_reseau=0;nbr_reseau<Architecture.size();nbr_reseau++){
        vector<int> boucleStructurante=Architecture[nbr_reseau].boucleStructurante;
        vector<vector<vector<int>>> chainesCollecte=Architecture[nbr_reseau].chainesCollecte;
        double x0_brut=graphe.coord_geo_x[boucleStructurante[0]];
        double y0_brut=graphe.coord_geo_y[boucleStructurante[0]];
        double x0=(a_x*x0_brut)+b_x;
        double y0=(a_y*y0_brut)+b_y;
        double x0_memoire=x0;
        double y0_memoire=y0;
        double x,y;
        for (int i=1;i<boucleStructurante.size();i++){
            double x_brut=graphe.coord_geo_x[boucleStructurante[i]];
            double y_brut=graphe.coord_geo_y[boucleStructurante[i]];
            x=(a_x*x_brut)+b_x;
            y=(a_y*y_brut)+b_y;
            drawLine((int)x0,(int)y0,(int)x,(int)y,RED);
            x0=x;
            y0=y;
        }
        if(1<boucleStructurante.size()){drawLine((int)x,(int)y,(int)x0_memoire,(int)y0_memoire,RED);}
        //faut encore ecrire laffichage des  chaines de collectes...
        for (int i=0;i<chainesCollecte.size();i++){
            vector<vector<int>> chaine_de_chaine=chainesCollecte[i];
            for(int j=0;j<chaine_de_chaine.size();j++){
                vector<int> une_chaine=chaine_de_chaine[j];
                for(int z=0;z<une_chaine.size()-1;z++){
                    double x1_brut=graphe.coord_geo_x[une_chaine[z]];
                    double y1_brut=graphe.coord_geo_y[une_chaine[z]];
                    double x1=(a_x*x1_brut)+b_x;
                    double y1=(a_y*y1_brut)+b_y;
                    double x2_brut=graphe.coord_geo_x[une_chaine[z+1]];
                    double y2_brut=graphe.coord_geo_y[une_chaine[z+1]];
                    double x2=(a_x*x2_brut)+b_x;
                    double y2=(a_y*y2_brut)+b_y;
                    drawLine((int)x1,(int)y1,(int)x2,(int)y2,BLACK);
                }
            }

        }
    }

    click();
    closeWindow(W);
}
int nbr_aleatoire_entre_a_inclu_et_b_exclu(int a,int b){
    return a+rand()%(b-a);
}
vector<int> creation_boucle_structurante_aleatoire(Reseau &r,const Cluster &cluster,const Graph &graphe){
    //ici le tableau r.boucleStructurante contient deja l'int de la distribution
    r.les_noeuds.push_back(cluster.distribution);
    int sizeCluster = cluster.antennes.size();//exemple: si cluster contient le distributeur 0 et l'antenne 2, sizeCluster=1
    //int taille_boucle_structurante=nbr_aleatoire_entre_a_inclu_et_b_exclu(1,min(31,sizeCluster+2));//la boucle structurante est de taille 1 car elle contient au moins le distributeur
    int taille_boucle_structurante=nbr_aleatoire_entre_a_inclu_et_b_exclu(1,min(7,sizeCluster+2));//la boucle structurante est de taille 1 car elle contient au moins le distributeur
    vector<int> pile;
    for(int numero_antenne=0;numero_antenne<cluster.antennes.size();numero_antenne++){
        pile.push_back(cluster.antennes[numero_antenne]._x);
        r.les_noeuds.push_back(cluster.antennes[numero_antenne]._x);
    }
    random_shuffle(pile.begin(),pile.end() );
    for(int i=1;i<taille_boucle_structurante;i++){//si taille boucleStructurante vaut 2, on ne rajoute qu'une antenne
        r.boucleStructurante.push_back(pile[i-1]);
        //cout<<"dans boucle structurante"<<pile[i-1]<<endl;
    }
    vector<int> pile_des_antennes_qui_restent_a_placer;
    for(int i=taille_boucle_structurante;i<pile.size()+1;i++){
        pile_des_antennes_qui_restent_a_placer.push_back(pile[i-1]);
        //cout<<"hors boucle structurante"<<pile[i-1]<<endl;
    }
    return pile_des_antennes_qui_restent_a_placer;
}
vector<int> creation_boucle_structurante_aleatoire_optimise(Reseau &r,const Cluster &cluster,const Graph &graphe){
    //ici le tableau r.boucleStructurante contient deja l'int de la distribution
    r.les_noeuds.push_back(cluster.distribution);
    int sizeCluster = cluster.antennes.size();//exemple: si cluster contient le distributeur 0 et l'antenne 2, sizeCluster=1
    int taille_boucle_structurante=nbr_aleatoire_entre_a_inclu_et_b_exclu(1,min(31,sizeCluster+2));//la boucle structurante est de taille 1 car elle contient au moins le distributeur
    vector<int> pile;
    for(int numero_antenne=0;numero_antenne<cluster.antennes.size();numero_antenne++){
        pile.push_back(cluster.antennes[numero_antenne]._x);
        r.les_noeuds.push_back(cluster.antennes[numero_antenne]._x);
    }
    random_shuffle(pile.begin(),pile.end() );
    for(int i=1;i<taille_boucle_structurante;i++){//si taille boucleStructurante vaut 2, on ne rajoute qu'une antenne
        r.boucleStructurante.push_back(pile[i-1]);
        //cout<<"dans boucle structurante"<<pile[i-1]<<endl;
    }
    vector<int> pile_des_antennes_qui_restent_a_placer;
    for(int i=taille_boucle_structurante;i<pile.size()+1;i++){
        pile_des_antennes_qui_restent_a_placer.push_back(pile[i-1]);
        //cout<<"hors boucle structurante"<<pile[i-1]<<endl;
    }
    return pile_des_antennes_qui_restent_a_placer;
}
vector<int> modification_boucle_structurante(Reseau &r,const Cluster &cluster,const Graph &graphe){
    vector<int> boucle_structurante_initiale=r.boucleStructurante;
    vector<int> pile_des_antennes_a_placer;
    int que_faire=nbr_aleatoire_entre_a_inclu_et_b_exclu(0,3);
    //deja, il faut construire la pile des antennes à placer
    for(int i=0;i<cluster.antennes.size();i++){
        int a=cluster.antennes[i]._x;
        bool est_deja_dedans=false;
        for(int j=0;j<boucle_structurante_initiale.size();j++){
            if(boucle_structurante_initiale[j]==a){
                est_deja_dedans=true;
                j=boucle_structurante_initiale.size();
            }
        }
        if(est_deja_dedans==false){
            pile_des_antennes_a_placer.push_back(a);
        }
    }
    //ok,maintenant pile est construit.
    //soit pile est vide, soit pile non vide

    //soit on supprime un element de la boucle structurante si la boucle structurante est de taille>1

    //soit on ajoute un element si pile non vide
    //cout<<"que_faire"<<que_faire<<endl;
    if(que_faire==0){
        bool ajoute=(pile_des_antennes_a_placer.size()>1)&&(boucle_structurante_initiale.size()<29);
        if(ajoute){
            //cout<<"ajoute"<<endl;
            //cout<<"ssize"<<boucle_structurante_initiale.size()<<endl;
            int indice_sur_la_boucle=nbr_aleatoire_entre_a_inclu_et_b_exclu(1,boucle_structurante_initiale.size()+1);
            //cout<<"apres indice boucle"<<endl;
            int indice_hors_boucle=nbr_aleatoire_entre_a_inclu_et_b_exclu(0,pile_des_antennes_a_placer.size());
            //cout<<"size"<<pile_des_antennes_a_placer.size()<<"indice"<<indice_hors_boucle<<endl;
            int valeur_hors_boucle=pile_des_antennes_a_placer[indice_hors_boucle];
            //cout<<"avant insert"<<endl;
            boucle_structurante_initiale.insert(boucle_structurante_initiale.begin()+indice_sur_la_boucle,1,valeur_hors_boucle);
            //cout<<"aes insert"<<endl;
            pile_des_antennes_a_placer.erase(pile_des_antennes_a_placer.begin()+indice_hors_boucle);
            r.boucleStructurante=boucle_structurante_initiale;

        }
    }

    //soit echange deux elements
    if(que_faire==1){
        bool echange;
        echange=(pile_des_antennes_a_placer.size()>0)&&(boucle_structurante_initiale.size()>1);
        //cout<<"pile des antenne a placer.size="<<pile_des_antennes_a_placer.size()<<endl;
        if(echange){
            int indice_sur_la_boucle=nbr_aleatoire_entre_a_inclu_et_b_exclu(1,boucle_structurante_initiale.size());
            int indice_hors_boucle=nbr_aleatoire_entre_a_inclu_et_b_exclu(0,pile_des_antennes_a_placer.size());
            int valeur_sur_la_boucle=boucle_structurante_initiale[indice_sur_la_boucle];
            int valeur_hors_boucle=pile_des_antennes_a_placer[indice_hors_boucle];
            pile_des_antennes_a_placer[indice_hors_boucle]=valeur_sur_la_boucle;
            boucle_structurante_initiale[indice_sur_la_boucle]=valeur_hors_boucle;
            r.boucleStructurante=boucle_structurante_initiale;
        }
    }

    //soit on supprime un element de la boucle structurante
    if(que_faire==2){
        bool supprime=(boucle_structurante_initiale.size()>1);
        if(supprime){
            int indice_sur_la_boucle=nbr_aleatoire_entre_a_inclu_et_b_exclu(1,boucle_structurante_initiale.size());
            int valeur_sur_la_boucle=boucle_structurante_initiale[indice_sur_la_boucle];
            boucle_structurante_initiale.erase(boucle_structurante_initiale.begin()+indice_sur_la_boucle);
            pile_des_antennes_a_placer.push_back(valeur_sur_la_boucle);
            r.boucleStructurante=boucle_structurante_initiale;
        }
    }
    return pile_des_antennes_a_placer;
}
void creation_chaines(Reseau &r,vector<int> &pile,const Cluster &cluster,const Graph &graphe){
    //pile=pile des elements qui restent à placer dans ce cluster
    //je prends les elements un à un dans la pile, et je trouve le minimum, soit ce min est directement sur la chaine structurante, soit il est sur une chaine de collecte
    vector<vector<vector<int>>> a;
    r.chainesCollecte=a;
    for(int j=0;j<r.boucleStructurante.size();j++){
        vector<vector<int>> une_chaine_de_chaine;
        r.chainesCollecte.push_back(une_chaine_de_chaine);
    }
    while(pile.size()>0){
        int distance_min_boucle=graphe.distance[1+ (graphe.N*r.boucleStructurante[0])+pile[0]];
        int i_min=0;//antenne à placer
        int j_min=0;//antenne sur la boucle structurante qui realise le min
        for(int i=0;i<pile.size();i++){
            for(int j=0;j<r.boucleStructurante.size();j++){
                int distance=graphe.distance[1+ (graphe.N*r.boucleStructurante[j])+pile[i]];
                if(distance<distance_min_boucle){
                    distance_min_boucle=distance;
                    i_min=i;
                    j_min=j;
                }
            }
        }

        int distance_min_chaine=distance_min_boucle;// rmq on n'a pas besoin de verifier K_min est sur une chaine de taille <5...
        int I_min=0;//antenne à placer de la distance min
        int J_min=0;//antenne sur la boucle associée à la chaine qui realise le min
        int K_min=0;
        int Z_min=0;//numero de l'antenne sur la chaine de collecte
        for(int i=0;i<pile.size();i++){
            for(int j=0;j<r.boucleStructurante.size();j++){
                vector<vector<int>> une_chaine_de_chaine=r.chainesCollecte[j];
                for(int k=0;k<une_chaine_de_chaine.size();k++){
                    int z=une_chaine_de_chaine[k].size()-1;
                    if(z<5){
                        int distance=graphe.distance[1+ (graphe.N*une_chaine_de_chaine[k][z])+pile[i]];
                        if(distance<distance_min_chaine){
                            distance_min_chaine=distance;
                            I_min=i;
                            J_min=j;
                            K_min=k;
                            Z_min=z;
                        }
                    }
                }
            }
        }

        if(distance_min_chaine<distance_min_boucle){
            cout<<"on rajoute"<<pile[I_min]<<"  de distance"<<distance_min_chaine<<endl;
            //on le rajoute en bout de chaine
            r.chainesCollecte[J_min][K_min].push_back(pile[I_min]);
            pile.erase(pile.begin()+I_min);
        }
        else{
            //on creer une nouvelle chaine
            cout<<"on rajoute"<<pile[i_min]<<"  de distance"<<distance_min_boucle<<endl;
            vector<int> nouvelle_chaine;
            nouvelle_chaine.push_back(r.boucleStructurante[j_min]);
            nouvelle_chaine.push_back(pile[i_min]);
            r.chainesCollecte[j_min].push_back(nouvelle_chaine);
            pile.erase(pile.begin()+i_min);
        }
    }
}
void creation_chaines_opti(Reseau &r,vector<int> &pile,const Cluster &cluster,const Graph &graphe){
    //pile=pile des elements qui restent à placer dans ce cluster
    //je prends les elements un à un dans la pile, et je trouve le minimum, soit ce min est directement sur la chaine structurante, soit il est sur une chaine de collecte
    vector<vector<vector<int>>> a;
    r.chainesCollecte=a;
    for(int j=0;j<r.boucleStructurante.size();j++){
        vector<vector<int>> une_chaine_de_chaine;
        r.chainesCollecte.push_back(une_chaine_de_chaine);
    }
    //on creer une pile des distances et une pile des arg distance
    vector<int> pile_distance;
    vector<bool> pile_boucle;
    vector<int> pile_indice_sur_boucle;
    vector<int> pile_indice_de_chaine;
    for(int i=0;i<pile.size();i++){
        int distance_min_pour_cet_element_de_la_pile=graphe.distance[1+(graphe.N*r.boucleStructurante[0])+pile[i]];
        int j_min=0;//antenne sur la boucle structurante qui realise le min
        for(int j=0;j<r.boucleStructurante.size();j++){
            int distance=graphe.distance[1+ (graphe.N*r.boucleStructurante[j])+pile[i]];
            if(distance<distance_min_pour_cet_element_de_la_pile){
                distance_min_pour_cet_element_de_la_pile=distance;
                j_min=j;
            }
        }
        pile_distance.push_back(distance_min_pour_cet_element_de_la_pile);
        pile_boucle.push_back(true);//au debut ya pasencore de chaine, et les eemets sont naturelement branché par defaut à la boucle
        pile_indice_sur_boucle.push_back(j_min);
        pile_indice_de_chaine.push_back(-1);
    }
    while(pile.size()>0){
        int distance_min=pile_distance[0];
        int i_min=0;//antenne à placer
        for(int i=0;i<pile.size();i++){
            if(pile_distance[i]<distance_min){
                distance_min=pile_distance[i];
                i_min=i;
            }
        }//magiquement ici, on a parcouru TOUTE la boucle structurante et TOUTES les  chaines !!!
        int antenne_nouvelement_placee=pile[i_min];
        //cout<<"on rajoute"<<pile[i_min]<<"  de distance"<<distance_min<<endl;
        if(pile_boucle[i_min]==0){
            int I_min=i_min;//antenne à placer de la distance min
            int J_min=pile_indice_sur_boucle[i_min];//antenne sur la boucle associée à la chaine qui realise le min
            int K_min=pile_indice_de_chaine[i_min];


            //on le rajoute en bout de chaine
            r.chainesCollecte[J_min][K_min].push_back(pile[I_min]);
            pile.erase(pile.begin()+i_min);
            pile_boucle.erase(pile_boucle.begin()+i_min);
            pile_distance.erase(pile_distance.begin()+i_min);
            pile_indice_de_chaine.erase(pile_indice_de_chaine.begin()+i_min);
            pile_indice_sur_boucle.erase(pile_indice_sur_boucle.begin()+i_min);
            //il faut recalculer le min pour les elements de la pile qui étaient en concurence avec i-min
            for(int i=0;i<pile.size();i++){
                if((pile_indice_sur_boucle[i]==J_min)&&(pile_boucle[i]==false)&&(pile_indice_de_chaine[i]==K_min)){//penser au fait qu'il n'y a pas de recalcul si le nouveau creer une nouvelle chaine car on peut mettre une infinite d'antenne directement sur la meme antenne de la boucle
                    int d_min=graphe.distance[1+ (graphe.N*r.boucleStructurante[0])+pile[i]];
                    int jj_min=0;
                    //parcours de la boucle
                    for(int j=0;j<r.boucleStructurante.size(); j++){
                        int d=graphe.distance[1+ (graphe.N*r.boucleStructurante[j])+pile[i]];
                        if(d<d_min){
                            d_min=d;
                            jj_min=j;
                        }
                    }
                    pile_distance[i]=d_min;
                    pile_boucle[i]=true;
                    pile_indice_sur_boucle[i]=jj_min;
                    pile_indice_de_chaine[i]=-1;

                    //parcours des chaines
                    for(int j=0;j<r.boucleStructurante.size(); j++){
                        for(int z=0;z<r.chainesCollecte[j].size();z++){
                            int derniere_antenne_de_la_chaine=r.chainesCollecte[j][z][r.chainesCollecte[j][z].size()-1];
                            int d=graphe.distance[1+ (graphe.N*derniere_antenne_de_la_chaine)+pile[i]];
                            if((d<d_min)&&(r.chainesCollecte[j][z].size()<6)){
                                d_min=d;
                                pile_distance[i]=d_min;
                                pile_boucle[i]=false;
                                pile_indice_sur_boucle[i]=j;
                                pile_indice_de_chaine[i]=z;
                            }
                        }

                    }

                }
            }

            //mais maintenant il faut mettre a jour tous les elements de la pile
            if(r.chainesCollecte[J_min][K_min].size()<6){
                for(int i=0;i<pile.size();i++){

                    int distance=graphe.distance[1+ (graphe.N*antenne_nouvelement_placee)+pile[i]];
                    if(distance<pile_distance[i]){
                        pile_distance[i]=distance;
                        pile_boucle[i]=false;//maintenant, l'antenne est plus proche d'une chaine que de la boucle
                        pile_indice_de_chaine[i]=K_min;
                        pile_indice_sur_boucle[i]=J_min;
                    }
                }
            }



        }
        else{
            //on creer une nouvelle chaine
            vector<int> nouvelle_chaine;
            int j_min=pile_indice_sur_boucle[i_min];
            nouvelle_chaine.push_back(r.boucleStructurante[j_min]);
            nouvelle_chaine.push_back(pile[i_min]);
            r.chainesCollecte[j_min].push_back(nouvelle_chaine);
            pile.erase(pile.begin()+i_min);
            pile_boucle.erase(pile_boucle.begin()+i_min);
            pile_distance.erase(pile_distance.begin()+i_min);
            pile_indice_de_chaine.erase(pile_indice_de_chaine.begin()+i_min);
            pile_indice_sur_boucle.erase(pile_indice_sur_boucle.begin()+i_min);
            int k_min=r.chainesCollecte[j_min].size()-1;
            //ici il faut juste verifier pour les autres elements que la distance avec l'antenne nouvellement placee n'est pas plus petite que la derniere distance minimale
            if(r.chainesCollecte[j_min][k_min].size()<6){
                for(int i=0;i<pile.size();i++){

                    int distance=graphe.distance[1+ (graphe.N*antenne_nouvelement_placee)+pile[i]];
                    if(distance<pile_distance[i]){
                        pile_distance[i]=distance;
                        pile_boucle[i]=false;//maintenant, l'antenne est plus proche d'une chaine que de la boucle
                        pile_indice_sur_boucle[i]=j_min;
                        pile_indice_de_chaine[i]=k_min;
                    }
                }
            }

        }

    }
}
vector<Reseau> creation_chaines_global(vector<Reseau> Architecture,const Graph &graphe){
    //pile=pile des elements qui restent à placer dans ce cluster
    //je prends les elements un à un dans la pile, et je trouve le minimum, soit ce min est directement sur la chaine structurante, soit il est sur une chaine de collecte

    vector<Reseau> new_archi;
    vector<int> pile;
    for(int i=0;i<Architecture.size();i++){
        //cout<<"on creer la pile"<<endl;
        Reseau r_vide;
        r_vide.boucleStructurante=Architecture[i].boucleStructurante;
        for(int a=0;a<Architecture[i].chainesCollecte.size();a++){
            //cout<<"a"<<endl;
            vector<vector<int>> chaine_de_chaine=Architecture[i].chainesCollecte[a];
            for(int b=0;b<chaine_de_chaine.size();b++){
                for(int c=1;c<chaine_de_chaine[b].size();c++){
                    pile.push_back(chaine_de_chaine[b][c]);
                }
            }
        }
        new_archi.push_back(r_vide);
    }

    for(int a=0;a<Architecture.size();a++){
        //cout<<"on recopie les boucle structurantes"<<endl;
        for(int j=0;j<new_archi[a].boucleStructurante.size();j++){
            vector<vector<int>> une_chaine_de_chaine;
            new_archi[a].chainesCollecte.push_back(une_chaine_de_chaine);
        }
    }
    while(pile.size()>0){
        if(pile.size()%50==0){
            cout<<"size pile"<<pile.size()<<endl;
        }

        int distance_min_boucle=graphe.distance[1+ (graphe.N*new_archi[0].boucleStructurante[0])+pile[0]];
        int i_min=0;//antenne à placer
        int j_min=0;//antenne sur la boucle structurante qui realise le min
        int a_min=0; // numero du reseau ou onla placerai par defaut

        for(int i=0;i<pile.size();i++){
            for(int a=0;a<Architecture.size();a++){
             Reseau r=new_archi[a];
             for(int j=0;j<r.boucleStructurante.size();j++){
                 int distance=graphe.distance[1+ (graphe.N*r.boucleStructurante[j])+pile[i]];
                 if(distance<distance_min_boucle){
                     distance_min_boucle=distance;
                     i_min=i;
                     j_min=j;
                     a_min=a;
                 }
             }
            }

        }

        int distance_min_chaine=distance_min_boucle;//rmq, pas besoin d'initialiser correctement!
        int I_min=0;//antenne à placer de la distance min
        int J_min=0;//antenne sur la boucle associée à la chaine qui realise le min
        int K_min=0;
        int Z_min=0;//numero de l'antenne sur la chaine de collecte
        int A_min=0;

        for(int i=0;i<pile.size();i++){
            for(int a=0;a<Architecture.size();a++){
                Reseau r=new_archi[a];
                for(int j=0;j<r.boucleStructurante.size();j++){
                    vector<vector<int>> une_chaine_de_chaine=r.chainesCollecte[j];
                    for(int k=0;k<une_chaine_de_chaine.size();k++){
                        int z=une_chaine_de_chaine[k].size()-1;
                        if(z<5){
                            int distance=graphe.distance[1+ (graphe.N*une_chaine_de_chaine[k][z])+pile[i]];
                            if(distance<distance_min_chaine){
                                distance_min_chaine=distance;
                                I_min=i;
                                J_min=j;
                                K_min=k;
                                Z_min=z;
                                A_min=a;
                            }
                        }
                    }
                }
            }

        }
        if(distance_min_chaine<distance_min_boucle){
            //on le rajoute en bout de chaine
            new_archi[A_min].chainesCollecte[J_min][K_min].push_back(pile[I_min]);
            pile.erase(pile.begin()+I_min);
        }
        else{
            //on creer une nouvelle chaine
            vector<int> nouvelle_chaine;
            nouvelle_chaine.push_back(new_archi[a_min].boucleStructurante[j_min]);
            nouvelle_chaine.push_back(pile[i_min]);
            new_archi[a_min].chainesCollecte[j_min].push_back(nouvelle_chaine);
            pile.erase(pile.begin()+i_min);
        }
    }
    return new_archi;
}

Reseau creationReseau(const Cluster &cluster,const Graph &graphe){
    Reseau r(cluster.distribution);
    int sizeCluster = cluster.antennes.size();//exemple: si cluster contient le distributeur 0 et l'antenne 2, sizeCluster=1
    vector<int> pile_des_antennes_qui_restent_a_placer=creation_boucle_structurante_aleatoire(r,cluster,graphe);
    creation_chaines_opti(r,pile_des_antennes_qui_restent_a_placer,cluster,graphe);
    //r.affichage_graphique_Reseau(graphe);
    return r;
}


vector<Reseau> creationArchitecture(const vector<Cluster> &clusters,const Graph &graphe){
    int nb_clusters = clusters.size();
    vector<Reseau> architecture;
    for(int i=0; i<nb_clusters; i++){
        architecture.push_back(creationReseau(clusters.at(i),graphe));
    }
    return architecture;
}
int calcul_cost_boucle_structurante(const Reseau &reseau,const Graph &graphe) {
    vector<int> boucle=reseau.boucleStructurante;
    int cost_boucle=0;
    if(reseau.boucleStructurante.size()>1){
        cost_boucle+= graphe.distance[1+ (graphe.N*reseau.boucleStructurante[boucle.size()-1])+reseau.boucleStructurante[0]];
    }//pas besoin de boucler si boucle structurante ne contient que le distri et une autre antenne!
    for (int i=0; i+1<boucle.size();i++) {
        cost_boucle += graphe.distance[1+ (graphe.N*reseau.boucleStructurante[i])+reseau.boucleStructurante[i+1]];
    }
    return cost_boucle;
}

int calcul_cost_chaines_collecte(const Reseau &reseau,const Graph &graphe) {
    int cost_chaines = 0;
    vector<vector<vector<int>>> Les_chaines=(reseau.chainesCollecte);

    for(int i=0; i<Les_chaines.size();i++) {
        for(int i_prime=0;i_prime<Les_chaines[i].size();i_prime++){
            vector<int> une_chaine=Les_chaines[i][i_prime];
            for (int j=0; j+1<une_chaine.size();j++) {//alors la!!!!!!!!!!!!!!!!!!!!!!!
                cost_chaines += graphe.distance[1+ (graphe.N*une_chaine[j])+une_chaine[j+1]];
            }
        }

    }
    return cost_chaines;
}
int calcul_cost_total_reseau(const Reseau &reseau,const Graph &graphe) {
    int cost_total_reseau = calcul_cost_boucle_structurante(reseau,graphe);
    cost_total_reseau+=calcul_cost_chaines_collecte(reseau, graphe);
    return cost_total_reseau;
}
int calcul_cost_total_architecture(const vector<Reseau> &a,const Graph &graphe) {
    int cost_arcitecture=0;
    for(int i=0;i<a.size();i++){
        cost_arcitecture+=calcul_cost_total_reseau(a[i],graphe);
    }
    return cost_arcitecture;
}

vector<Reseau> heuristique(const Graph &graphe,vector<Cluster> &clusters){
    return creationArchitecture(clusters, graphe);
}
