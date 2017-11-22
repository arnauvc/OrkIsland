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

  MAT pos_orks;
  MAT mapa;

  //Dijkstra toooo perraco
  Dir cerca(int id){
    Unit u = unit(id); //unitat amb identificador id
    Pos pos = u.pos; //Posicio d'aquesta unitat u

    return NONE;
  }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    
    if(round() == 0){
      //my_orks = MAT(rows(), VEC(cols(), -1));
      mapa = MAT(rows(), VEC(cols()));
      cerr<< "Numero files*columnes: " << rows()*cols()<< endl;
    }
    
/*
    //Definieix la posicio de tots els orks en el taulell a cada ronda
    for(int i = 0; i < rows(); ++i){
      for(int j = 0; j < cols(); ++j){
        pos_orks[i][j] = cell(i,j).unit_id;
      }
    }
*/
    //vector de id's dels meus orks
    VEC my_orks = orks(me());
    
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
