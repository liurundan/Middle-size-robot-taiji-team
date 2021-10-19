#ifndef  _VisualPositionOptimiser_h_
#define  _VisualPositionOptimiser_h_

#include "VisibleObject.h"
#include "FieldLUT.h"

class VisualPositionOptimiser
{
public:
	double error( Vector pos, MyAngle ang, const VisibleObjectList& vis,
		          unsigned int max_lines);
    VisualPositionOptimiser (const FieldLUT&, double, double) throw ();
 
    double calculate_distance_weights (const VisibleObjectList&, unsigned int =10000) throw ();
 
    double optimise (Vector&, MyAngle&, const VisibleObjectList&, unsigned int, unsigned int =10000) const throw ();
  
    double analyse (Vector&, double&, Vector, MyAngle, const VisibleObjectList&, unsigned int =10000) const throw ();
protected:
	 void error( double&, double&, double&, double&, 
		         double, double, double, const VisibleObjectList&, 
				 unsigned int) const throw ();

private:
	const FieldLUT& the_field_lut;   
    double c;                         
    double c2;                     
    double d2;                    
    vector<double> weights;   
};

#endif