#include "Player.hh"



/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Hamster


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
    static Player* factory () {
        return new PLAYER_NAME;
    }

  /**
   * Types and attributes for your player can be defined here.
   */

    typedef vector<int> VEC;

    struct PosA{
        Pos pos;
        double dis;
        Dir dir;//Dir del pare original
    };   

    vector<Dir> vdir{BOTTOM, RIGHT, TOP, LEFT};

    struct PosComp{
        bool operator()(const PosA& a, const PosA& b) {return a.dis > b.dis;}
    };

    inline int prodIJ(Pos a) {return a.i * a.j;}

    //int MAX_ORKS = ceil((nb_cities() + nb_paths())/60);


  //Dijkstra toooo perraco
  Dir cerca(int id){
    int size = rows() * cols();
    Unit u = unit(id); 
    Pos pos = u.pos;
    PosA pa;
    pa.pos = pos; //posicio
    pa.dis = 0; //valor de distancia
    pa.dir = NONE; // direccio pare

    map<Pos,int> d;//Mapa de distancies
    d[pos] = 0;

    vector<bool> S(size, false);//Vector de nodes visitats

    priority_queue<PosA, vector<PosA>, PosComp > Q;
    Q.push(pa);

    while (not Q.empty()){ 
        PosA ppa = Q.top();
        Pos posicio = Q.top().pos;
        Q.pop();  
        if(not S[prodIJ(posicio)]){ //Si la posicio no esta visitada
            S[prodIJ(posicio)] = true;
            VEC ranvec = random_permutation(vdir.size());
            for(int i = 0; i < 4; ++i){
                Pos posi = posicio + Dir(ranvec[i]);//Posicio meva + nova direccio
                Cell c = cell(posi);//Cela de la nova posicio a comprovar
                if(pos_ok(posi) and c.type != WATER){
                    int costvida = cost(c.type);//Cost en vida de la nova cela
                    auto it = d.find(posi);
                    if(it == d.end() or d[posi] > d[posicio] + costvida){
                        d[posi] = d[posicio] + costvida;

                        //Chasing putitas
                        if(c.unit_id != -1 and unit(c.unit_id).player != me() and unit(c.unit_id).health < u.health and ppa.dir == NONE){
                            return Dir(ranvec[i]); //direccio de la cela que avaluem
                        }

                        //Escape putita
                        if(c.unit_id != -1 and unit(c.unit_id).player != me() and unit(c.unit_id).health > u.health and ppa.dir == NONE){
                            pair<int, Dir> parella;
                            parella = make_pair(5, Dir(ranvec[i]));
                            for(int t = 0; t < 4; ++t){//Per les 4 direccions
                                if(Dir(ranvec[i]) != Dir(ranvec[t])){ // Si no es la direccio del enemic
                                    Pos newpos = posicio + Dir(ranvec[t]);
                                    Cell newc = cell(newpos);
                                    if(u.health - cost(newc.type) < 1) return Dir(ranvec[t]);
                                    else if(cost(newc.type) == 0) return Dir(ranvec[t]);
                                    else if (cost(newc.type) < parella.first){
                                        parella.first = cost(newc.type);
                                        parella.second = Dir(ranvec[t]);
                                    }
                                }
                            }
                            return parella.second;
                        }


                        if((c.type == CITY and city_owner(c.city_id) != me()) or (c.type == PATH and path_owner(c.path_id) != me())) {
                            if(ppa.dir == NONE) return Dir(ranvec[i]); 
                            else return ppa.dir; 
                        }
                        
                        PosA pq;
                        pq.pos = posi;
                        pq.dis = d[posi];
                        if(ppa.dir == NONE) pq.dir = Dir(ranvec[i]);
                        else pq.dir = ppa.dir;
                        Q.push(pq);
                        
                    }
                }
            }
        }
    }  
    return NONE;
  }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    /*
    if(round() == 0) {
        cities = VEC(nb_cities());
        paths = VEC(nb_paths());
    }
    */

    //vector de id's dels meus orks
    VEC my_orks = orks(me()); // Les meves unitats (orks)

    bool cit, pat = true;

    if(round() > 100){//Assumim que no conquerirem tot abans de la meitat
        cit = false; //cert si ens falten ciutats per conquerir
        for(int c = 0; c < nb_cities() and !cit; ++c){
            if(city_owner(c) != me()) cit = true;
        }
        pat = false; //cert si ens falten camins per conquerir
        for(int p = 0; p < nb_paths() and !pat; ++p){
            if(path_owner(p) != me()) pat = true;
        }
    }

    if (cit or pat){ // Si tenim tot conquerit, no fem res
        for(unsigned int i = 0; i < my_orks.size(); ++i){
        execute(Command(my_orks[i],cerca(my_orks[i])));
        }
    }
    
  }
};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
