#include "StdAfx.h"
#include "ObstacleLocation.h"
#include <limits>

#include "ObstacleLocation.h"
  
using namespace std;

ObstacleLocation::ObstacleLocation (unsigned int n) throw (std::bad_alloc) : pos (n), width(n) {;}

ObstacleLocation::ObstacleLocation (const ObstacleLocation& ol) throw (std::bad_alloc) 
   : pos(ol.pos), width(ol.width) {;}

void ObstacleLocation::writeAt(std::ostream &stream) const
{
  for (unsigned int i=0; i< pos.size(); i++)
    stream << "("<< pos[i].x << "," << pos[i].y << ";" << width[i] << ")";
}

int ObstacleLocation::readFrom(std::istream &stream)
{
  int n=0;
  pos.clear();
  width.clear();
 
  do 
  {
    Vec p;
    double w;
    stream.ignore((std::numeric_limits<std::streamsize>::max)(),'(');
    stream >> p.x;
    if (stream.fail() || stream.eof()) break;
    stream.ignore((std::numeric_limits<std::streamsize>::max)(),',');
    stream >> p.y;
    if (stream.fail() || stream.eof()) break;
    stream.ignore((std::numeric_limits<std::streamsize>::max)(),';');
    stream >> w;
    if (stream.fail() || stream.eof()) break;
    stream.ignore((std::numeric_limits<std::streamsize>::max)(),')');
    
    pos.push_back(p);
    width.push_back(w);
    n++;
  } 
  while (!(stream.fail() || stream.eof()));
  
  return n;
}
