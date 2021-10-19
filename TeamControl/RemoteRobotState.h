#ifndef _RemoteRobotState_h
#define _RemoteRobotState_h

#include "RobotLocation.h"
#include "BallLocation.h"
#include "GameState.h"
#include "DriveVector.h"
#include "RobotData.h"
#include "MessageBoard.h"
#include "ObstacleLocation.h"
#include "VisibleObject.h"
#include "FieldGeometry.h"
 
struct DirectionOfPlay 
{
    int own_half;       ///< +1 = yellow, -1=blue
    int team_color;   ///< +1 = cyan, -1= magenta
};

struct RemoteRobotState 
{
    std::string name;                       ///< Name  Robots
    std::string ip;                         ///< IP-Adresse
    int port;                               ///< Port  communication
	
    bool comm_started;                      ///< communication init?
    bool comm_okay;                         ///< communication okay?
	
    RobotLocation robot_pos;       ///< Position  Robots
    BallLocation ball_pos;         ///< Position  Balls
	
    int team;                               ///< +1, wenn zum Tribotsteam gehoert, -1 sonst
    DirectionOfPlay tribots_team;           ///< Orientierung von mir
	
    bool in_game;                           ///< Roboter aktiviert?
    RefereeState refstate;         ///< RefereeState des Roboters
    std::string playertype;                 ///< Spielertyp
    std::string playerrole;                 ///< Spielerrolle
	
    bool use_joystick;                      ///< Joystick-Steuerung?
    DriveVector joy_drive;         ///< Drive-Vector des Joysticks
	
    bool robot_data_request;                ///< sollen Roboterdaten uebertragen werden?
    RobotData robot_data;          ///< die Roboterdaten
	
    bool obstacle_data_request;             ///< sollen Hindernisse übertragen werden?
    ObstacleLocation obstacles;    ///< die Hindernisse
	
    bool visible_object_request;            ///< sollen gesehenen Objekte übertragen werden?
    VisibleObjectList visible_objects; ///< die gesehenen Objekte
	
    MessageBoard mboard;           ///< Messages
	
    FieldGeometry fieldgeometry;   ///< die Feldgeometrie, zu Kontrollzwecken
};
 
#endif
