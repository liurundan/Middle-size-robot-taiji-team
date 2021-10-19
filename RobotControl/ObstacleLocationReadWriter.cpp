#include "StdAfx.h"
#include "ObstacleLocationReadWriter.h"
#include "stringconvert.h"

using namespace std;


ObstacleLocationWriter::ObstacleLocationWriter (std::ostream& d) throw () : dest(d) {;}

ObstacleLocationWriter::~ObstacleLocationWriter () throw () {
  dest << std::flush;
}

void ObstacleLocationWriter::write (unsigned long int tav, const ObstacleLocation& obj) throw () {
  vector<Vec>::const_iterator pit = obj.pos.begin();
  vector<Vec>::const_iterator pitend = obj.pos.end();
  vector<double>::const_iterator wit = obj.width.begin();
  while (pit<pitend) {
    dest << tav << '\t' 
	 << pit->x << '\t' 
	 << pit->y << '\t' 
	 << (*wit) << '\n';
    pit++;
    wit++;
  }
}


ObstacleLocationReader::ObstacleLocationReader (std::istream& s) throw () : src(s), next(0) {
  if (src.good()) src >> next;
}

bool ObstacleLocationReader::next_timestamp (unsigned long int& t) const throw () {
  if (src.eof())
    return false;
  t=next;
  return true;
}

bool ObstacleLocationReader::read_until (unsigned long int& tav, ObstacleLocation& obj, unsigned long int tuntil) throw () {
  tav=next;
  bool read_anything = false;
  obj.pos.clear();
  obj.width.clear();
  while (!src.eof() && next<=tuntil) {
    if (tav!=next) {
      obj.pos.clear();
      obj.width.clear();
      tav=next;
    }
    string line;
    deque<string> parts;
    getline (src, line);
    split_string (parts, line);
    src >> next;
    if (parts.size()>=3) {
      Vec p;
      double w;
      string2double (p.x, parts[0]);
      string2double (p.y, parts[1]);
      string2double (w, parts[2]);
      obj.pos.push_back (p);
      obj.width.push_back (w);
      read_anything=true;
    }
  }
  return read_anything;
}
