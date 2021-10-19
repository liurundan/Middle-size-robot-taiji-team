#ifndef _visible_object_h
#define _visible_object_h

#include "Time.h"
#include "Vec.h"
#include <vector>

struct VisibleObject 
{
	VisibleObject () throw ();

	VisibleObject (Vec, int =0, double =0) throw ();
	VisibleObject (const VisibleObject&) throw ();
	const VisibleObject& operator= (const VisibleObject&) throw ();

	Vec pos;            
	int object_type;     
	double width;       
	static const int unknown;       
	static const int ball;          
	static const int blue_goal;     
	static const int yellow_goal;   
	static const int blue_goal_post_left;      
	static const int yellow_goal_post_left;    
	static const int blue_goal_post_right;     
	static const int yellow_goal_post_right;  
	static const int blue_pole;    
	static const int yellow_pole;   
	static const int teammate;      
	static const int opponent;     
	static const int obstacle;      
	static const int white_line;    

};


struct VisibleObjectList 
{
	Time timestamp;                         
	std::vector<VisibleObject> objectlist;  

	VisibleObjectList (unsigned int len =0) throw () : objectlist(len) {;}
	~VisibleObjectList () throw () {;}
	VisibleObjectList (const VisibleObjectList&) throw (std::bad_alloc);
	const VisibleObjectList& operator= (const VisibleObjectList&) throw (std::bad_alloc);
 
	void writeAt(std::ostream &stream) const;
 
	int  readFrom(std::istream &stream);
};

 

#endif