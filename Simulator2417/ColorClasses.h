#ifndef _colorclasses_h_
#define _colorclasses_h_

#include <vector>
#include <string>
using namespace std;
  
typedef struct 
{
	unsigned char r; ///< red value in [0,...,255]
	unsigned char g; ///< green value in [0,...,255]
	unsigned char b; ///< blue value in [0,...,255]
} RGBTuple;

typedef struct 
{
	unsigned char y; ///< luminance in [0,...,255]
	unsigned char u; ///< u in [0,...,255]
	unsigned char v; ///< v in [0,...,255]
} YUVTuple;

typedef struct 
{
	unsigned char u; ///< u in [0,...,255]
	unsigned char y; ///< luminance in [0,...,255]
	unsigned char v; ///< v in [0,...,255]
} UYVTuple;

typedef struct 
{
	unsigned char u; ///< u in [0,...,255]
	unsigned char y1;///< luminance of first pixel in [0,...,255]
	unsigned char v; ///< v in [0,...,255]
	unsigned char y2;///< luminance of second pixel in [0,...,255]
} UYVYTuple;  

enum 
{ 
	COLOR_IGNORE = 0,
	COLOR_BALL,
	COLOR_BLUE,
	COLOR_YELLOW,
	COLOR_OBSTACLE,      ///< black
	COLOR_LINE,          ///< white
	COLOR_FIELD,         ///< green
	COLOR_CYAN,          ///< unused
	COLOR_MAGENTA        ///< unused
};

struct ColorClassInfo 
{
	int      id;               
	string   name;      
	RGBTuple color;        
	bool     findTransitions;  
	bool     findOnlyFirst;   
	bool     findPoints;       

	ColorClassInfo(int id, string name, const RGBTuple& color,
		   bool findTransitions=true, bool findPoints=false, 
		   bool findOnlyFirst=false);
};

class ColorClassInfoList
{
	public:
	  vector<ColorClassInfo*> classList;
	  ColorClassInfoList();

	~ColorClassInfoList();

	static const ColorClassInfoList* 
	  getColorClassInfoList();

	protected:
	static ColorClassInfoList* theColorClassInfoList;

	private:
	void std_init ();  
};
 
#endif
