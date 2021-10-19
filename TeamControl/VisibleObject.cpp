#include "StdAfx.h"
#include "VisibleObject.h"
#include <limits>
 
using namespace std;

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
  pos.x=0;
  pos.y=0;
  object_type=0;
  width=0;
}

VisibleObject::VisibleObject (Vec pos1, int ot1, double w) throw () : 
pos(pos1), object_type(ot1), width(w) {;}

VisibleObject::VisibleObject (const VisibleObject& src) throw () : 
 pos(src.pos), object_type(src.object_type), width(src.width) {;}

const  VisibleObject&  VisibleObject::operator= (const  VisibleObject& src) throw ()
{
  pos=src.pos;
  object_type=src.object_type;
  width=src.width;
  return (*this);
}

VisibleObjectList::VisibleObjectList (const  VisibleObjectList& src) throw (std::bad_alloc) : 
timestamp (src.timestamp), objectlist (src.objectlist) {;}

const  VisibleObjectList&  VisibleObjectList::operator= (const  VisibleObjectList& src) throw (std::bad_alloc) 
{
  timestamp = src.timestamp;
  objectlist = src.objectlist;
  return *this;
}                                              

void  VisibleObjectList::writeAt(std::ostream &stream) const
{
  for (unsigned int i=0; i< objectlist.size(); i++)
    stream << "("<< objectlist[i].pos.x << "," << objectlist[i].pos.y << ";" 
	   << objectlist[i].width << "," << objectlist[i].object_type <<  ")";
}

int  VisibleObjectList::readFrom(std::istream &stream)
{
  int n=0;
  objectlist.clear();
 
  do 
  {
    VisibleObject o;
    
    stream.ignore( (numeric_limits<std::streamsize>::max)(),'(');
    stream >> o.pos.x;
    if (stream.fail() || stream.eof()) break;
    stream.ignore( (numeric_limits<std::streamsize>::max)(),',');
    stream >> o.pos.y;
    if (stream.fail() || stream.eof()) break;
    stream.ignore( (numeric_limits<std::streamsize>::max)(),';');
    stream >> o.width;
    if (stream.fail() || stream.eof()) break;
    stream.ignore( (numeric_limits<std::streamsize>::max)(),',');
    stream >> o.object_type;
    if (stream.fail() || stream.eof()) break;
    stream.ignore( (numeric_limits<std::streamsize>::max)(),')');
    
    objectlist.push_back(o);
    n++;
  } 
  while (!(stream.fail() || stream.eof()));
  
  return n;
}


