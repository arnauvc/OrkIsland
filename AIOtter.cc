#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Otter


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
    //typedef pair<Pos,double> PosP; // posicio i pes
    typedef vector<int> VEC;
    //typedef vector<VEC> MAT;

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

                        /*
                        //bool eunit = true;
                        if((c.type == CITY and city_owner(c.city_id) != me()) or (c.type == PATH and path_owner(c.path_id) != me())){
                            if(ppa.dir == NONE) return Dir(ranvec[i]); 
                            else return ppa.dir; 
                        }
                        else if(ppa.dir == NONE and c.unit_id != -1 and unit(c.unit_id).player != me() and unit(c.unit_id).health < (u.health - costvida) ){ //Hi ha una unitat 
                            return Dir(ranvec[i]); 
                        }
                         
                        PosA pq;
                        pq.pos = posi;
                        pq.dis = d[posi];
                        if(ppa.dir == NONE) pq.dir = Dir(ranvec[i]);
                        else pq.dir = ppa.dir;
                        Q.push(pq);
                        */
                        
                        
                        
                        if((c.type == CITY and city_owner(c.city_id) != me()) or (c.type == PATH and path_owner(c.path_id) != me())){
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
    //vector de id's dels meus orks
    VEC my_orks = orks(me());

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
