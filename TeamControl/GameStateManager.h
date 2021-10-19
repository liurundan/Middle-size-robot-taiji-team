#ifndef _GameStateManager_h
#define _GameStateManager_h

#include "GameState.h"
#include "BallLocation.h"
#include "FieldGeometry.h"
#include "ConfigReader.h"
#include "RefereeStateMachine.h"

class GameStateManager
{
public:
	  
    GameStateManager (  ConfigReader&, const FieldGeometry&) throw ();
     
    ~GameStateManager() throw ();
    
    const GameState& get () const throw ();
    
    void update () throw ();
    
    void update (RefboxSignal) throw ();
     
    void set_in_game (bool) throw ();
  
    void init_cycle (Time, Time) throw ();
 
private:
	GameState gs;                 
    BallLocation ref_ball;        
    RefereeStateMachine rsm;      
};

#endif