#ifndef _SPhysGotoPos_h
#define _SPhysGotoPos_h

#include "CmdGenerator.h"
#include "Avoid.h"

class SPhysGotoPos : public CmdGenerator
{
public:
    SPhysGotoPos () throw ();
	~SPhysGotoPos() throw();
    void getCmd(DriveVector*, const Time&) throw();
    void init (Vec, Angle, bool, bool =true, bool =true ) throw ();
    void init (Vec, Angle, double, bool =true, bool =true) throw ();
    void set_dynamics (double, double, double, double) throw ();
    void get_dynamics (double&, double&, double&, double&, double&, double&) throw ();

    void set_target(bool pd, Vec pos, Angle ang, double vel, double w, int cons_obstacles = 1 ) throw ();
    void set_target(bool pd, Vec pos, double vel, int cons_obstacles = 1  ) throw ();
    void getCmd(DriveVector* dv , const Time& t, bool pd ) throw();

private:
    Vec    target_pos;
    Angle  target_heading;
    double max_target_velocity;
	double max_target_vrot;
	
    bool   tolerance_pos;
    bool   tolerance_heading;
    bool   needToTurn;

    double max_tv;
    double max_rv;
    double max_ta;
    double max_ra;
    double max_td;
    double max_rd;

    int acc_delay;  

	bool   use_pd_control;
	Avoid* avoid;
    static int consider_obstacles;
};

#endif 

