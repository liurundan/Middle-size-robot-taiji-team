#include "StdAfx.h"
#include "ImageProcessingDummy.h"

using namespace std;

void ImageProcessingDummy::process_image () throw () 
{
  VisibleObjectList vol;
  WorldModel::get_main_world_model().set_visual_information (vol);
}