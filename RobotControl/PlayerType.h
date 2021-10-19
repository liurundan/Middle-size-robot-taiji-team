#ifndef _player_type_h
#define _player_type_h

#include "DriveVector.h"
#include "Time.h"

class PlayerType
{
public:
    virtual ~PlayerType () throw () {;}
    virtual DriveVector process_drive_vector (Time) throw ()=0;
};

#endif

