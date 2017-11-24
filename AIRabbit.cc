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

    struct PosA{
        PosP posp;
        Dir dir;//Dir del pare original
    };

  typedef vector<int> VEC;
  typedef vector<VEC> MAT;



  

  vector<Dir> vdir{BOTTOM, RIGHT, TOP, LEFT};

  map<Pos,int> d;

    struct PosComp{
        //bool operator()(const PosP& a, const PosP& b) {return a.first.i != b.first.i ? a.first.i < b.first.i : a.first.j < b.first.j;}
         bool operator()(const PosA& a, const PosA& b) {return a.posp.second > b.posp.second;}
    };

    MAT pos_orks;

    int prodIJ(Pos a) {return a.i * a.j;}


    Dir direccio(Pos a, Pos b){
        if(a + BOTTOM == b) return BOTTOM;
        else if (a + TOP == b) return TOP;
        else if (a + RIGHT == b) return RIGHT;
        else return LEFT;
    }


  //Dijkstra toooo perraco
  Dir cerca(int id){
    int size = rows() * cols();
    Unit u = unit(id); //unitat amb identificador id
    cerr<< "Ork numero: " << id << endl;
    Pos pos = u.pos; //Posicio d'aquesta unitat u
    cerr << "Posicio inicial: "<<pos << endl;
    d[pos] = 0;//Posicio inicial -> te distancia 0
    vector<bool> S(size, false);//Vector de nodes visitats
    priority_queue<PosA, vector<PosA>, PosComp > Q;
    PosA pa;
    pa.posp.first = pos; //posicio
    pa.posp.second = 0; //valor de distancia
    pa.dir = NONE; // direccio pare
    Q.push(pa);
    while (not Q.empty()){
        cerr<< "Tornem a comencar"<< endl;
        PosA ppa = Q.top();
        Pos posicio = Q.top().posp.first;
        Q.pop();
        cerr<< "Ara amb la posicio: "<< posicio << endl;
        int coordPos = prodIJ(posicio);
        if(not S[prodIJ(posicio)]){ //Si la posicio no esta visitada
            S[prodIJ(posicio)] = true;
            VEC ranvec = random_permutation(vdir.size());
            for(int i = 0; i < 4; ++i){
                Pos posi = posicio + Dir(ranvec[i]);//Posicio meva + nova direccio
                cerr<< "La nova direccio es: "<< Dir(i) <<endl;
                cerr << "I la nova posicio a evaluar: "<<posi<< endl;
                int coord = prodIJ(posi);
                Cell c = cell(posi);//Cela de la nova posicio a comprovar
                if(pos_ok(posi) and c.type != WATER){
                    int costvida = cost(c.type);//Cost en vida de la nova cela
                    auto it = d.find(posi);
                    if(it == d.end() or d[posi] > d[posicio] + costvida){
                        d[posi] = d[posicio] + costvida;
                        if((c.type == CITY and c.city_id != me()) or (c.type == PATH and c.path_id != me())){
                            if(ppa.dir == NONE) return Dir(ranvec[i]);
                            else return ppa.dir;
                        }
                        cerr <<"Enquem cela" <<posi << endl;
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
    return LEFT;
  }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    
    if(round() == 0){
      //pos_orks = MAT(rows(), VEC(cols(), -1));
    
/*
    //Definieix la posicio de tots els orks en el taulell a cada ronda
    //En teoria serveix per comparar la ronda anterior amb l'actual
    for(int i = 0; i < rows(); ++i){
      for(int j = 0; j < cols(); ++j){
        pos_orks[i][j] = cell(i,j).unit_id;
      }
    }
*/
    //vector de id's dels meus orks
    VEC my_orks = orks(me());
    
    /*
    //Per tots els orks, executa una ordre de moviment
    for(unsigned int i = 0; i < my_orks.size(); ++i){
      execute(Command(my_orks[i],cerca(my_orks[i])));
    }
    */
   
    }
    VEC my_orks = orks(me());
    int i = 11;
    execute(Command(my_orks[i],cerca(my_orks[i])));
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
