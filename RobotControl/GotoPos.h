#ifndef _GOTOPOS_H
#define _GOTOPOS_H

#include "CmdGenerator.h"
#include "BasicCmd.h"
#include "Avoid.h"

class GotoPos : public CmdGenerator
{
public:
   GotoPos();
   ~GotoPos();
   void set_target( Vec target, double velocity, double target_tolerance = 1000.0, int cons_obstacles = 1, bool usePD = false );
   void getCmd(DriveVector *dv, const Time& t);
   void getCmd(DriveVector *dv, const Time& t, bool usePD ); 
private:
   BasicCmd*  basic_cmd;
   Avoid*     avoid;
   int        osbstacle_found;
   Vec        target_pos;
   Angle      target_heading;
   double     target_tolerance;
   double     velocity;
   static int consider_obstacles;
   bool       usePD;
};
#endif