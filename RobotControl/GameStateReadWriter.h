#ifndef _GameStateReadWriter_h
#define _GameStateReadWriter_h

#include <iostream>
#include "GameState.h"

class GameStateWriter
{
public:
    GameStateWriter (std::ostream&) throw ();
    ~GameStateWriter () throw ();
    void write (unsigned long int, const GameState&) throw ();
protected:
private:
	std::ostream& dest;

};

class GameStateReader
{
public:
    GameStateReader (std::istream&) throw ();
    bool next_timestamp (unsigned long int&) const throw ();
    bool read_until (unsigned long int&, GameState&, unsigned long int) throw ();
protected:
private:
    std::istream& src;
    unsigned long int next;

};

#endif