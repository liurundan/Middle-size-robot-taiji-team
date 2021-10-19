#include "StdAfx.h"
#include "SimpleObstacleDetector.h"
#include "WorldModel.h"
#include "LineScanning.h"
#include "ColorClasses.h"
#include "WorldModel.h"
#include "Image.h"
#include "VisibleObject.h"
#include "Painter.h"

#include <stdlib.h>

using namespace std;

SimpleObstacleDetector::SimpleObstacleDetector(
	           const ConfigReader& config,
			   const CoordinateMapping* mapping, 
			   const CoordinateMapping* rel2img,
			   double minTransitionSize) 
    : ObstacleDetector(), mapping(mapping), rel2img(rel2img),
      minTransitionSize(minTransitionSize), vis(0)
{
    min_width = 50;
    clustering_thresh = 300;
    //config.get("SimpleObstacleDetector::clustering_thresh", clustering_thresh);
   // config.get("SimpleObstacleDetector::min_width", min_width);
} 

SimpleObstacleDetector::~SimpleObstacleDetector()
{}
  
void SimpleObstacleDetector::searchObstacles(
			const Image& image, 
		    const ScanResult* scanResult, Time time, 
		    VisibleObjectList* vol, bool writeWM ) throw (RobotsException)
{
    if (scanResult->id != COLOR_OBSTACLE) 
	{
		throw RobotsException(__FILE__ 
					   ": Expected results of color class "
					   "COLOR_OBSTACLE but received results of another "
					   "class.");
	}
    
    std::vector<Vec> opoints;
    Vec obstaclePosition;
    for (unsigned int i=0; i < scanResult->transitions.size(); i+=2) 
	{
      
      if (scanResult->transitions.size() <= i+1) 
	  { // a pair left?
		throw RobotsException(__FILE__
					   ": The transition list is not organized "
					   "pair-wise. One end transision is missng.");
      }
      
      const Transition& transStart = scanResult->transitions[i];
      const Transition& transEnd   = scanResult->transitions[i+1];
      
      if(  transStart.type != Transition::START ||
	       transEnd.type   != Transition::END ) 
	  {
		throw RobotsException(__FILE__
					   ": The transition list is not organized "
					   "pair-wise. Each start transtition has to be "
					   "followed by exactly one endtransision!");
      }
     
      if ((transStart.toPos - transEnd.fromPos).length() < minTransitionSize) 
	  {
	       continue;
      }

      obstaclePosition = mapping->map(transStart.toPos);
      
      opoints.push_back(obstaclePosition);
    }

    SimpleVecCluster clust(clustering_thresh);
    for (unsigned int h=0; h < opoints.size(); h++) 
	{
      clust.add(opoints[h]);

#if 0      
      if (vol) 
	  {
		vol->objectlist.push_back(VisibleObject (opoints[h], VisibleObject::obstacle) );
      }
      if (writeWM)
	  {
		WorldModel::get_main_world_model().set_visual_information( 
			VisibleObject (opoints[h], VisibleObject::obstacle), time );
      }
#endif
    }
    
    for( unsigned int j=0;j<clust.cluster.size(); j++ )
	{
		if (clust.cluster[j].width < min_width) 
			continue;
		if (vol) 
		{
		  vol->objectlist.push_back(
			  VisibleObject (clust.cluster[j].support, VisibleObject::obstacle, clust.cluster[j].width) );
		}
		if (writeWM) 
		{
		  WorldModel::get_main_world_model().set_visual_information( 
			  VisibleObject (clust.cluster[j].support, VisibleObject::obstacle, clust.cluster[j].width), time );
		}
	} 
    
    if (vis) 
	{
      paintResults(clust);
      vis = 0;
    }
}


void SimpleObstacleDetector::paintResults(SimpleVecCluster& cluster)
{
    Painter p(*vis);
    p.setColor(200, 200, 200);

    for (unsigned int cl=0; cl < cluster.cluster.size(); cl++) 
	{
      if (cluster.cluster[cl].width < min_width) continue;
      p.setPen(cl%2);
	  
      p.setColor(((cl/2)%2) * 50 + 150, 
		 ((cl/2)%2) * 50 + 150, 
		 ((cl/2)%2) * 50 + 150);
      for (unsigned int s=0; s < cluster.cluster[cl].samples.size(); s++) 
	  {
		Vec point = rel2img->map(cluster.cluster[cl].samples[s]);
		p.markCrosshair((int)point.x, (int)point.y, 3);
      }

      p.setPen(Painter::PEN_SOLID);
      Vec point = rel2img->map(cluster.cluster[cl].support);
      p.markRect((int)point.x, (int)point.y, 3);
      p.markRect((int)point.x, (int)point.y, 5);
    }   
}