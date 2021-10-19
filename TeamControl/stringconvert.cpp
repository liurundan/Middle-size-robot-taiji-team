#include "StdAfx.h"
#include "stringconvert.h"

#include <cctype>
#include <cstdio>

using namespace std;

void split_string (deque<string>& dest, const string& src) throw (std::bad_alloc)
{
  dest.clear();
  if (src.length()==0)
    return;
  int i=-1;
  //bool was_white = isspace (src[0]);
  bool was_white =  (isspace (src[0]) != 0 )? true:false;
  for (unsigned int j=0; j<src.length(); j++) 
  {
    if (isspace (src[j])) 
	{
      if (was_white)
	  {
		i=j;
      } 
	  else 
	  {
		was_white=true;
		dest.push_back (src.substr (i+1, j-i-1));
		i=j;
      }
    } 
	else
	{
      was_white=false;
    }
  }
  if (!was_white) 
  {
    dest.push_back (src.substr (i+1, src.length()));
  }
}

bool prefix (const std::string& p, const std::string w) throw ()
{
  const unsigned int n=p.length();
  if (n>w.length())
    return false;
  for (unsigned int i=0; i<n; i++)
    if (p[i]!=w[i])
      return false;
  return true;
}

bool string2double (double& dest, const string& src) throw () 
{
  char* end_char;
  dest = strtod (src.c_str(), &end_char);
  return ((*end_char)=='\0');
}

bool string2float (float& dest, const string& src) throw () 
{
  double d;
  bool success = string2double (d, src);
  dest=d;
  return success;
}

bool string2lint (long int& d, const string& s) throw () 
{
  char* end_char;
  d = strtol (s.c_str(), &end_char, 0);
  if ((*end_char)!='\0')
    return false;
  return true;
}

bool string2ulint (unsigned long int& d, const string& s) throw () 
{
  char* end_char;
  d = strtoul (s.c_str(), &end_char, 0);
  if ((*end_char)!='\0')
    return false;
  if ((s[0]=='-') && (d!=0))
      return false;
  return true;
}

bool string2int (int& d, const string& s) throw () 
{
  long int l;
  bool success = string2lint (l, s);
  d=l;
  return success;
}

bool string2uint (unsigned int& d, const string& s) throw () 
{
  unsigned long int l;
  bool success = string2ulint (l, s);
  d=l;
  return success;
}

bool string2bool (bool& d, const string& s) throw ()
{
  if (s==string("0")) 
  {
    d=false;
    return true;
  }
  if (s==string("1"))
  {
    d=true;
    return true;
  }
  if (s==string("true")) 
  {
    d=true;
    return true;
  }
  if (s==string("false")) 
  {
    d=false;
    return true;
  }
  return false;
}

