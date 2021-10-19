#ifndef _SOCCERTYPES_
#define _SOCCERTYPES_
#include "Geometry.h"

/*****************************************************************************/
/********************* DEFINES ***********************************************/
/*****************************************************************************/
#define TIMER_RESOLUTION     1         // 定时器精度 1ms
#define CONTROL_INTERVAL     100       // 控制周期，以毫秒计75

#define Pi 3.14159265358979323846
#define pi 3.14159265358979323846
 
#define OURTEAMNUM 5                   // 我方队员个数
#define OPPTEAMNUM 5                   // 对方队员个数

#define BAUDRATE     "19200" 
#define DATALENGTH   10             
 
/////通讯端口设置//////////////////////////////////////////

#define COACHPORT    5080
#define DISPLAYPORT  5080

#define C2PPORT      5081
#define P2DPORT      5082

#define P2PPORT      5083
#define USBPORT      5090
#define ROBOTPORT    5091

#define LOCALIP       "192.168.11.9"   //"192.168.0.226" // 
#define HOMEIP        "127.0.O.1"

#define COACHIP       "192.168.11.8"  //"192.168.0.226" //
#define DISPLAYIP     "192.168.11.9"   //"192.168.0.226" //
#define BROADCASTIP   "192.168.11.255"   //"192.168.0.255" //
#define TEAMMATEIP    "192.168.11.3"
#define NUMBER        " 5 号"

////////////////////////////////////////////////////////////
////////////  机器人尺寸  //////////////////////////////////
////////////////////////////////////////////////////////////

#define RADIUS       22.5        ///// cm

#define XWARP         0
#define YWARP         0

#define TEAM        1

////////////////////////////////////////////////////////////
////////////////////_____Geometry______/////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////_____Geometry______/////////////////////
////////////////////////////////////////////////////////////
//动作应用的宏

#define MAX_VEL      300.0       ///// cm/s 
#define LIMIT_VEL    150.0       ///// cm/s
#define LIMIT_VEL1    220.0
#define MaxKickDist   45.0
#define MINDISTANCE          10.0
#define MAXDISTANCE          700 //1200.0

#define PITCH_LENGTH          1200.0       /*!< length of the pitch             */
#define PITCH_WIDTH           800.0       /*!< width  of the pitch             */
#define PITCH_MARGIN          50.0       /*!< margin next to the pitch        */
#define PENALTY_AREA_LENGTH   150
#define PENALTY_AREA_WIDTH    500
#define PENALTY_X (PITCH_LENGTH/2.0-PENALTY_AREA_LENGTH)
#define MAX_FLAGS              25
#define MAX_LINES               9       /*!< maximum number of lines on field*/
#define MAX_OBSTACLES          12

#define BLOCKING_BALL_DIST     25.0
#define MIN_USEFUL_CRR         0.25
#define BLOCKING_DIST          150
/*****************************************************************************/
/********************* CONSTANTS *********************************************/
/*****************************************************************************/
//比赛类型

const char OUR_KICKOFF = 0;     //我方开球
const char OPP_KICKOFF = 1;     //对方开球
const char OUR_THROWIN = 2;     //我方开界外球
const char OPP_THROWIN = 3;     //对方开界外球
const char OUR_PENALTY = 4;     //我方罚点球
const char OPP_PENALTY = 5;     //对方罚点球
const char OUR_GOALKICK = 6;    //我方开门球
const char OPP_GOALKICK = 7;    //对方开门球
const char OUR_CORNERKICK = 8;  //我方开角球
const char OPP_CORNERKICK = 9;  //对方开角球
const char OUR_FREEKICK = 10;   //我方开任意球
const char OPP_FREEKICK = 11;   //对方开任意球
const char DROPBALL     = 12;   //双方争球
//const char CANCEL       = 13;
//机器人是否能运动
const char STOPROBOT   = 14;    //机器人停止运动终止或者暂停比赛
const char STARTROBOT  = 15;    //机器人运动开始比赛
//比赛状态
const char OUR_YELLOW = 20;     //我方球门为黄色，进攻蓝色球门
const char OUR_BLUE   = 21;     //我方球门为蓝色，进攻黄色球门

//场地参数
const int FIELDLENGTH=1200; // 1200;        //球场长度，根据不同的场地条件修改
const int FIELDWIDTH =800; // 800;         //球场宽度，根据不同的场地条件修改
const int FIELD_X = 1200;        //球场长度，根据不同的场地条件修改
const int FIELD_Y = 800;         //球场宽度，根据不同的场地条件修改
const int DOOR_Y=200; 
const int GoalWidth = 200;

const int OPPONENTGOALX = 600;     
const int OPPONENTGOALY = 0;
const double  UnknownDoubleValue  = -10000.0;  
const AngDeg  UnknownAngleValue   = -10000.0;  
const int     UnknownIntValue     = -10000;    
const int     UnknownTime         = -20;      

////////////////////////////////////////////////////////////

/*****************************************************************************/
/********************** ENUMERATIONS *****************************************/
/*****************************************************************************/

/*! ObjectT is an enumeration of all possible objects that are part of the
    RoboCup soccer simulation. The class SoccerTypes contains different methods
    to easily work with these objects and convert them to text strings and text
    strings to ObjectT. */
enum ObjectT 
{ // don't change order
  OBJECT_BALL,             //Ball                     
  OBJECT_LINE_0,           // Left line                
  OBJECT_LINE_1,           // Right line               
  OBJECT_LINE_2,           // Bottom line              
  OBJECT_LINE_3,
  OBJECT_LINE_4,                      
  OBJECT_LINE_5,                       
  OBJECT_LINE_6,                        
  OBJECT_LINE_7,
  OBJECT_LINE_8,                
  OBJECT_MARKPOLE_BLUE_0,         // Flag left top           
  OBJECT_MARKPOLE_BLUE_1,      // Flag top left 50         
  OBJECT_MARKPOLE_YELLOW_2,      //Flag top left 40          
  OBJECT_MARKPOLE_YELLOW_3,      // Flag top left 30          
  OBJECT_GOAL_BLUE,           // Left goal                
  OBJECT_GOAL_YELLOW,           // Right goal               
  
  OBJECT_FLAG_YE_T,         /*!< Flag left top            */   // 19 flags
    
  OBJECT_FLAG_C_T,         /*!< Flag top center          */
   
  OBJECT_FLAG_BL_T,         /*!< Flag right top           */
   
  OBJECT_FLAG_G_BL_T,       /*!< Flag goal right top      */
  OBJECT_FLAG_BL_0,         /*!< Flag right 0             */
  OBJECT_FLAG_G_BL_B,       /*!< Flag goal right bottom   */
   
  OBJECT_FLAG_BL_B,         /*!< Flag right bottom        */
  
  OBJECT_FLAG_C_B,         /*!< Flag center bottom       */
   
  OBJECT_FLAG_YE_B,         /*!< Flag left bottom         */
  
  OBJECT_FLAG_G_YE_B,       /*!< Flag goal left bottom    */
  OBJECT_FLAG_YE_0,         /*!< Flag left 0              */
  OBJECT_FLAG_G_YE_T,       /*!< Flag goal left top       */
  
  OBJECT_FLAG_P_YE_T,       /*!< Flag penaly left top     */
  OBJECT_FLAG_P_YE_C,       /*!< Flag penaly left center  */
  OBJECT_FLAG_P_YE_B,       /*!< Flag penaly left bottom  */
  OBJECT_FLAG_P_BL_T,       /*!< Flag penaly right top    */
  OBJECT_FLAG_P_BL_C,       /*!< Flag penaly right center */
  OBJECT_FLAG_P_BL_B,       /*!< Flag penaly right bottom */
  
  OBJECT_FLAG_C,           /*!< Flag center field        */
  //lrd//
  OBJECT_GOAL_UNKNOWN,     // Unknown goal   
  OBJECT_TEAMMATE_1,       // Teammate nr 1            
  OBJECT_TEAMMATE_2,       //< Teammate nr 2            
  OBJECT_TEAMMATE_3,       // Teammate nr 3           
  OBJECT_TEAMMATE_4,                  
  OBJECT_TEAMMATE_5,                    
  OBJECT_TEAMMATE_6,                  
  OBJECT_TEAMMATE_UNKNOWN, // Teammate nr unkown       
  OBJECT_OPPONENT_1,       // Opponent nr 1            
  OBJECT_OPPONENT_2,       // Opponent nr 2            
  OBJECT_OPPONENT_3,       // Opponent nr 3             
  OBJECT_OPPONENT_4,       // Opponent nr 4            
  OBJECT_OPPONENT_5,       // Opponent nr 5             
  OBJECT_OPPONENT_6,       // Opponent nr 5            
  OBJECT_OPPONENT_UNKNOWN, // Opponent nr unknown       
  
  OBJECT_PLAYER_UNKNOWN_0,
  OBJECT_PLAYER_UNKNOWN_1,   // Unknown player            
  OBJECT_PLAYER_UNKNOWN_2,
  OBJECT_PLAYER_UNKNOWN_3,   // Unknown player            
  OBJECT_PLAYER_UNKNOWN_4,
  OBJECT_PLAYER_UNKNOWN_5,   // Unknown player            
  OBJECT_PLAYER_UNKNOWN_6,
  OBJECT_PLAYER_UNKNOWN_7,   // Unknown player            
  OBJECT_PLAYER_UNKNOWN_8,
  OBJECT_PLAYER_UNKNOWN_9,   // Unknown player            
  OBJECT_PLAYER_UNKNOWN_10,
  OBJECT_PLAYER_UNKNOWN_11,   // Unknown player            
  
  OBJECT_UNKNOWN,          // Unknown object            
  OBJECT_TEAMMATE_GOALIE,  // Goalie of your side       
  OBJECT_OPPONENT_GOALIE,  /// Goalie of opponent side  
  OBJECT_ILLEGAL,          // illegal object            
  OBJECT_MAX_OBJECTS       // maximum nr of objects     
};
 
enum SideT 
{
  SIDE_YELLOW,     // left side    
  SIDE_BLUE,       // right SIDE   
  SIDE_ILLEGAL     // illegal SIDE 
};
enum PlayModeT 
{
  PM_OUR_KICKOFF,      //我方开球
  PM_OPP_KICKOFF,      //对方开球
  PM_OUR_THROWIN ,      //我方开界外球
  PM_OPP_THROWIN ,    //对方开界外球
  PM_OUR_PENALTY ,      //我方罚点球
  PM_OPP_PENALTY ,      //对方罚点球
  PM_OUR_GOALKICK,     //我方开门球
  PM_OPP_GOALKICK ,    //对方开门球
  PM_OUR_CORNERKICK,  //我方开角球
  PM_OPP_CORNERKICK , //对方开角球
  PM_OUR_FREEKICK,    //我方开任意球
  PM_OPP_FREEKICK,   //对方开任意球
  PM_DROPBALL,       //双方争球      
  PM_CANCEL,          // quit            
  PM_BEFORE_KICK_OFF,
  PM_PLAY_ON,     // play_on:                                                      
  PM_FROZEN,
  PM_TIME_OVER,
  PM_ILLEGAL       // unknown playmode                                    
};
enum ObjectSetT
{
  OBJECT_SET_TEAMMATES,        //teammates                       
  OBJECT_SET_OPPONENTS,        // opponents                       
  OBJECT_SET_PLAYERS,          // players                         
  OBJECT_SET_TEAMMATES_NO_GOALIE,// teammates without the goalie  
  OBJECT_SET_FLAGS,            /// flags                           
  OBJECT_SET_LINES,            // lines                           
  OBJECT_SET_ILLEGAL           // illegal                         
};

enum PlayerT {
  PT_ILLEGAL,             /*!< illegal player type */
  PT_GOALKEEPER,          /*!< goalkeeper          */
  PT_DEFENDER1,    /*!< central defender    */
  PT_DEFENDER2,
  PT_DEFENDER3,
  PT_MIDFIELDER,   /*!< central midfielder  */
  PT_ATTACKER1,            /*!< central attacker    */
  PT_ATTACKER2,
  MAX_PLAYER_TYPES
};

/*****************************************************************************/
/********************** CLASS SOCCERTYPES ************************************/
/*****************************************************************************/

/*! The class SoccerTypes contains different methods to work with the
    different enumerations defined in SoccerTypes.h. It is possible to
    convert soccertypes to strings and strings to soccertypes. It is
    also possible to get more specific information about some of the
    soccertypes. All methods are static so it is possible to call the
    methods without instantiating the class. */
class SoccerTypes
{
public:
	static ObjectT         getLineObjectFromIndex( int  iIndex );
	static ObjectT         getObstacleObjectFromIndex( int  iIndex );
	static ObjectT         getFlagObjectFromIndex( int i );
    static bool            isFlag                    ( ObjectT      o          );
	static bool            isPole                    ( ObjectT      o          );
	static int             getIndex                  ( ObjectT o );
	static bool            isLine                    ( ObjectT      o          );
	static bool            isGoal                    ( ObjectT      o          );
	static ObjectT         getOwnGoal                ( SideT        s          );
	static ObjectT         getGoalOpponent           ( SideT        s          );
	static bool            isBall                    ( ObjectT      o          );
    static bool            isTeammate                ( ObjectT      o          );
    static bool            isOpponent                ( ObjectT      o          );
    static bool            isGoalie                  ( ObjectT      o          );
    static bool            isPlayer                  ( ObjectT      o          );
	static bool            isKnownPlayer             ( ObjectT      o          );
	static ObjectT         getTeammateObjectFromIndex( int          iIndex     );
    static ObjectT         getOpponentObjectFromIndex( int          iIndex     );
	static Vector          getGlobalPositionFlag     ( ObjectT      o,
                                                       SideT        s,
                                                       double dGoalWidth = 200.0);
	static bool            isInSet                   ( ObjectT      o,
                                                     ObjectSetT   o_s,
                                                     ObjectT      objectGoalie=
                                                       OBJECT_TEAMMATE_1     );
	static AngDeg          getGlobalAngleLine        ( ObjectT      o,SideT        s);
};

/*****************************************************************************/
/********************** CLASS TIME   *****************************************/
/*****************************************************************************/

/*! This class contains the time representation of the soccer server.
    It is represented by an ordered pair (t,s) where t denotes the current
    server cycle and s is the number of cycles since the clock has stopped.
    Here the value for t equals that of the time stamp contained
    in the last message received from the server, whereas the value $s$ will
    always be 0 while the game is in progress. It is only during certain dead
    ball situations (e.g. an offside call leading to a free kick) that this
    value will be different, since in these cases the server time will stop
    while cycles continue to pass (i.e. actions can still be performed).
    Representing the time in this way has the advantage that it allows the
    players to reason about the number of cycles between events in a meaningful
    way. */
class Time
{
  long int m_iTime;            // Number of cycles, denoting the time  
  long int m_iCycle;
  long int sec;                // 
  static const Time starting_time;
public:
  Time( long int     iTime = -1 );
  bool        updateTime       ( long int     iTime                        );
  long int    getTime          (                                      );
  long int    getTimeDifference( Time    t                            );
  
  bool        updateCycle       ( long int     iCycle                        );
  long int    getCycle          (                                      );
  long int    getCycleDifference( Time    t                            );
  // overloaded arithmetic operators
  Time   operator +       ( Time t );
  long int    operator -       ( Time t );
  void   operator +=      ( Time t );
  void   operator -=      ( Time t );
  bool   operator !=      ( Time t );
  bool   operator !=      ( const long int  &i );
  bool   operator ==      ( Time t );
  bool   operator ==      ( const long int  &i );
  bool   operator <       ( Time t );
  bool   operator <       ( const long int  &i );
  bool   operator <=      ( Time t );
  bool   operator <=      ( const long int  &i );
  bool   operator >       ( Time t );
  bool   operator >       ( const long int  &i );
  bool   operator >=      ( Time t );
  bool   operator >=      ( const long int  &i );
};
 
#endif