#ifndef _PlayerRole_h
#define _PlayerRole_h
 
enum PlayerRole
{
	Defend1,
	Defend2,
	Defend3,
	Attack1,
	Attack2,
	Support
};

static const unsigned int num_roles = 8;

static const char playerRoleNames [8][20] = {
	"Defend1",
	"Defend2",
	"Defend3",
	"Attack1",
	"Attack2",
	"Support",
	"unknown",
	"unknown"
};    
 

#endif