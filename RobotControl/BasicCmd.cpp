#include "StdAfx.h"
#include "BasicCmd.h"
#include "WorldModel.h"
#include <cmath>

BasicCmd::BasicCmd(): CmdGenerator("BasicCmd")
{
	type_set = -1;
	vx = 0.0;
	vy = 0.0;
	w  = 0.0;
	vtrans = Vec::zero_vector;
	vrot   = 0.0;
	bkick  = false;
	powerkick  = 0;
	for (unsigned int i = 0; i<4; i++)
		wheelSpeed[i] = 0.0;
}

void BasicCmd::set_kick(BYTE power)
{
	type = KICK;
 	this->power = power;
}

void BasicCmd::set_turn(double moment)
{
	type = TURN;
	this->moment = moment;
}

void BasicCmd::set_dash(double velocity,double direction,double omega)
{
	type = DASH;
	vx = velocity * cos(direction);
	vy = velocity * sin(direction);
	w  = omega;
}

void BasicCmd::set_move(double velocity,double angle,double omega)
{
	type = MOVE;
	double RVx = 0.0, RVy = 0.0, ROme = 0.0;  
	RVx  = velocity * cos(angle);
	RVy  = velocity * sin(angle);
	ROme = omega;
    vtrans = Vec(RVx,RVy) ;
	vrot   = ROme;
}

void BasicCmd::set_turnMove(double velocity,double angle,double omega)
{
	type = TURN_MOVE;

	double _xm = 0.0, _ym = 0.0, _phim = 0.0;
 
	_xm   = velocity * cos(angle);
	_ym   = velocity * sin(angle);
	_phim = omega;
	vrot   = _phim;

	double vy_offset = vrot > 0 ? -0.8 :( vrot == 0? 0: + 0.8 );
    vtrans = Vec(_xm,_ym + vy_offset );
	
	// need to debug
 /*	wheelSpeed[0] = ( -70.7 * ( _xm + _ym )   + _phim  * 22.5 ) * 0.4233; 
	wheelSpeed[1] = ( 50.0 * _ym - 86.6 * _xm + _phim  * 22.5 ) * 0.4233;
	wheelSpeed[2] = ( 86.6 * _xm + 50.0 * _ym + _phim  * 22.5 ) * 0.4233;
	wheelSpeed[3] = ( 70.7 * ( _xm - _ym )    + _phim  * 22.5 ) * 0.4233;*/
}

void BasicCmd::set_uturn()
{
	type = U_TURN;
	double _xm = 0.0, _ym = 0.0, _phim = 0.0;
 
 	// need to debug
 	wheelSpeed[0] = ( -70.7 * ( _xm + _ym )   + _phim  * 22.5 ) * 0.4233; 
	wheelSpeed[1] = ( 50.0 * _ym - 86.6 * _xm + _phim  * 22.5 ) * 0.4233;
	wheelSpeed[2] = ( 86.6 * _xm + 50.0 * _ym + _phim  * 22.5 ) * 0.4233;
	wheelSpeed[3] = ( 70.7 * ( _xm - _ym )    + _phim  * 22.5 ) * 0.4233;
}

void BasicCmd::set_stop()
{
	type = STOP;
	vtrans = Vec::zero_vector;
	vrot   = 0.0;
	this->power = 0.0;
	bkick = false;
	for (unsigned int i = 0; i<4; i++)
		wheelSpeed[i] = 0.0;
}

void BasicCmd::set_rotate(int way)
{
	type = ROTATE;
    vtrans = Vec( 0.5, 1.5*way );   // 0.8 1.0 -1.8
	vrot   = -2.0 * way;
}

void BasicCmd::getCmd(DriveVector *dv, const Time&)
{
	switch(type)
	{
	case KICK:
		dv->kick = true;
		dv->kickballtime = power;
		break;
	case DASH:
		dv->vtrans = Vec(vx,vy);
		dv->vrot   = w;
		break;
	case TURN:
		dv->vtrans = Vec::zero_vector;
		dv->vrot   = moment;
		break;
	case MOVE:
		dv->vtrans = this->vtrans;
		dv->vrot   = this->vrot;
		break;
	case TURN_MOVE:
		//*dv = DriveVector(	wheelSpeed[0],wheelSpeed[1],wheelSpeed[2],
		//					wheelSpeed[3],false,0,WHEELVELOCITY);
		dv->vtrans = this->vtrans;
		dv->vrot   = this->vrot;
		break;
	case U_TURN:
		*dv = DriveVector(wheelSpeed[0],wheelSpeed[1],wheelSpeed[2],wheelSpeed[3],false,0,WHEELVELOCITY);
 		break;
	case STOP:
		dv->vtrans = this->vtrans;
		dv->vrot   = this->vrot;
		dv->kick   = false;
		dv->kickballtime = 0;
		break;
	case ROTATE:
		dv->vtrans = this->vtrans;
		dv->vrot   = this->vrot;
		break;
	default:
		break;
	}
}