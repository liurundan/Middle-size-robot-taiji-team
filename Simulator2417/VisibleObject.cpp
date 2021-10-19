#include "StdAfx.h"
#include "VisibleObject.h"

const int  VisibleObject::unknown=0;
const int  VisibleObject::ball=1;
const int  VisibleObject::blue_goal=2;
const int  VisibleObject::yellow_goal=3;
const int  VisibleObject::blue_pole=4;
const int  VisibleObject::yellow_pole=5;
const int  VisibleObject::teammate=6;
const int  VisibleObject::opponent=7;
const int  VisibleObject::obstacle=8;
const int  VisibleObject::white_line=9;
const int  VisibleObject::blue_goal_post_left=10;
const int  VisibleObject::blue_goal_post_right=11;
const int  VisibleObject::yellow_goal_post_left=12;
const int  VisibleObject::yellow_goal_post_right=13;

VisibleObject::VisibleObject () throw () 
{
  pos.X=0;
  pos.Y=0;
  object_type=0;
  width=0;
}

VisibleObject::VisibleObject (Vector pos1, int ot1, double w) throw () : pos(pos1), object_type(ot1), width(w) {;}

VisibleObject::VisibleObject (const VisibleObject& src) throw () : pos(src.pos), object_type(src.object_type), width(src.width) {;}

const  VisibleObject&   VisibleObject::operator= (const  VisibleObject& src) throw () 
{
  pos=src.pos;
  object_type=src.object_type;
  width=src.width;
  return (*this);
}

VisibleObjectList::VisibleObjectList (const  VisibleObjectList& src) throw (std::bad_alloc) : timestamp (src.timestamp), objectlist (src.objectlist) {;}

const  VisibleObjectList&  VisibleObjectList::operator= (const  VisibleObjectList& src) throw (std::bad_alloc)
 {
  timestamp  =  src.timestamp;
  objectlist = src.objectlist;
  return *this;
}  

 