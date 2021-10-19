#ifndef _GameState_
#define _GameState_

#include "MyTime.h"

enum RefereeState{
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
    errorState               
};
 enum RefboxSignal {
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
    SIGopponentGoalScored
  };
     
   struct GameState
   {
    RefereeState refstate;           ///< aktueller Refereestate
    MyTime       latest_update;              ///< zeitpunkt der letzten Aktualisierung von refstate
    bool         in_game;                    ///< true, wenn der Roboter im Spiel ist, false sonst
    RefboxSignal refbox_signal;      ///< das zuletzt empfangene Signal der Refereebox

    unsigned int own_score;          ///< Torerfolge eignes Team
    unsigned int opponent_score;     ///< Torerfolge gegnerisches Team

    double       intended_cycle_time;      ///< vorgesehene Zykluszeit
    double       actual_cycle_time;        ///< tatsaechliche Zykluszeit
    unsigned long int cycle_num;     ///< Zyklusnummer
    MyTime       cycle_start_time;           ///< Zeit, zu der Zyklus gestartet
    MyTime       expected_execution_time;    ///< Zeit, zu der die Umsetzung des Fahrtbefehls erwartet wird
  };


  static const int num_referee_states = 23;                  ///< Anzahl moeglicher Spielsituationen
  static const char referee_state_names [23][23] =
  {          
    "Stop                  ",
    "Freies Spiel          ",
    "vor eigenem Anstoss   ",
    "vor Anstoss Gegner    ",
    "nach Anstoss Gegner   ",
    "vor eigenem Torabstoss",
    "vor Torabstoss Gegner ",
    "nach Torabstoss Gegner",
    "vor eigenem Eckball   ",
    "vor Eckball Gegner    ",
    "nach Eckball Gegner   ",
    "vor eigenem Einwurf   ",
    "vor Einwurf Gegner    ",
    "nach Einwurf Gegner   ",
    "vor eigenem Freistoss ",
    "vor Freistoss Gegner  ",
    "nach Freistoss Gegner ",
    "vor eigenem Strafstoss",
    "vor Strafstoss Gegner ",
    "nach Strafstoss Gegner",
    "eigener Strafstoss    ",
    "Strafstoss Gegner     ",
    "Fehler                "
  };
  

  static const int num_refbox_signals = 33;                  
  static const char refbox_signal_names [33][23] = 
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
    "SIGopponentGoalScored "
  };

#endif