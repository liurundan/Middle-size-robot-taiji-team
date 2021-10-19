#ifndef _WS_PSET_H_
#define _WS_PSET_H_

#include "dataClass.h"
#include "geometry.h"
#include <vector>
#include <string.h>

struct WSpset
{
public:
	static const int MAX_NUM;
	int    num;
	Object p[16];
	std::vector<Object> p_obstacle;
	std::vector<Object> p_need;

public:
	WSpset() { num = 0;	}
	
public:
	Object operator[](int idx) const {return p_obstacle[idx];	}
	
	void operator=(const WSpset& pset)
	{
		num = pset.num;
		if( num > 0 )
		{
			memcpy(p,pset.p,num*sizeof(Object));
		}
		for (int i=0; i < p_obstacle.size(); i++)
		{
			p_obstacle.push_back(pset.p_obstacle[i]);
		}
	}
	
	void operator+=(const WSpset & pset) 
	{
		append(pset);
	}
	
public: 
	void append(const WSpset & pset);
	bool append(Object);
    bool prepend(Object);
	void join(const WSpset & pset);
	bool join(Object);
	void meet(const WSpset & pset);
	void remove(const WSpset & pset);
	void remove(Object player);
    
	void remove_all();
	
	Object closest_player_to_point(const Vec pos)  ;

public:  
	void keep_and_sort_players_by_x_from_right(int how_many);
	void keep_and_sort_players_by_x_from_left(int how_many);
	void keep_and_sort_players_by_y_from_right(int how_many);
	void keep_and_sort_players_by_y_from_left(int how_many);
	
	void keep_and_sort_closest_players_to_point(int how_many, Vec pos);
 	void keep_players_with_max_age(int age);
	
	void keep_players_in(Area const & set);
	
	void keep_players_in_circle(Vec pos, double radius);
	
	/** if you cannot constrain the sides to be parallel to the x or y axis,
	then take a look at keep_players_in_quadrangle
	*/
	void keep_players_in_rectangle(Vec center, double size_x, double size_y);
	
	/** a rectangle can be also specified by two of his corners which lie 
	on opposite sides of one of his diagonals
	
	  p1                                        p2           p1
      +---------+                               +---------+  
      |         |     <--- OK                   |         |  <--- NOT OK
      |         |                               |         |  
      +---------+                               +---------+  
	  p2
	*/
	void keep_players_in_rectangle(Vec p1, Vec p2);
	
	/**
	if p1,p2,p3 are not colinear, then they define a triangle
	*/
	void keep_players_in_triangle(Vec p1, Vec p2, Vec p3);
	
	/**
	quadrangle is like a rectangle, but vertices are not required to be parallel to
	the x or the y axes!!!
	p1 and p3 must be connected by a diagonal of the quadrangle, or equivalently: 
	the points p1,p2,p3,p4 must follow the circumference of the rectangle
	
	  p2           p1                             p3           p1
      +---------+                                +---------+  
      |         |     <--- OK                    |         |  <--- NOT OK
      |         |                                |         |  
      +---------+                                +---------+  
	  p3           p4                             p2          p4
	  
	*/
	void keep_players_in_quadrangle(Vec p1, Vec p2, Vec p3, Vec p4);
	
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
      c                           d
	  
		the distance between  (a and c) is width
		the distance between  (b and d) is width
		
		  the vectors a-c and b-d are parallel and orthogonal to the Vec p2-p1
		  
			but p2-p1 doesn't need to be parallel to the x or the y axes
	*/
	void keep_players_in_quadrangle(Vec p1, Vec p2, double width);
	
	/**
	quadrangle is like a rectangle, but vertices are not required to be parallel to
	the x or the y axes!!!
	b
	--------+
	a         ---------        |
	+--------                 |
	|                         |
	|                         |
    p1 +                         + p2
	|                         |
	|                         |
	+--------                 |
	c         ---------        |
	--------+
	d
	
      the distance between  (a and c) is width
      the distance between  (b and d) is width2 (and doesn't need to be the same as width)
	  
		the Vecs a-c and b-d are parallel and orthogonal to the Vec p2-p1
		
		  but p2-p1 doesn't need to be parallel to the x or the y axes
	*/
	void keep_players_in_quadrangle(Vec p1, Vec p2, double width, double width2);
	
	/** a halfplane is specified be a point [pos] on it's boundary and by an angle. 
	all Vecs belonging to the halfplane are then:
	
      pos + k * vec; with vec.arg() between [ang] and [ang + PI]
	*/
	void keep_players_in_halfplane(Vec pos, Angle ang1);
	
	/** a halfplane can also be defined by a point [pos] on it's boundary and by
	the normal Vec [normal_vec] pointing towards the interior of the 
	half plane. |normal_vec| = 1 is NOT required!!!
	*/
	void keep_players_in_halfplane(Vec pos, Vec normal_vec);
	
	
	/** a cone is specified by his focal point [pos] and the two angles
	all Vecs belonging to the cone are then
	
      pos + k * vec; with vec.arg() between [ang1] and [ang2] (going counterclockwise from ang1 to ang2)
	*/
	void keep_players_in_cone(Vec pos, Angle ang1, Angle ang2);
	
	/** a cone can also be defined by his focal point [pos] and the two directions,
	so that all Vecs belonging to the cone have the form:
	
      pos + k * vec; with vec.arg() between [dir1.arg()] and [dir2.arg()] (going counterclockwise from ang1 to ang2)
      
		/ dir2
		/
		/
		/
		p /
		\
		\
		\
		\
		\ dir1
		
		  dir1.norm() and dir2.norm() are NOT relevant.
	*/
	void keep_players_in_cone(Vec pos, Vec dir1, Vec dir2);
	
	/** a cone can also be defined by his focal point [pos],its direction,
	and the width of its opening angle
	
	  /
	  /
	  /
	  /
	  p /_________  dir
	  \
	  \
	  \
	  \
	  \
 		  dir1.norm() is NOT relevant.
	*/
	void keep_players_in_cone(Vec pos, Vec dir, Angle ang);
	
protected: //kept protected until nobody needs it
	/** keeps players with pass_info.valid == true  */
	void keep_players_with_pass_info(); //see WSinfo::get_teammate_with_newest_pass_info()
protected: //some useful tools but not part of the interface
		   /**
		   increasing sorting, the smallest elements in front and just how_many of them will be kept
	*/
	void keep_and_sort(int how_many, std::vector<double> xxx);
	
	
};



#endif
