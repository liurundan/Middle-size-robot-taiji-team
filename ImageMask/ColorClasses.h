#ifndef _colorclasses_h_
#define _colorclasses_h_

#include <vector>
#include <string>

#include "ConfigReader.h"
#include "Image.h"

enum
{ 
	 COLOR_IGNORE = 0,
	 COLOR_BALL,
	 COLOR_BLUE,
	 COLOR_YELLOW,
	 COLOR_OBSTACLE,      ///< black
	 COLOR_LINE,          ///< white
	 COLOR_FIELD,         ///< green
	 COLOR_CYAN,          
	 COLOR_MAGENTA        
};

struct ColorClassInfo
{
	int id;               
    std::string name;      
    RGBTuple color;        
    bool findTransitions; 
    bool findOnlyFirst;    
    bool findPoints;       
    
    ColorClassInfo(int id, std::string name, const RGBTuple& color,
		   bool findTransitions=true, bool findPoints=false, 
		   bool findOnlyFirst=false);
};

class ColorClassInfoList
{
public:
	 std::vector<ColorClassInfo*> classList;
 
    ColorClassInfoList();
    
    ColorClassInfoList(const ConfigReader& config);
    
    ~ColorClassInfoList();

    static const ColorClassInfoList* 
      getColorClassInfoList(const ConfigReader* config = 0);
protected:
	 static ColorClassInfoList* theColorClassInfoList;

private:
	  void std_init ();   

};

#endif

