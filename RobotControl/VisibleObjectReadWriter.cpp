#include "StdAfx.h"
#include "VisibleObjectReadWriter.h"

#include "stringconvert.h"

using namespace std;
 
VisibleObjectWriter::VisibleObjectWriter (std::ostream& d) throw () : dest(d) {;}

VisibleObjectWriter::~VisibleObjectWriter () throw () {
  dest << std::flush;
}

void VisibleObjectWriter::write (unsigned long int tav, unsigned long int trec, const VisibleObject& obj) throw () {
  dest << tav << '\t' 
       << trec << '\t' 
       << obj.pos.x << '\t' 
       << obj.pos.y << '\t' 
       << obj.object_type << '\t' 
       << obj.width << '\n';
}

void VisibleObjectWriter::write (unsigned long int tav, unsigned long int trec, const VisibleObjectList& obj) throw () {
  for (unsigned int i=0; i<obj.objectlist.size(); i++) 
    dest << tav << '\t' 
	 << trec << '\t' 
	 << obj.objectlist[i].pos.x << '\t' 
	 << obj.objectlist[i].pos.y << '\t' 
	 << obj.objectlist[i].object_type << '\t' 
	 << obj.objectlist[i].width << '\n';
}


VisibleObjectReader::VisibleObjectReader (std::istream& s) throw () : src(s), next(0) {
  if (src.good()) src >> next;
}

bool VisibleObjectReader::next_timestamp (unsigned long int& t) const throw () {
  if (src.eof())
    return false;
  t=next;
  return true;
}

bool VisibleObjectReader::read_until (unsigned long int& tav, unsigned long int& trec, VisibleObjectList& obj, unsigned long int tuntil) throw () {
  tav=next;
  bool read_anything = false;
  obj.objectlist.clear();
  while (!src.eof() && next<=tuntil) {
    if (tav!=next) {
      obj.objectlist.clear();
      tav=next;
    }
    string line;
    deque<string> parts;
    getline (src, line);
    split_string (parts, line);
    src >> next;
    if (parts.size()>=5) {
      VisibleObject dest;
      string2ulint (trec, parts[0]);
      string2double (dest.pos.x, parts[1]);
      string2double (dest.pos.y, parts[2]);
      string2int (dest.object_type, parts[3]);
      string2double (dest.width, parts[4]);
      obj.objectlist.push_back (dest);
      read_anything=true;
    }
  }
  obj.timestamp.set_msec (trec);
  return read_anything;
}
