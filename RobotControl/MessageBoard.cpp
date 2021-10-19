#include "StdAfx.h"
#include "MessageBoard.h"

#include "MessageBoard.h"
#include "stringconvert.h"
#include "WorldModel.h"  // for debug
#include <iostream>

#define DEBUG_INCOMING 0


MessageBoard::MessageBoard () throw (std::bad_alloc) : incoming (50), outgoing (50), null_string ("") 
{
  clear();
}

const std::vector<std::string>  MessageBoard::get_incoming () const throw () 
{
  return incoming;
}

const std::vector<std::string>  MessageBoard::get_outgoing () throw () 
{
  outgoing_stream << std::flush;
  std::string line;
  while (!outgoing_stream.eof()) 
  {
    std::getline (outgoing_stream, line);
    if (outgoing_stream.eof() && line.length()==0)
      break;
    outgoing.push_back (line);
  }
  outgoing_stream.clear();    
  return outgoing;
}

void  MessageBoard::clear () throw () 
{
  incoming.clear();
  outgoing.clear();
}

void  MessageBoard::publish (const std::string& s) throw () 
{
  outgoing.push_back (s);
}

void  MessageBoard::publish (const char* s) throw () 
{
  outgoing.push_back (std::string(s));
}

std::ostream&  MessageBoard::publish_stream () throw () 
{
  return outgoing_stream;
}

void  MessageBoard::receive (const std::string& s) throw () 
{
  incoming.push_back (s);
#if DEBUG_INCOMING
  LOUT << "MBoard::receive: " << s << '\n';
#endif

}

void  MessageBoard::receive (const char* s) throw () 
{

  incoming.push_back (std::string(s));
#if DEBUG_INCOMING
  LOUT << "MBoard::receive: " << s << '\n';
#endif

}

void  MessageBoard::receive (const std::vector<std::string>& s) throw ()
{
  incoming.insert (incoming.end(), s.begin(), s.end());
#if DEBUG_INCOMING
  for (unsigned int i=0; i<s.size(); i++) 
  {
    LOUT << "MBoard::receive: " << s[i] << '\n';
  }
#endif
}

const std::string&  MessageBoard::scan_for_prefix (const std::string& s) const throw () 
{
  std::vector<std::string>::const_iterator it = incoming.begin();
  std::vector<std::string>::const_iterator itend = incoming.end();
  while (it<itend) 
  {
    if ( prefix (s, *it))
      return (*it);
    else
      it++;
  }
  return null_string;
}

const std::string&  MessageBoard::scan_for_prefix (const char* s) const throw ()
{
  std::string s1 (s);
  return scan_for_prefix (s1);
}
