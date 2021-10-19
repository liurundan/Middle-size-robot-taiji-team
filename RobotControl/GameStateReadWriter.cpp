#include "StdAfx.h"
#include "GameStateReadWriter.h"
#include "stringconvert.h"

using namespace std;


GameStateWriter::GameStateWriter (std::ostream& d) throw () : dest(d) {;}

GameStateWriter::~GameStateWriter () throw () {
  dest << std::flush;
}

void GameStateWriter::write (unsigned long int tav, const GameState& obj) throw () {
  dest << tav << '\t' 
       << obj.refstate << '\t'
       << obj.in_game << '\n';
}


GameStateReader::GameStateReader (std::istream& s) throw () : src(s), next(0) {
  if (src.good()) src >> next;
}

bool GameStateReader::next_timestamp (unsigned long int& t) const throw () {
  if (src.eof())
    return false;
  t=next;
  return true;
}

bool GameStateReader::read_until (unsigned long int& tav, GameState& obj, unsigned long int tuntil) throw () {
  tav=next;
  bool read_anything = false;
  while (!src.eof() && next<=tuntil) {
    tav=next;
    string line;
    deque<string> parts;
    getline (src, line);
    split_string (parts, line);
    src >> next;
    if (parts.size()>=2) {
      read_anything=true;
      int a;
      string2int (a, parts[0]);
      obj.refstate = RefereeState (a);
      string2bool (obj.in_game, parts[1]);
    }
  }
  return read_anything;
}
