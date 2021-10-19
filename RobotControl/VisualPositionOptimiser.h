#ifndef _VisualPositionOptimiser_h_
#define _VisualPositionOptimiser_h_

#include "VisibleObject.h"
#include "FieldLUT.h"

class VisualPositionOptimiser
{
public:
    VisualPositionOptimiser (const FieldLUT&, double, double) throw ();
   
    double calculate_distance_weights (const VisibleObjectList&, unsigned int =10000) throw ();
  
    double optimise (Vec&, Angle&, const VisibleObjectList&, unsigned int, unsigned int =10000) const throw ();
  
    double analyse (Vec&, double&, Vec, Angle, const VisibleObjectList&, unsigned int =10000) const throw ();

	double error(Vec pos, Angle ang, const VisibleObjectList& vis,unsigned int max_lines=100) const throw();
	
protected:
	void error (double&, double&, double&, double&, double, 
	double, double, const VisibleObjectList&, unsigned int) const throw ();

private:
	const FieldLUT& the_field_lut;  
    double c;                         
    double c2;                      
    double d2;                      
    std::vector<double> weights;   
};

#endif
