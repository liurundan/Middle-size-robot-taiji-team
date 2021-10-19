#ifndef _VisualContainer_h_
#define _VisualContainer_h_

#include "VisibleObject.h"
#include <vector>

class VisualContainer
{
public:
    VisualContainer () throw (std::bad_alloc);
    ~VisualContainer () throw ();

    void add (const VisibleObjectList&) throw (std::bad_alloc);
    void add (const VisibleObject&, Time) throw (std::bad_alloc);

    void clear () throw ();

    const VisibleObjectList& get_lines () const throw ();
    const VisibleObjectList& get_goals () const throw ();
    const VisibleObjectList& get_balls () const throw ();
    const VisibleObjectList& get_obstacles () const throw ();
    const VisibleObjectList& get_all () const throw ();

    void shuffle_lines () throw (std::bad_alloc);
    void prune_lines (unsigned int) throw ();
protected:
private:
	VisibleObjectList lines;        
    VisibleObjectList goals;       
    VisibleObjectList balls;       
    VisibleObjectList obstacles;    
    VisibleObjectList all;       
};


#endif
