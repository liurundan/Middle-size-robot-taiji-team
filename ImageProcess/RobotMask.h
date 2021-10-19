#ifndef _robotmask_h_
#define _robotmask_h_

#include <string>
#include <iostream>

class  RobotMask
{
public:
  ~RobotMask();

  static void load(std::string filename);

   
  inline static const RobotMask* getMask();

  inline bool isValid(int x, int y) const;

protected:
  RobotMask(std::string filename);
  static const RobotMask* instance;

  bool* mask;
  int width;
  int height;
};

// inlines ////////////////////////////////////////////////////////////////////

bool RobotMask::isValid(int x, int y) const
{
  if (x>=0 && y>=0 && x<width && y < height) 
  {  
    return mask[x+y * width];
  }
  else 
  {
    return false;
  }
}

const RobotMask* RobotMask::getMask()
{
  return instance; 
}
 
#endif