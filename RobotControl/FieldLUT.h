#ifndef _FieldLUT_h_
#define _FieldLUT_h_

#include "FieldGeometry.h"
#include "Vec.h"

class FieldLUT
{
public:
	 
    FieldLUT (const FieldGeometry&, unsigned int) throw (std::bad_alloc);
    ~FieldLUT () throw ();
 
    double distance (const Vec&) const throw ();
    Vec gradient (const Vec&) const throw ();

private:
	unsigned int x_res;                               
    unsigned int y_res;                                
    unsigned int cell_size;                            

    double* array;                                     
    Vec* grad;                                

    double error_outside;                             

    void draw_line_segment (Vec, Vec);                 
    void draw_arc (Vec, double, Angle, Angle);         
    void draw_dot (Vec);                               
    void update (unsigned int, unsigned int, double);  
};
 
#endif

 
