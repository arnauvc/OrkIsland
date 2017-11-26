#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Rabbit


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
    typedef pair<Pos,double> PosP; // posicio i pes
    typedef vector<int> VEC;
    typedef vector<VEC> MAT;

    struct PosA{
        PosP posp;
        Dir dir;//Dir del pare original
    };   

    vector<Dir> vdir{BOTTOM, RIGHT, TOP, LEFT};

    struct PosComp{
        bool operator()(const PosA& a, const PosA& b) {return a.posp.second > b.posp.second;}
    };

    MAT pos_orks;

    inline int prodIJ(Pos a) {return a.i * a.j;}

    inline Dir direccio(Pos a, Pos b){
        if(a + BOTTOM == b) return BOTTOM;
        else if (a + TOP == b) return TOP;
        else if (a + RIGHT == b) return RIGHT;
        else return LEFT;
    }

  //Dijkstra toooo perraco
  Dir cerca(int id){
    int size = rows() * cols();
    Unit u = unit(id); //unitat amb identificador id
    Pos pos = u.pos; //Posicio d'aquesta unitat u
    map<Pos,int> d;//Mapa de distancies
    d[pos] = 0;//Posicio inicial -> te distancia 0
    vector<bool> S(size, false);//Vector de nodes visitats
    priority_queue<PosA, vector<PosA>, PosComp > Q;
    PosA pa;
    pa.posp.first = pos; //posicio
    pa.posp.second = 0; //valor de distancia
    pa.dir = NONE; // direccio pare
    Q.push(pa);
    while (not Q.empty()){ 
        PosA ppa = Q.top();
        Pos posicio = Q.top().posp.first;
        Q.pop();  
        int coordPos = prodIJ(posicio);
        if(not S[prodIJ(posicio)]){ //Si la posicio no esta visitada
            S[prodIJ(posicio)] = true;
            VEC ranvec = random_permutation(vdir.size());
            for(int i = 0; i < 4; ++i){
                Pos posi = posicio + Dir(ranvec[i]);//Posicio meva + nova direccio
                int coord = prodIJ(posi);
                Cell c = cell(posi);//Cela de la nova posicio a comprovar
                if(pos_ok(posi) and c.type != WATER){
                    int costvida = cost(c.type);//Cost en vida de la nova cela
                    auto it = d.find(posi);
                    if(it == d.end() or d[posi] > d[posicio] + costvida){
                        d[posi] = d[posicio] + costvida;
                        if((c.type == CITY and city_owner(c.city_id) != me()) or (c.type == PATH and path_owner(c.path_id) != me())){
                            if(ppa.dir == NONE){
                                return Dir(ranvec[i]);  
                            } 
                            else{ 
                               return ppa.dir; 
                            } 
                        }
                        PosA pq;
                        pq.posp.first = posi;
                        pq.posp.second = d[posi];
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

    if(round() == 0){
      //pos_orks = MAT(rows(), VEC(cols(), -1));
    }

    //Per tots els orks, executa una ordre de moviment
    for(unsigned int i = 0; i < my_orks.size(); ++i){
      execute(Command(my_orks[i],cerca(my_orks[i])));
    }
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
