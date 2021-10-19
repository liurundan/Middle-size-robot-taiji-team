#ifndef _WHITEBOARD_H_
#define _WHITEBOARD_H_

#include "Time.h"
#include "Frame2D.h"
#include "geometry.h"
#include "PlayerRole.h"
#include <vector>
#include <string>

class WhiteBoard
{
public:
	class Data 
	{
       public:
         Time t;
         unsigned long cycle;
       virtual ~Data() {}
    };
   
    static WhiteBoard* getTheWhiteBoard();
 
    const Frame2d& getAbs2RelFrame(const Time& t);
    const Frame2d& getRel2AbsFrame(const Time& t);
 
    bool isBallInOppGoal(const Time& t);
    bool doPossesBall(const Time& t);
  
    bool m_bDoNotGetBallPossession;
   
    bool teamPossesBall();
    
    bool advancedTeamPossesBall();    

    void kickOffPosition(Vec&, Angle&);
   
    const PlayerRole getPlayerRole();
   
    void changePlayerRole(PlayerRole);
    void checkMessageBoard();
    std::vector<std::pair<std::string,Vec> > getTeammate();
	
    virtual ~WhiteBoard();
	
	void set_small_camera_ball(BYTE p);
    BYTE get_small_camera_ball();
	
	bool shall_I_get_ball();
	bool m_bShall_I_get_ball;

protected:
	WhiteBoard();
    static WhiteBoard* instance;
	static void destroy();

    Data* ballInOppGoalData;
    Data* possesBallData;
    Data* abs2relData;
    Data* rel2absData;

    PlayerRole playerRole;

    int frames_ball_owned;
    unsigned int cycles_without_team_posses_ball;
    unsigned int cycles_without_advanced_team_posses_ball;    
    
	unsigned int cycles_ball_free;
    Vec kick_off_pos;
    Angle kick_off_heading;
    
	std::vector<std::pair<std::string,Vec> > teammate;

    class BoolData : public Data
	{
      public:
        bool b;
    };
     
    class Frame2dData : public Data 
	{
    public:
      Frame2d frame;
    };

	BYTE small_camera_ball_dir;

  
};

#define WBOARD WhiteBoard::getTheWhiteBoard()
#endif