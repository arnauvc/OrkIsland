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
  typedef vector<int> VEC;
  typedef vector<VEC> MAT;

  typedef pair<Pos,double> PosP; // posicio i pes

  vector<Dir> vdir{BOTTOM, RIGHT, TOP, LEFT};

    struct PosComp{
        bool operator()(const PosP& a, const PosP& b) {return a.first.i != b.first.i ? a.first.i < b.first.i : a.first.j < b.first.j;}
    };

    MAT pos_orks;
    MAT mapa;

    int prodIJ(Pos a) {return a.i * a.j;}

  //Dijkstra toooo perraco
  Dir cerca(int id){
    int size = rows() * cols();
    Unit u = unit(id); //unitat amb identificador id
    Pos pos = u.pos; //Posicio d'aquesta unitat u
    cerr << "Posicio inicial: "<<pos << endl;
    vector<int> d = vector<int>(size, INT_MAX); //Vector de distancies
    d[prodIJ(pos)] = 0;//Posicio inicial -> te distancia 0
    queue<Pos> QD;//Cua de posicions a seguir
    vector<bool> S(size, false);//Vector de nodes visitats
    priority_queue<PosP, vector<PosP>, PosComp > Q;
    Q.push(PosP(pos,0));
    while (not Q.empty()){
        cerr<< "Tornem a comencar"<< endl;
        Pos posicio = Q.top().first;
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
                    if(d[coord] > d[coordPos] + costvida){
                        d[coord] = d[coordPos] + costvida;
                        QD.push(posi);
                        if(c.type == CITY or c.type == PATH){
                            cerr<< "Hem trobat ciutat o cami " << endl;
                            cerr<< "A la posicio: " << posi << endl;
                            Pos p = QD.front();
                            return NONE;
                        }
                        cerr <<"Enquem cela" <<posi << endl;
                        Q.push(PosP(posi,d[coord]));
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
      //my_orks = MAT(rows(), VEC(cols(), -1));
      mapa = MAT(rows(), VEC(cols()));
    
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
    execute(Command(my_orks[0],cerca(my_orks[0])));
    }
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
