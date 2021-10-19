#include "StdAfx.h"
#include "RobotMask.h"

#include <fstream>
#include <iostream>

using namespace std;

static void skip(istream& in);
static void error(const string& msg);

const RobotMask* RobotMask::instance = 0;

RobotMask::RobotMask(std::string filename)
  : mask(0), width(0), height(0)
{
  ifstream in(filename.c_str());
  if (!in) 
  {
 	CString sTmp;
	sTmp = filename.c_str();
	AfxMessageBox("Could not open mask file: " + sTmp);
    exit(1);
  }

  char c;
  int type, maxCol;

  skip(in);
  if (!in.get(c)) error("Unexpected end of file.");
  if (c != 'P' && c != 'p') error("Not a PPM file.");

  skip(in);
  in >> type;
  skip(in);
  in >> width;
  skip(in);
  in >> height;
  skip(in);
  in >> maxCol;    
  
  if (type != 6) 
  {  
    error("Not a binary ppm file.");
  }
  in.ignore(1);    
  unsigned char* buf = new unsigned char[width*height*3];
  in.read(reinterpret_cast<char*>(buf), width*height*3);
  mask = new bool[width*height];

  for (int x = 0; x < width; x++) 
  {
    for (int y = 0; y < height; y++) 
	{
      mask[x+y*width] = buf[x*3+y*width*3] > 127;  
	}
  }
  delete [] buf;
}

RobotMask::~RobotMask()
{
  if (mask) delete [] mask;
   
}

void RobotMask::load(std::string filename)
{
  if(!instance) 
  {
    instance  = new RobotMask(filename);
  }
  else 
  {
    error("Mask has already been loaded, please exit progam first and load again.");
  }
}

static void error(const string& msg) 
{
  CString sTmp;
  sTmp = msg.c_str();
  AfxMessageBox(sTmp);
  return;
}
  
static void skip(istream& in)
{
  char c;
  do
  {
    if (!in.get(c)) return;
  } 
  while(isspace(c) || c=='\n' || c=='\r');
  if (c=='#') 
  {                 
    do 
	{                         
      if (!in.get(c)) return;
    } while(c!='\n');
    skip(in);                    
  }
  else                           
    in.putback(c);                
}