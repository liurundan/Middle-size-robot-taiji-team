#include "StdAfx.h"
#include "FieldMapper.h"
#include "LineScanning.h"
#include "ColorClasses.h"
#include "WorldModel.h"
#include "Image.h"

#include <cstdlib>
#include <cmath>

using namespace std;

FieldMapper::FieldMapper(int _x1,int _x2,int _y1,int _y2,int _w,int _h)
{
	x1=_x1;
	x2=_x2;
	y1=_y1;
	y2=_y2;
	w=_w;
	h=_h;
}

void FieldMapper::buildFieldMap(const Image& image)
{
 	int xaccess;
	int yaccess;
	xstep=(x2-x1)/w;
	ystep=(y2-y1)/h;
 
    // cout <<"xstep "<<xstep<<" ystep "<<ystep<<endl;
	for (int ya=0;ya<h;ya++)
	{
	  yaccess=y1+ystep*ya;
	  for (int xa=0;xa<w;xa++)
	  {
		xaccess=x1+xstep*xa;
         //cout<<"Acessing "<<xaccess<<" "<<yaccess<<endl;
		if (image.getPixelClass(xaccess,yaccess)==COLOR_FIELD)
		  imagemap[xa][ya]=1;
		else
		  imagemap[xa][ya]=0;

	  }
	}
 
	growRegions();
}
 
bool FieldMapper::insideField(int x,int y) const
{
	int xa;
	int ya;
	xa=x/xstep;
	ya=y/ystep;
	// cout <<" access"<< xa <<" "<<ya<<endl;
	return imagemap[xa][ya];
}


void FieldMapper::growRegions()
{
	//    cout <<" access"<< endl;
	//  Time vorher;
 	int count=0;
	int lasthorizontal=0;

	for (int y=0;y<h;y++)
	{
	  for (int x=0;x<w;x++)
	  {

		if (imagemap[x][y]==true)
		{
		  if (count < 3)
		  {
			for (int z=lasthorizontal;z<x;z++)
			{
				imagemap[z][y]=true;
			}
		  }

		  lasthorizontal=x;
		  count=0;

		}
		else
		{
		  count++;
		}
	  }
	} 
 
	count=0;
	int lastvertical=0;

	for (int x=0;x<w;x++)
	{
	  for (int y=0;y<h;y++)
	  {

		if (imagemap[x][y]==true)
		{
		  if (count < 3)
		  {
			for (int z=lastvertical;z<y;z++)
			{
				imagemap[x][z]=true;
			}
		  }

		  lastvertical=y;
		  count=0;

		}
		else
		{
		  count++;

		}

	  }
}


#if 0  
cout<<"**************************   FieldMap  *************************"<<endl;
for (int j=0;j<h;j++){
for (int i=0;i<w;i++){
  	if(imagemap[i][j]) cout<<"0";
	else cout << ".";
}
cout<<endl;  
}
#endif

}