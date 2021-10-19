#include "StdAfx.h"
#include "WorldModel.h"
#include "WSpset.h"

#define USE_GEOMETRY2D
const int WSpset::MAX_NUM = 16;

void WSpset::append(const WSpset & pset) 
{
	int size= pset.num;
	if (size + num > MAX_NUM) 
	{
		size= MAX_NUM- size -num;
	}
	memcpy(p+num,pset.p, size * sizeof( Object ));
	num+=size;
}

bool WSpset::append(Object player) 
{
	if (num >= MAX_NUM) 
	{
		return false;
	}
	p[num]= player;
	num++;
	return true;
}

bool WSpset::prepend(Object player) 
{
	if (num >= MAX_NUM) 
	{
		return false;
	}
	for (int i= num; i > 0; i--)
		p[i]= p[i-1];
	p[0]= player;
	num++;
	return true;
}

void WSpset::join(const WSpset & pset) 
{
	for (int i=0; i< pset.num; i++) 
	{
		bool include= true;
		for (int k=0; k < num; k++) 
			if ( p[k] == pset.p[i] ) 
			{
				include= false;
				break;
			}
			if (include) 
			{
				bool tmp= append(pset.p[i]);
				if (!tmp)
					return;
			}
	}
}

bool WSpset::join(Object player) 
{
	bool include= true;
	for (int k=0; k < num; k++) 
		if ( p[k] == player ) 
		{
			include= false;
			break;
		}
		if (include) 
			return append(player);
		return true;
}

void WSpset::meet(const WSpset & pset) 
{
	int current_spare_idx= 0;
	for (int i=0; i< num; i++) 
	{
		bool keep= false;
		for (int k=0; k < pset.num; k++) 
			if ( p[i] == pset.p[k] ) 
			{
				keep= true;
				break;
			}
			if (keep) 
			{
				p[current_spare_idx]= p[i];
				current_spare_idx++;
			}
	}
	num= current_spare_idx;
}

void WSpset::remove(const WSpset & pset) 
{
	int current_spare_idx= 0;
	for (int i=0; i< num; i++) 
	{
		bool keep= true;
		for (int k=0; k < pset.num; k++) 
			if ( p[i] == pset.p[k] ) 
			{
				keep= false;
				break;
			}
			if (keep) 
			{
				p[current_spare_idx]= p[i];
				current_spare_idx++;
			}
	}
	num= current_spare_idx;
}

void WSpset::remove(Object player) 
{
	int current_spare_idx= 0;
	for (int i=0; i< num; i++) 
		if ( p[i] != player ) 
		{
			p[current_spare_idx]= p[i];
			current_spare_idx++;
		}
		
		num= current_spare_idx;
}

Object WSpset::closest_player_to_point(const Vec pos) 
{
  /*if (num <= 0)
  {
	  Object o;
	  o.onGreen = false;
	  return o;
  }
 
  Object result = p[0];
  double min_sqr_dist= result.absPos.sqr_distance(pos);

  for (int i=1; i<num; i++) 
  {
    double tmp= p[i].absPos.sqr_distance(pos);
    if (tmp < min_sqr_dist) 
	{
      min_sqr_dist= tmp;
      result= p[i];
    }
  }*/
  if (p_obstacle.size() <= 0)
  {
	  Object o;
	  o.onGreen = false;
	  return o;
  }
 
  Object result = p_obstacle[0];
  double min_sqr_dist= result.absPos.sqr_distance(pos);

  for (int i=1; i<p_obstacle.size(); i++) 
  {
    double tmp = p_obstacle[i].absPos.sqr_distance(pos);
    if (tmp < min_sqr_dist) 
	{
      min_sqr_dist= tmp;
      result = p_obstacle[i];
    }
  }

  return result;
}

void WSpset::keep_and_sort_players_by_x_from_right(int how_many) 
{
  //double xxx[MAX_NUM];
  std::vector<double> xxx;
  for (int i=0; i< p_obstacle.size(); i++)
    xxx.push_back(- p_obstacle[i].absPos.x) ;

  keep_and_sort(how_many, xxx);
  xxx.clear();
}

void WSpset::keep_and_sort_players_by_x_from_left(int how_many) 
{
  //double xxx[MAX_NUM];
  std::vector<double> xxx;
  for (int i=0; i< p_obstacle.size(); i++)
    xxx.push_back(p_obstacle[i].absPos.x);

  keep_and_sort(how_many, xxx);
  xxx.clear();
}

void WSpset::keep_and_sort_players_by_y_from_right(int how_many)
{
 /* double xxx[MAX_NUM];
  for (int i = 0; i < MAX_NUM; i++) 
    xxx[i] = - p[i].absPos.y;
   keep_and_sort( how_many, xxx);*/

     //double xxx[MAX_NUM];
  std::vector<double> xxx;
  for (int i=0; i< p_obstacle.size(); i++)
    xxx.push_back(- p_obstacle[i].absPos.y);

  keep_and_sort(how_many, xxx);
  xxx.clear();

}

void WSpset::keep_and_sort_players_by_y_from_left(int how_many)
{
  /*double xxx[MAX_NUM];
  for (int i = 0; i < MAX_NUM; i++) 
    xxx[i] = p[i].absPos.y;
   keep_and_sort( how_many, xxx);
*/
  std::vector<double> xxx;
  for (int i=0; i< p_obstacle.size(); i++)
    xxx.push_back( p_obstacle[i].absPos.y);

  keep_and_sort(how_many, xxx);
  xxx.clear();

}

void WSpset::keep_and_sort_closest_players_to_point(int how_many, Vec pos) 
{
 /* double sqr_dist[MAX_NUM];
  for (int i=0; i< MAX_NUM; i++)
    sqr_dist[i]= (p[i].absPos-pos).squared_length();

  keep_and_sort(how_many, sqr_dist);*/

  std::vector<double> sqr_dist;
  for (int i=0; i< p_obstacle.size(); i++)
    sqr_dist.push_back( (p_obstacle[i].absPos-pos).squared_length());

  keep_and_sort(how_many, sqr_dist);
  sqr_dist.clear();

}


void WSpset::keep_players_in( Area const & set ) 
{
 /* int current_spare_idx= 0;
  for (int i=0; i<num; i++) 
    if ( set.is_inside(p[i].absPos) ) 
	{
      p[current_spare_idx]= p[i];
      current_spare_idx++;
    }
  num= current_spare_idx;*/

  int current_spare_idx= 0;
  for( unsigned int i = 0; i < p_obstacle.size(); i++ ) 
  {
    if ( set.is_inside(p_obstacle[i].absPos)  )
	{
		current_spare_idx++;
		p_need.push_back(p_obstacle[i]);
		
	}
  }
  num = current_spare_idx; 
}


void WSpset::keep_players_in_circle(Vec pos, double radius) 
{
	/* double sqr_radius= radius*radius;
	int current_spare_idx= 0;
	for (int i=0; i<num; i++) 
	if ( (p[i].absPos.sqr_distance(pos) <= sqr_radius ) )
	{
	p[current_spare_idx]= p[i];
	current_spare_idx++;
	}
	num= current_spare_idx;*/
	double sqr_radius = radius*radius;
	int current_spare_idx = 0;
	for( unsigned int i = 0; i<p_obstacle.size(); i++) 
	{
		if ( (p_obstacle[i].absPos.sqr_distance(pos) <= sqr_radius ) )
		{
			current_spare_idx++;
			p_need.push_back(p_obstacle[i]);
		}
	}
	num = current_spare_idx; 
}

void WSpset::keep_players_in_rectangle(Vec center, double size_x, double size_y) 
{
#ifdef USE_GEOMETRY2D
  keep_players_in(XYRectangle(center,size_x,size_y));
#else
  double max_x= center.x + 0.5*size_x;
  double min_x= center.x - 0.5*size_x;
  double max_y= center.y + 0.5*size_y;
  double min_y= center.y - 0.5*size_y;
/*  int current_spare_idx= 0;
  for (int i=0; i<num; i++) {
    Vec pos= p[i].absPos;
    if ( pos.x <= max_x && pos.x >= min_x && pos.y <= max_y && pos.y >= min_y) 
	{
      p[current_spare_idx]= p[i];
      current_spare_idx++;
    }
  }
  num = current_spare_idx;*/

  int current_spare_idx= 0;
  for( unsigned int i = 0; i < p_obstacle.size(); i++ ) 
  {
	  Vec pos= p_obstacle[i].absPos;
	  if ( pos.x <= max_x && pos.x >= min_x && pos.y <= max_y && pos.y >= min_y) 
	  {
		  current_spare_idx++;
		  p_need.push_back(p_obstacle[i]);
	  }
	  
  }
  num = current_spare_idx; 

#endif
}

void WSpset::keep_players_in_rectangle(Vec p1, Vec p2) 
{
#ifdef USE_GEOMETRY2D
  keep_players_in(XYRectangle(p1,p2));
#else
  Vec tmp= 0.5*p1 + 0.5*p2;
  keep_players_in_rectangle(tmp, fabs(p1.x - p2.x), fabs(p1.y - p2.y));
#endif
}

void WSpset::keep_players_in_triangle(Vec p1, Vec p2, Vec p3) 
{
#ifdef USE_GEOMETRY2D
  keep_players_in(Triangle(p1,p2,p3));
#else
 /* int current_spare_idx= 0;
  for (int i=0; i<num; i++) 
    if ( Tools::point_in_triangle(p[i].absPos, p1,p2,p3) ) 
	{
      p[current_spare_idx]= p[i];
      current_spare_idx++;
    }
  num= current_spare_idx;*/

  int current_spare_idx= 0;
  for( unsigned int i = 0; i < p_obstacle.size(); i++ ) 
  {
	  if ( Tools::point_in_triangle(p_obstacle[i].absPos, p1,p2,p3)) 
	  {
		  current_spare_idx++;
		  p_need.push_back(p_obstacle[i]);
	  }
	  
  }
  num = current_spare_idx; 

#endif
}

/**
   p1 and p3 must be connected by a diagonal of the quadrangle, or equivalently: 
   the points p1,p2,p3,p4 must follow the circumference of the rectangle

    p2           p1                           p3           p1
      +---------+        		        +---------+  
      |         |     <--- OK		        |         |  <--- NOT OK
      |         | 			        |         |  
      +---------+			        +---------+  
    p3           p4			      p2           p4
       
*/
void WSpset::keep_players_in_quadrangle(Vec p1, Vec p2, Vec p3, Vec p4) 
{
#ifdef USE_GEOMETRY2D
  keep_players_in(Quadrangle(p1,p2,p3,p4));
#else
 /* int current_spare_idx= 0;
  for (int i=0; i<num; i++) 
    if ( Tools::point_in_triangle(p[i].absPos, p1,p2,p3) ||
	 Tools::point_in_triangle(p[i].absPos, p1,p3,p4) ) 
	{ 
      p[current_spare_idx]= p[i];
      current_spare_idx++;
    }
  num= current_spare_idx;*/
  int current_spare_idx = 0;
  for( unsigned int i = 0; i < p_obstacle.size(); i++ ) 
  {
	  if ( Tools::point_in_triangle(p_obstacle[i].absPos, p1,p2,p3)  ||
		   Tools::point_in_triangle(p_obstacle[i].absPos, p1,p3,p4) )
	  {
		  current_spare_idx++;
		  p_need.push_back(p_obstacle[i]);
	  }
	  
  }
  num = current_spare_idx; 

#endif
}

/**
   quadrangle is like a rectangle, but vertices are not required to be parallel to
   the x or the y axes!!!

   a                           b
   +-------------------------+          	
   |                         |
   |                         |
p1 +                         + p2
   |                         |
   |                         |
   +-------------------------+ 
  c	                      d
 
   the distance between  (a and c) is width
   the distance between  (b and d) is width

   the Vecs a-c and b-d are parallel and orthogonal to the Vec p2-p1

   but p2-p1 doesn't need to be parallel to the x or the y axes
*/
void WSpset::keep_players_in_quadrangle(Vec p1, Vec p2, double width) 
{
#ifdef USE_GEOMETRY2D
  keep_players_in(Quadrangle(p1,p2,width));
#else
  Vec tmp= p2-p1;
  Vec norm;
  norm.x= -tmp.y;
  norm.y= tmp.x;
  norm.normalize(0.5*width);
  Vec g1= p1+ norm;
  Vec g2= p1- norm;
  Vec g3= p2- norm;
  Vec g4= p2+ norm;

  //the order of the g* points is important!!!
  keep_players_in_quadrangle(g1,g2,g3,g4);
#endif
}

void WSpset::keep_players_in_quadrangle(Vec p1, Vec p2, double width, double width2) 
{
#ifdef USE_GEOMETRY2D
  keep_players_in(Quadrangle(p1,p2,width,width2));
#else
  Vec norm2= p2-p1;
  Vec norm;
  norm.x= -norm2.y;
  norm.y= norm2.x;
  norm2 = norm;
  norm.normalize(0.5*width);
  norm2.normalize(0.5*width2);

  Vec g1= p1+ norm;
  Vec g2= p1- norm;
  Vec g3= p2- norm2;
  Vec g4= p2+ norm2;

  //the order of the g* points is important!!!
  keep_players_in_quadrangle(g1,g2,g3,g4);
#endif
}

void WSpset::keep_players_in_halfplane(Vec pos, Vec normal_vec) {
#ifdef USE_GEOMETRY2D
  keep_players_in(Halfplane(pos,normal_vec));
#else
  // A * x + B * y - C >= 0   is the representation of the halfplane
  double A= normal_vec.x;
  double B= normal_vec.y;
  double C= A*pos.x + B*pos.y;
/*
  int current_spare_idx= 0;
  for (int i=0; i<num; i++) 
    if (  A * p[i].absPos.x + B * p[i].absPos.y >= C ) {
      p[current_spare_idx]= p[i];
      current_spare_idx++;
    }
  num= current_spare_idx;*/
 
  int current_spare_idx = 0;
  for( unsigned int i = 0; i < p_obstacle.size(); i++ ) 
  {
	  if (  A * p_obstacle[i].absPos.x + B * p_obstacle[i].absPos.y >= C )
	  {
		  current_spare_idx++;
		  p_need.push_back(p_obstacle[i]);
	  }
	  
  }
  num = current_spare_idx; 

#endif
}
void WSpset::keep_players_in_halfplane(Vec pos, Angle ang1) 
{
#ifdef USE_GEOMETRY2D
  keep_players_in(Halfplane(pos,ang1));
#else
  //we will implement this without the usage of the quite costly Vec::arg() 
  //method for the players
  Vec tmp;
  ang1+= 0.5*M_PI;
  tmp = Vec::getVecPositionFromPolar(1.0, ang1 ); 
  keep_players_in_halfplane(pos,tmp);
#endif
}

void WSpset::keep_players_in_cone(Vec pos, Angle ang1, Angle ang2) 
{
#ifdef USE_GEOMETRY2D
  keep_players_in(Cone2d(pos,ang1,ang2));
#else
  keep_players_in_halfplane(pos,ang1);
  keep_players_in_halfplane(pos, ang2+ M_PI);
#endif
}

void WSpset::keep_players_in_cone(Vec pos, Vec dir1, Vec dir2) 
{
#ifdef USE_GEOMETRY2D
  keep_players_in(Cone2d(pos,dir1,dir2));
#else
  Vec tmp;
  tmp.x= -dir1.y;
  tmp.y= dir1.x;
  keep_players_in_halfplane(pos,tmp);
  tmp.x= dir2.y;
  tmp.y= -dir2.x;
  keep_players_in_halfplane(pos,tmp);
#endif
}

void WSpset::keep_players_in_cone(Vec pos, Vec dir, Angle ang) 
{
#ifdef USE_GEOMETRY2D
  keep_players_in(Cone2d(pos,dir,ang));
#else
  double a= 0.5*ang.get_rad(); //here a half of the angle is taken
  double c= cos( a );
  double s= sin( a );

  /* Rotation Matrix (counterclockwise)    Rotation Matrix (clockwise) 
     [ cos   -sin]                         [  cos    sin]                         
     [ sin    cos]                         [ -sin    cos]                         
  */                                                                           

  Vec dir1= Vec( c*dir.x + s*dir.y, c*dir.y - s*dir.x); //rotate clockwise  
  Vec dir2= Vec( c*dir.x - s*dir.y, s*dir.x + c*dir.y); //rotate counterclockwise

  keep_players_in_cone(pos,dir1,dir2);
#endif
}
  
/******************************************************************************/
/* protected methods of WSpset                                                */
/******************************************************************************/
void WSpset::keep_and_sort(int how_many, std::vector<double> measured_data) 
{
	/*
  if (how_many > num)
    how_many= num;
  
  for (int i=0; i< how_many; i++) 
  {
    int min_idx= i;
    for (int j=i+1; j< num; j++) 
      if ( measured_data[j] < measured_data[min_idx] ) 
        min_idx= j;

    if (min_idx != i) 
	{ //toggle the elements at i and min_idx
      double int_tmp= measured_data[i];
      measured_data[i]= measured_data[min_idx];
      measured_data[min_idx]= int_tmp;

      Object obj_tmp= p[i];
      p[i]= p[min_idx];
      p[min_idx]= obj_tmp;
    }
  }
  num= how_many;*/
  if( how_many > p_obstacle.size())
    how_many= p_obstacle.size();
  
  for (int i=0; i< how_many; i++) 
  {
    int min_idx = i;
    for (int j=i+1; j< p_obstacle.size(); j++) 
      if ( measured_data[j] < measured_data[min_idx] ) 
        min_idx= j;

    if (min_idx != i) 
	{ //toggle the elements at i and min_idx
      double int_tmp= measured_data[i];
      measured_data[i]= measured_data[min_idx];
      measured_data[min_idx]= int_tmp;

      Object obj_tmp= p_obstacle[i];
	  p_obstacle[i] = p_obstacle[min_idx];
      p_obstacle[min_idx] = obj_tmp;
    }
  }
  num= how_many; 
}

void WSpset::remove_all()
{
	p_obstacle.clear();
	p_need.clear();
}


 
