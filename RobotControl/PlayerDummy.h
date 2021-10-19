#ifndef _player_dummy_h
#define _player_dummy_h

#include "PlayerType.h"
#include "WorldModel.h"
#include "ConfigReader.h"
#include "PhysicalMotionModel.h"

class PlayerDummy : public PlayerType
{
public:
	PlayerDummy ( ConfigReader&) throw ();
    ~PlayerDummy () throw () {;}
    DriveVector process_drive_vector (Time tt) throw ();
protected:
    PhysicalMotionModel motion;
private:
	Vec vtrans;
    double vrot;
    double vaux[4];
    DriveVectorMode dv_mode;

};

#endif

