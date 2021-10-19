#ifndef _GameState_h
#define _GameState_h

#include "Time.h"

enum RefereeState 
{ 
    stopRobot,               
	freePlay,                 
	preOwnKickOff,           
	preOpponentKickOff,       
	postOpponentKickOff,      
	preOwnGoalKick,          
	preOpponentGoalKick,     
	postOpponentGoalKick,     
	preOwnCornerKick,        
	preOpponentCornerKick,   
	postOpponentCornerKick,   
	preOwnThrowIn,           
	preOpponentThrowIn,      
	postOpponentThrowIn,      
	preOwnFreeKick,          
	preOpponentFreeKick,      
	postOpponentFreeKick,     
	preOwnPenalty,           
	preOpponentPenalty,      
	postOpponentPenalty,     
	ownPenalty,              
	opponentPenalty,         
	errorState,
	preDroppedBall
};

enum RefboxSignal 
{
    SIGnop,                  
	SIGstop,
	SIGhalt,
	SIGstart,
	SIGready,
	SIGcyanKickOff,
	SIGmagentaKickOff,
	SIGownKickOff,
	SIGopponentKickOff,
	SIGcyanFreeKick,
	SIGmagentaFreeKick,
	SIGownFreeKick,
	SIGopponentFreeKick,
	SIGcyanGoalKick,
	SIGmagentaGoalKick,
	SIGownGoalKick,
	SIGopponentGoalKick,
	SIGcyanCornerKick,
	SIGmagentaCornerKick,
	SIGownCornerKick,
	SIGopponentCornerKick,
	SIGcyanThrowIn,
	SIGmagentaThrowIn,
	SIGownThrowIn,
	SIGopponentThrowIn,
	SIGcyanPenalty,
	SIGmagentaPenalty,
	SIGownPenalty,
	SIGopponentPenalty,
	SIGcyanGoalScored,
	SIGmagentaGoalScored,
	SIGownGoalScored,
	SIGopponentGoalScored,
	SIGdroppedBall
};

struct GameState 
{
    RefereeState refstate;            
    Time latest_update;              
    bool in_game;                     
    RefboxSignal refbox_signal;      
	
    unsigned int own_score;           
    unsigned int opponent_score;     
	
    double intended_cycle_time;       
    double actual_cycle_time;         
    unsigned long int cycle_num;     
    Time cycle_start_time;            
    Time expected_execution_time;     
};

static const int num_referee_states = 24;                  
static const char referee_state_names [24][23] = 
{         
    "Stop                  ",
	"Free play             ",
	"before own kickoff    ",
	"before opp kickoff    ",
	"after opp kickoff     ",
	"before own goalkick   ",
	"before opp goalkick   ",
	"after opp goalkick    ",
	"before own cornerkick ",
	"before opp cornerkick ",
	"after opp cornerkick  ",
	"before own throwin    ",
	"before opp throwin    ",
	"after opp throwin     ",
	"before own freekick   ",
	"before opp freekick   ",
	"after opp freekick    ",
	"before own penalty    ",
	"before opp penalty    ",
	"after opp penalty     ",
	"own penalty           ",
	"opp penalty           ",
	"error state           ",
	"before dropped ball   "
};

static const int num_refbox_signals = 34;                  
static const char refbox_signal_names [34][23] = 
{         
    "SIGnop                ",
	"SIGstop               ",
	"SIGhalt               ",
	"SIGstart              ",
	"SIGready              ",
	"SIGcyanKickOff        ",
	"SIGmagentaKickOff     ",
	"SIGownKickOff         ",
	"SIGopponentKickOff    ",
	"SIGcyanFreeKick       ",
	"SIGmagentaFreeKick    ",
	"SIGownFreeKick        ",
	"SIGopponentFreeKick   ",
	"SIGcyanGoalKick       ",
	"SIGmagentaGoalKick    ",
	"SIGownGoalKick        ",
	"SIGopponentGoalKick   ",
	"SIGcyanCornerKick     ",
	"SIGmagentaCornerKick  ",
	"SIGownCornerKick      ",
	"SIGopponentCornerKick ",
	"SIGcyanThrowIn        ",
	"SIGmagentaThrowIn     ",
	"SIGownThrowIn         ",
	"SIGopponentThrowIn    ",
	"SIGcyanPenalty        ",
	"SIGmagentaPenalty     ",
	"SIGownPenalty         ",
	"SIGopponentPenalty    ",
	"SIGcyanGoalScored     ",
	"SIGmagentaGoalScored  ",
	"SIGownGoalScored      ",
	"SIGopponentGoalScored ",
	"SIGdroppedBall        "

};

#endif