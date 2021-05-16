#include "annexe.h"



void affiche(const Graph &graphe){
    Window W=openWindow(512,512);
    double x_max=maximum_vector(graphe.coord_geo_x);
    double y_max=maximum_vector(graphe.coord_geo_y);
    double x_min=minimum_vector(graphe.coord_geo_x);
    double y_min=minimum_vector(graphe.coord_geo_y);

    double a_x=(500-10)/(x_max-x_min);
    double b_x=10-(a_x*x_min);
    double a_y=(500-10)/(y_max-y_min);
    double b_y=10-(a_y*y_min);
    for (int i=0;i<graphe.N;i++){
        double x_brut=graphe.coord_geo_x[i];
        double y_brut=graphe.coord_geo_y[i];
        double x=(a_x*x_brut)+b_x;
        double y=(a_y*y_brut)+b_y;
        Color C;
        if (graphe.distrib[i]==true){C=RED;}
        else{C=BLACK;}
        drawRect((int)x,(int)y,5,5,C);
        std::string s = std::to_string(i);
        drawString((int)x,(int)y,s,C,8);
    }
    click();
    closeWindow(W);
}
void write_solution(const Graph &graphe,const vector<Reseau> &Architecture, string file_path) {
    ofstream file(file_path.c_str(),ios::out);
    if(file) {
        for(int n_reseau=0;n_reseau<Architecture.size();n_reseau++){
            vector<int> BoucleStruct=((Architecture[n_reseau]).boucleStructurante);
            file<<"b"<<" ";
            for(int i=0;i<BoucleStruct.size()-1;i++){
                file<<BoucleStruct[i]<<" ";
            }
            file<<BoucleStruct[BoucleStruct.size()-1]<<endl;

            vector<vector<vector<int>>> tab_Chaine=((Architecture[n_reseau]).chainesCollecte);
            for(int nb_chaine=0;nb_chaine<tab_Chaine.size();nb_chaine++){
                if(tab_Chaine[nb_chaine].size()>0){
                    for(int q=0;q<tab_Chaine[nb_chaine].size();q++){
                        vector<int> une_chaine=tab_Chaine[nb_chaine][q];
                        if (une_chaine.size()>0){
                            file<<"c ";
                        }
                        for(int i=0;i<une_chaine.size()-1;i++){
                            file<<une_chaine[i]<<" ";
                        }
                        if(une_chaine.size()>0){
                            file<<une_chaine[une_chaine.size()-1]<<endl;
                        }
                    }
                }
            }
        }
        file.close();
    }
    else
        cout<<"Error while loading file to write"<<endl;
}



int distribInClusters(const vector<Cluster> &clusters, int ptDistribution){ //renvoie la position du cluster associé à la ptDistrib, -1 s'il nexiste pas
    int res = -1;
    int nClusters = clusters.size();
    if(nClusters>0){
        for (int i=0;i<nClusters; i++){
            if(clusters[i].distribution == ptDistribution )
                res = i;
        }
    }
    return res;
}
int nearestdistribfromantenne(const Graph &graphe, int ind_antenne) { //indice de l'antenne dans tableau nodes
    int distri_min=0;
    int distance_min=graphe.distance[1+(graphe.N*distri_min)+ind_antenne];
    for(int distri=0;distri<graphe.nb_distrib;distri++){
        int distance=graphe.distance[1+(graphe.N*distri)+ind_antenne];
        if(distance<distance_min){
            distri_min=distri;
            distance_min=distance;
        }
    }
    return distri_min;
}
int nearestdistribfromantenne_norme2(const Graph &graphe, int ind_antenne) { //indice de l'antenne dans tableau nodes
    int distri_min=0;
    float distance_min=graphe.distance_norme2(0,ind_antenne);
    for(int distri=0;distri<graphe.nb_distrib;distri++){
        float distance=graphe.distance_norme2(distri,ind_antenne);
        if(distance<distance_min){
            distri_min=distri;
            distance_min=distance;
        }
    }
    return distri_min;
}
vector<Cluster> creation_cluster(const Graph& graphe,bool norme2){
    vector<Cluster> clusters;
    for(int i=0; i< graphe.N; i++){ // i balaie tous les sommets
        if(graphe.distrib[i]){ // si le sommet i est un pt de distrib, il faut verifier qu'il a un cluster
            if(distribInClusters(clusters, i)==-1)
                clusters.push_back(Cluster(i));
        }
        else{ //si c'est une antenne
            int nearestDistrib;
            if(norme2){
                nearestDistrib = nearestdistribfromantenne_norme2(graphe, i);
            }
            else{
                nearestDistrib = nearestdistribfromantenne(graphe, i);
            }
            int distanceToDistrib = graphe.distance[1+graphe.N*nearestDistrib + i];
            int indiceCluster = distribInClusters(clusters, nearestDistrib);
            if(indiceCluster<0){//si le cluster n'existe pas encore
                Cluster nouveauCluster(nearestDistrib);
                nouveauCluster.ajoutAntenne(i,distanceToDistrib);
                clusters.push_back(nouveauCluster);
            }
            else{
                clusters[indiceCluster].ajoutAntenne(i,distanceToDistrib);
            }
        }
    }
    //fin creation clusters
    for(int numero_cluster=0;numero_cluster<clusters.size();numero_cluster++){
        //clusters[numero_cluster].affiche_Cluster();
    }
    return clusters;
}
vector<Cluster> creation_cluster_a_partir_architecture(vector<Reseau> Architecture,const Graph& graphe){
    vector<Cluster> clusters;
    for(int i=0;i<Architecture.size();i++){
        Cluster cl(Architecture[i].boucleStructurante[0]);
        for(int a=1;a<Architecture[i].boucleStructurante.size();a++){//on ne met pas le distributeur dans le cluster!
            int antenne=Architecture[i].boucleStructurante[a];
            Point2D p(antenne,0);
            cl.antennes.push_back(p);
        }
        for(int a=0;a<Architecture[i].chainesCollecte.size();a++){
            //cout<<"a"<<endl;
            vector<vector<int>> chaine_de_chaine=Architecture[i].chainesCollecte[a];
            for(int b=0;b<chaine_de_chaine.size();b++){
                for(int c=1;c<chaine_de_chaine[b].size();c++){
                    int antenne=chaine_de_chaine[b][c];
                    Point2D p(antenne,0);
                    cl.antennes.push_back(p);
                }
            }
        }
        clusters.push_back(cl);
    }
    return clusters;
}
Reseau recuis(const vector<Cluster> &clusters, const Graph &graphe,int i,int ville,const vector<vector<int>> &best_scores_ville_cluster,vector<vector<int>> aide){
    cout<<"debut reseau "<<i<<endl;
    Reseau r_min=creationReseau(clusters.at(i),graphe);
    int distance_min=calcul_cost_total_reseau(r_min,graphe);
    for(int j=0;j<500;j++){//en gros, assez d'iteration pour donner un truc pas trop degueu, mais pas trop pour pas un minimum local
        Reseau r=creationReseau(clusters.at(i),graphe);
        int distance=calcul_cost_total_reseau(r,graphe);
        if(distance<distance_min){
            r_min=r;
            distance_min=distance;
            //cout<<"reseau= "<<i<<"   distance= "<<distance_min<<endl;
        }
    }
    double T=10000;
    double T_final=10;
    double alpha=-log(T/T_final);
    int nbr_iteration=5000;
    alpha=alpha/nbr_iteration;
    alpha=exp(alpha);
    //cout<<"alpha"<<alpha<<endl;
    int distance_prescedente=calcul_cost_total_reseau(r_min,graphe);
    Reseau r=r_min;
    bool bonus=true;// presemption d'innocence

    for(int j=0;j<nbr_iteration;j++){
        if(bonus&&(distance_min<best_scores_ville_cluster[ville][i]+aide[ville][i])){
            cout<<"BONUS                                            BONUS                           BONUS!"<<endl;
            j=-4*nbr_iteration;
            bonus=false;//le bonus n'est disponible qu'une fois
        }
        Reseau r_potentiel=r;
        vector<int> pile_des_antennes_qui_restent_a_placer=modification_boucle_structurante(r_potentiel,clusters[i],graphe);
        creation_chaines_opti(r_potentiel,pile_des_antennes_qui_restent_a_placer,clusters[i],graphe);
        int distance=calcul_cost_total_reseau(r_potentiel,graphe);
        double proba=( rand()/(double)RAND_MAX );
        int delta_distance=distance-distance_prescedente;
        //cout<<"ddistance"<<delta_distance<<endl;
        if(delta_distance<0){
            r=r_potentiel;
            distance_prescedente=distance;
            if(distance<distance_min){
                r_min=r;
                distance_min=distance;
                //cout<<"Reseau= "<<i<<"   distance= "<<distance_min<<"  j= "<<j<<"  delta=distance_d_min="<<distance-distance_min<<endl;
            }

        }
        else{
            //on accepte comme meme avec proba p=exp(delta_distance/T)
            proba=1;
            if(proba<exp(-delta_distance/T)){
                //cout<<"Reseau rs= "<<i<<"   distance= "<<distance<<"    r_potetiel bouvlestructurante.size"<<r_potentiel.boucleStructurante.size()<<" T="<<T<<endl;
                r=r_potentiel;
                distance_prescedente=distance;
            }

        }
        T=T*alpha;
    }
    return r_min;
}
vector<string> decoupe_string(string str){
    vector<string> tab;
    string sousChaine;
    std::istringstream ss(str);
    while (getline(ss, sousChaine,' '))
    {
        tab.push_back(sousChaine);
    }
    return tab;
}
void affiche_vector(vector<int> v){
    for(int i=0;i<v.size();i++){
        cout<<v[i]<<" ";
    }
    cout<<endl;
}
Reseau lecture_reseau(string str){
    ifstream fichier(str, ios::in);  // on ouvre en lecture
    Reseau r;
    vector<int> boucle;
    vector<int> les_noeuds;
    vector<vector<int>> chaines;
    if(fichier)  // si l'ouverture a fonctionné
    {

        string contenu;  // déclaration d'une chaîne qui contiendra la ligne lue
        string ligne;
        while(getline(fichier,ligne)){
            vector<string> ligne_decoupe=decoupe_string(ligne);
            if(ligne_decoupe[0]=="b"){
                for(int i=1;i<ligne_decoupe.size();i++){
                    string contenu=ligne_decoupe[i];
                    int a = std::stoi(contenu);
                    boucle.push_back(a);
                    les_noeuds.push_back(a);
                }
            }
            if(ligne_decoupe[0]=="c"){
                vector<int> une_chaine;
                for(int i=1;i<ligne_decoupe.size();i++){
                    string contenu=ligne_decoupe[i];
                    int a = std::stoi(contenu);
                    une_chaine.push_back(a);
                    les_noeuds.push_back(a);
                }
                chaines.push_back(une_chaine);
            }
        }
        r.boucleStructurante=boucle;
        r.les_noeuds=les_noeuds;
        for(int i=0;i<boucle.size();i++){
            int antenne_sur_boucle=boucle[i];
            vector<vector<int>> chaine_de_chaine;
            for(int j=0;j<chaines.size();j++){
                if(chaines[j][0]==antenne_sur_boucle){
                    chaine_de_chaine.push_back(chaines[j]);
                }
            }
            r.chainesCollecte.push_back(chaine_de_chaine);
        }
        fichier.close();
        return r;
    }
    else
        cerr << "Impossible d'ouvrir le fichier !" << endl;
}



int main() {
    srand(time(NULL));
    int ville=2;
    string file_path_distances;
    string file_path_nodes;
    string file_path_write;
    if(ville==0){
        file_path_distances="/home/esthete/Desktop/kiroptimiste/kiroptimiste/grenoble/distances.csv";
        file_path_nodes="/home/esthete/Desktop/kiroptimiste/kiroptimiste/grenoble/nodes2.csv";
        file_path_write="/home/esthete/Desktop/kiroptimiste/kiroptimiste/solutions/grenoble.csv";
    }
    if(ville==1){
        file_path_distances="/home/esthete/Desktop/kiroptimiste/kiroptimiste/nice/distances.csv";
        file_path_nodes="/home/esthete/Desktop/kiroptimiste/kiroptimiste/nice/nodes.csv";
        file_path_write="/home/esthete/Desktop/kiroptimiste/kiroptimiste/solutions/nice_recomposée_ordre_2.csv";
    }
    if(ville==2){
        file_path_distances="/home/esthete/Desktop/kiroptimiste/kiroptimiste/paris/distances.csv";
        file_path_nodes="/home/esthete/Desktop/kiroptimiste/kiroptimiste/paris/nodes.csv";
        file_path_write="/home/esthete/Desktop/kiroptimiste/kiroptimiste/solutions/geonewparis_7janvier_ordre2.csv";
    }
    Graph graphe(file_path_distances,file_path_nodes);
    //affiche(graphe);
    //calcul de la borne inf: chaque antenne qui n'est pas un ditributeur doit etre lié à une autre antenne autre que elle!
    int borne_inf=0;
    for(int i=0;i<graphe.N;i++){
        int borne_inf_pour_cette_antenne=graphe.distance[1+(graphe.N*i) + 0];
        bool lie_a_un_distri;
        if(graphe.distrib[i]==0){
            for(int j=0;j<graphe.N;j++){
                int da=graphe.distance[1+(graphe.N*i) + j];
                int db=graphe.distance[1+(graphe.N*j) + i];
                int d=min(da,db);
                if((graphe.distrib[j]==0)){
                    if((d<borne_inf_pour_cette_antenne)&&(i!=j)){
                        borne_inf_pour_cette_antenne=d;
                        lie_a_un_distri=false;
                    }
                }
                else{
                    if((d<borne_inf_pour_cette_antenne)&&(i!=j)){
                        borne_inf_pour_cette_antenne=d;
                        lie_a_un_distri=true;
                    }
                }
            }
            if(lie_a_un_distri==0){
                borne_inf_pour_cette_antenne=(int)(borne_inf_pour_cette_antenne/2);
            }
            borne_inf+=borne_inf_pour_cette_antenne;
        }
    }
    cout<<"borne inf="<<borne_inf<<endl;


    //vector<int> tab_bonus0={1517,474};
    vector<int> tab_bonus0={2000,2000};
    vector<int> tab_aide0={0,0};
    vector<int> tab_bonus1={1246,4356+30,827}; //nice
    vector<int> tab_aide1={0,30,0};
    //vector<int> tab_bonus2={2403,1545,1328,2104,1561,3391,2324,2805,2337,2728,3169};// le 1,3,6 sont sûr, les autres peut etre avec l'evaluation prescedente defaillante
    //vector<int> tab_bonus2={2401,1508,1321,2062,1569,3443,2324,2797,2320,2710,3166};// le 1,3,6 sont sûr, les autres peut etre avec l'evaluation prescedente defaillante
    //vector<int> tab_bonus2={9000,9000,90000,90000,90000,90000,90000,90000,90000,90000,90000,90000};
    vector<int> tab_bonus2={0,0,0,0,0,0,0,0,0,0,0,0};
    vector<int> tab_aide2={0,0,0,0,0,0,0,0,0,0,0,0};
    if(false){
        int s=0;
        for(int i=0;i<tab_bonus2.size();i++){
            s+=tab_bonus2[i] ;
        }
        cout<<" best score theorique pour les clusters d'ordre 0 = "<<s<<endl;
    }

    vector<vector<int>> best_scores_ville_cluster={tab_bonus0,tab_bonus1,tab_bonus2};
    vector<vector<int>> aide={tab_aide0,tab_aide1,tab_aide2};


    vector<Cluster> clusters=creation_cluster(graphe,false);// les clusters sont fixés une bonne fois pour toute


    if(true){
        int somme=0;
        vector<Reseau> architecture_originelle;
        for(int i=0;i<clusters.size();i++){
            std::string s = std::to_string(i);
            std::string sville = std::to_string(ville);
            //Reseau r=lecture_reseau("/home/esthete/Desktop/kiroptimiste/kiroptimiste/solutions/ville_nuit_du_2janvier2cluster"+s+".csv");
            //Reseau r=lecture_reseau("/home/esthete/Desktop/kiroptimiste/kiroptimiste/solutions/ville1cluster"+s+".csv");
            //Reseau r=lecture_reseau("/home/esthete/Desktop/kiroptimiste/kiroptimiste/solutions/ville_4janvier"+sville+"cluster_ordre_0_"+s+".csv");
            //Reseau r=lecture_reseau("/home/esthete/Desktop/kiroptimiste/kiroptimiste/solutions/ville_6janvier"+sville+"cluster_ordre_0_"+s+".csv");
            //Reseau r=lecture_reseau("/home/esthete/Desktop/kiroptimiste/kiroptimiste/solutions/newville_6janvier"+sville+"cluster_ordre_1_"+s+".csv");
            //Reseau r=lecture_reseau("/home/esthete/Desktop/kiroptimiste/kiroptimiste/solutions/geonewville_7janvier"+sville+"cluster_ordre_1_"+s+".csv");
            Reseau r=lecture_reseau("/home/esthete/Desktop/kiroptimiste/kiroptimiste/solutions/geonewville_8janvier"+sville+"cluster_ordre_2_"+s+".csv");
            somme+=calcul_cost_total_reseau(r,graphe);
            cout<<"calcul"<<i<<"     "<<calcul_cost_total_reseau(r,graphe)<<endl;
            //r.affichage_graphique_Reseau(graphe);
            architecture_originelle.push_back(r);
        }
        cout<<"total non modifie"<<somme<<endl;
        vector<Reseau>Archi_recomposee=creation_chaines_global(architecture_originelle,graphe);
        int calcul_pour_voir=calcul_cost_total_architecture(Archi_recomposee,graphe);
        cout<<"cost modifie"<<calcul_pour_voir<<endl;
        affichage_graphique_Architecture(architecture_originelle,graphe);
        //write_solution(graphe,Archi_recomposee,file_path_write);
        affichage_graphique_Architecture(Archi_recomposee,graphe);
        clusters=creation_cluster_a_partir_architecture(Archi_recomposee,graphe);// les clusters sont fixés une bonne fois pour toute

    }
    int best_score_ever=124907;//en vrai c'est 24907
    //int best_score_ever=7000;
    vector<int> score_meta;
    for(int meta=1;meta<1;meta++){
        vector<Reseau> Architecture;
        cout<<"                 META                                             "<<meta<<endl;
        if(true){//calculer le recuit a partir de du cluster donnee
            for(int i=0;i<clusters.size();i++){
                Reseau r_min=recuis(clusters,graphe,i,ville,best_scores_ville_cluster,aide);
                int cost_min=calcul_cost_total_reseau(r_min,graphe);
                for(int mort=0;mort<500;mort++){
                    Reseau r=recuis(clusters,graphe,i,ville,best_scores_ville_cluster,aide);
                    int cost=calcul_cost_total_reseau(r,graphe);
                    cout<<cost<<"                                                           mort            "<<mort<<endl;
                    if(cost<cost_min){
                        r_min=r;
                        cost_min=cost;
                    }
                }
                Architecture.push_back(r_min);
            }
            for(int i=0;i<clusters.size();i++){
                int cost_reseau=calcul_cost_total_reseau(Architecture[i],graphe);
                cout<<"distance reseau"<<i<<" = "<<cost_reseau<<endl;
                //Architecture[i].affichage_graphique_Reseau(graphe);
                bool write_cluster=(cost_reseau<=best_scores_ville_cluster[ville][i]);
                write_cluster=true;
                if(write_cluster){
                    cout<<"best_score cluster"<<i<<"  = "<<cost_reseau<<endl;
                    cout<<"veuillez modifier le tableau best score"<<endl;
                    std::string s = std::to_string(i);
                    std::string sville = std::to_string(ville);
                    vector<Reseau> Architecture_du_reseau;
                    Architecture_du_reseau.push_back(Architecture[i]);
                    write_solution(graphe,Architecture_du_reseau,"/home/esthete/Desktop/kiroptimiste/kiroptimiste/solutions/geonewville_8janvier"+sville+"cluster_ordre_2_"+s+".csv");
                }
            }
            //affichage_graphique_Architecture(Architecture,graphe);
            int cost_total_min=calcul_cost_total_architecture(Architecture,graphe);

            cout<<"cost total="<<cost_total_min<<endl;
            vector<int> tab_solution_min={1991,6429,25695};
            bool write=cost_total_min<tab_solution_min[ville];

            if(cost_total_min<best_score_ever){
                cout<<"jete beze"<<endl;
                best_score_ever=cost_total_min;
                write_solution(graphe,Architecture,file_path_write);
            }
        }
        if(true){
            int somme=0;
            vector<Reseau> architecture_originelle;
            for(int i=0;i<clusters.size();i++){
                std::string s = std::to_string(i);
                std::string sville = std::to_string(ville);
                //Reseau r=lecture_reseau("/home/esthete/Desktop/kiroptimiste/kiroptimiste/solutions/ville_nuit_du_2janvier2cluster"+s+".csv");
                //Reseau r=lecture_reseau("/home/esthete/Desktop/kiroptimiste/kiroptimiste/solutions/ville1cluster"+s+".csv");
                //Reseau r=lecture_reseau("/home/esthete/Desktop/kiroptimiste/kiroptimiste/solutions/ville_4janvier"+sville+"cluster_ordre_0_"+s+".csv");
                Reseau r=lecture_reseau("/home/esthete/Desktop/kiroptimiste/kiroptimiste/solutions/geonewville_8janvier"+sville+"cluster_ordre_2_"+s+".csv");

                somme+=calcul_cost_total_reseau(r,graphe);
                cout<<"calcul"<<i<<"     "<<calcul_cost_total_reseau(r,graphe)<<endl;
                //r.affichage_graphique_Reseau(graphe);
                architecture_originelle.push_back(r);
            }
            cout<<"total non modifie"<<somme<<endl;
            vector<Reseau>Archi_recomposee=creation_chaines_global(architecture_originelle,graphe);
            int calcul_pour_voir=calcul_cost_total_architecture(Archi_recomposee,graphe);
            cout<<"cost modifie"<<calcul_pour_voir<<endl;
            //affichage_graphique_Architecture(architecture_originelle,graphe);
            score_meta.push_back(calcul_pour_voir);
            cout<<"score: "<<endl;
            affiche_vector(score_meta);
            if(calcul_pour_voir<best_score_ever){
                best_score_ever=calcul_pour_voir;
                cout<<"WRITE!!!"<<calcul_pour_voir<<endl;
                write_solution(graphe,Archi_recomposee,file_path_write);
                for(int i=0;i<clusters.size();i++){
                    std::string s = std::to_string(i);
                    std::string sville = std::to_string(ville);
                    vector<Reseau> A;
                    A.push_back(Archi_recomposee[i]);
                    write_solution(graphe,A,"/home/esthete/Desktop/kiroptimiste/kiroptimiste/solutions/bestgeonewville_8janvier"+sville+"cluster_ordre_2_"+s+".csv");

                }
            }

            //affichage_graphique_Architecture(Archi_recomposee,graphe);
            //clusters=creation_cluster_a_partir_architecture(Archi_recomposee,graphe);// les clusters sont fixés une bonne fois pour toute

        }
    }
    cout<<"best"<<best_score_ever<<endl;

    endGraphics();
    return 0;
}
