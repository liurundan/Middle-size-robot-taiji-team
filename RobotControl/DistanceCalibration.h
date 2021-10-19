#ifndef _DistanceCalibration_h
#define _DistanceCalibration_h

#include <string>
#include <deque>
#include "ColorClasses.h"
#include "LineScanning.h"
#include "Vec.h"

class DistanceCalibration
{
public:
	 DistanceCalibration ( Vec);
    ~DistanceCalibration ();
    void search_markers (const  ScanResultList&,  Angle);
    void writeMarkerFile (const std::string&);
 
private:
	 
	 struct MarkerInfo 
	 {
		Angle		 angle;
		unsigned int true_distance;
		double		 image_distance;
		MarkerInfo () throw () {;}
		MarkerInfo (const MarkerInfo& m) : angle(m.angle), true_distance(m.true_distance), image_distance(m.image_distance) {;}
		const MarkerInfo& operator= (const MarkerInfo& m) 
		{ angle=m.angle; true_distance=m.true_distance; image_distance=m.image_distance; return *this; }
    };
	double search_min_transition_index (const std::deque<MarkerInfo>&, double);

    std::deque<MarkerInfo> markers;
    Vec image_center;

};

#endif