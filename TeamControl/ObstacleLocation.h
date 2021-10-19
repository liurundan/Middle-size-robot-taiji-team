#ifndef _obstacle_location_h
#define _obstacle_location_h

#include "Vec.h"
#include <vector>
#include <iostream>

struct ObstacleLocation
{
    
    ObstacleLocation (unsigned int =0) throw (std::bad_alloc);
    
    ObstacleLocation (const ObstacleLocation&) throw (std::bad_alloc);
    
    void writeAt(std::ostream &stream) const;
 
    int  readFrom(std::istream &stream);

    std::vector<Vec> pos;             
    std::vector<double> width;        
 
};
 

#endif