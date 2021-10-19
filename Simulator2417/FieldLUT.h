#ifndef _FieldLUT_
#define _FieldLUT_

#include "FieldGeometry.h"
#include "Geometry.h"
 
class FieldLUT
{
public:
	FieldLUT ( const FieldGeometry&, unsigned int ) throw( bad_alloc);
	~FieldLUT() throw();
	double distance(const Vector&) const throw();
	Vector gradient(const Vector&) const throw();
protected:
private:
	unsigned int x_res;                                 
    unsigned int y_res;                               
    unsigned int cell_size;                            

    double* array;                                    
    Vector* grad;                              

    double error_outside;                            

    void draw_line_segment (Vector, Vector);                 
    void draw_arc (Vector, double, MyAngle, MyAngle);         
    void draw_dot (Vector);                               
    void update (unsigned int, unsigned int, double);
};

#endif

 