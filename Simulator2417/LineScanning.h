#ifndef _linescanning_h_
#define _linescanning_h_
  
#include "ScanLines.h"
#include "FieldGeometry.h"
#include "Geometry.h"
#include "ColorClasses.h"

struct Transition 
{
	int type;         ///< START: start of the region, END: end of the region
	int from;         ///< class of the first pixel's color
	int to;           ///< class of the second pixel's color
	int twoStep;      ///< START: class of pixel two steps back, END: class of pixel two Steps ahead
	Vector fromPos;      ///< x-y-coordinates of the first pixel (image coords)
	Vector toPos;        ///< x-y-coordinates of the second pixel (image coords)
	bool virt;        ///< whether or not this transition is a virtual one
 
	inline Transition(int type, int from, int to, int twoStep,
			  const Vector& fromPos, const Vector& toPos, bool virt=false);
	enum { START=0, END };
};

struct ScanResult 
{
	int id;                     
	vector <Transition> transitions;  
	vector <Vector>     points;    
 
	inline ScanResult(int id);  
 	void   clear(); 
};

class ScanResultList 
{
public:
    vector<ScanResult*> results;   
    ScanResultList( int n );
    ~ScanResultList();
    void clear();
 
};
 
class LineScanner 
{
public:
    LineScanner( ScanLines* lines,
		         const ColorClassInfoList* colClasses,
		         FieldGeometry* Myfieldlines);
    ~LineScanner();
    void scan( ScanResultList* results, Frame2d FR  );
    
protected:
    ScanLines* lines;
    const ColorClassInfoList* colClasses;

    FieldGeometry* fieldlines; 
    void  visualize(const ScanResultList* results);
};
 
// inline /////////////////////////////////////////////////////////

Transition::Transition(int type, int from, int to, int twoStep,
		 const Vector& fromPos, const Vector& toPos, bool virt)
: type(type), from(from), to(to), twoStep(twoStep), fromPos(fromPos), 
  toPos(toPos), virt(virt)
{}

ScanResult::ScanResult(int id)
: id(id)
{}
  
#endif
