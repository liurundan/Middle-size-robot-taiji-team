#ifndef _OFFENCE_H_
#define _OFFENCE_H_

#include "BasicCmd.h"
#include "BallGet.h"
#include "Dribble.h"

#include "CmdGenerator.h"

class Offence : public CmdGenerator
{
public:
	Offence();
	~Offence();
    void getCmd(DriveVector *dv, const Time& t);
	void OffenceFW(const Time& t,double offenceVelocity);
	void OffenceDF(const Time& t,double offenceVelocity);
	void SideAttack(const Time& t );
	void Shoot(const Time& t);
  
private:
	BallGet*  getBall;
	BasicCmd* basic_cmd;
	Dribble*  dribble;
	DriveVector dest;
	
};

#endif