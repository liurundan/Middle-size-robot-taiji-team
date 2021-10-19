#ifndef _ErrorMinimiserSL_
#define _ErrorMinimiserSL_

#include "FieldLUT.h"
#include "OdometryContainer.h"
#include "VisualPositionOptimiser.h"
#include "RobotPositionKalmanFilter.h"
#include "VisibleObject.h"
#include "RobotLocation.h"
#include <fstream>

inline double drand()//产生0－1之间的随机数，返回双精度或浮点型
{ 
  return ((double)(rand() % 100)) / 100;
} 
static const int iNrParticles = 50;

class ErrorMinimiserSL
{
public:
     ErrorMinimiserSL( const OdometryContainer&, const FieldGeometry&) throw (std::bad_alloc);
     ~ErrorMinimiserSL () throw ();
 
    bool update (const VisibleObjectList&, const VisibleObjectList&) throw ();

   
    RobotLocation get (MyTime) const throw ();

     void reset () throw ();
     void reset (Vector) throw ();
 
    void reset (Vector, MyAngle) throw ();

	// for test 
	struct ParPos
	{
      ParPos();
	  ParPos(Vector);
	  ParPos(Vector,MyAngle);
	  Vector  pos;
      MyAngle heading;
	  double  quality;
	    
	};
	ParPos particlesPosAgent[iNrParticles];
 	void   resampleParticles( int iLeft, ParPos pp );
	RobotLocation averageParticles( ParPos posArray[], int iLength );
	void   updateParticles( ParPos pp  );
	int    checkParticles( const VisibleObjectList& lines,RobotLocation delta_xv  );
	void   initParticles(  );
	void   initParticles( ParPos posInitial );
    
protected:
private:
     struct AltPos 
	 {
      AltPos ();                               
      AltPos (Vector);                           
      AltPos (Vector, MyAngle);                     
      RobotPositionKalmanFilter rpos_filter;   
      MyTime init_time;                          
      double winning_coefficient;              
      double latest_error;                     
      bool   heading_checked;                    
      double ema_right_goal;                   
      double ema_wrong_goal;                   
    };

    FieldLUT* field_lut;                        
    VisualPositionOptimiser* vis_optimiser;     
    const OdometryContainer& odobox;           
    MyTime latest_update;                        
    unsigned int cycles_since_reset;            

    unsigned int max_lines;                     

    bool consider_yellow_goal;                 
    bool consider_blue_goal;                   
    bool use_internal_alternatives;             

    AltPos main_position;                       
    unsigned int num_external_alternatives;     
    std::vector<AltPos> external_alternatives;  

    void update_alternative (AltPos&, const VisibleObjectList&, const VisibleObjectList&, bool, bool); 
	// for test 
	RobotLocation rp;
	
};
#endif