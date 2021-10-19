#ifndef _COMMAND_
#define _COMMAND_
#include "WorldModel.h"
#include "MessageBoard.h"
#include "DriveVector.h"

class Command
{
protected:
   MessageBoard    *MSG2;

   WorldModel      *WM;
   Vector          vme;
   Vector          vrotate;

   Vector          speedData[8];
   
public:
   DriveVector getDriveVector();
   void stop();
   Command( MessageBoard *msg, WorldModel *wm );
   ~Command();
   void   moveTo( Vector posTo );
   void   turnTo( Vector pos   );
   void   getBall( Vector pos);

   void   SetSpeed();
};

#endif