#ifndef _linescanning_h_
#define _linescanning_h_

#include <vector>

#include "RobotsException.h"
#include "geometry.h"
#include "Vec.h"
#include "ScanLines.h"
#include "Image.h"
#include "ColorClasses.h"

struct Transition
{
	int type;        
    int from;         
    int to;            
    int twoStep;      
    Vec fromPos;      
    Vec toPos;        
    bool virt;         
 
    inline Transition(int type, int from, int to, int twoStep,
		      const Vec& fromPos, const Vec& toPos, bool virt=false);
    enum { START=0, END };
};

struct ScanResult
{
	int id;                     
    std::vector <Transition> transitions;  
    std::vector <Vec> points;    
     
    inline ScanResult(int id);  
   
    void clear(); 
};

class ScanResultList
{
public:
	std::vector<ScanResult*> results;   
    ScanResultList(int n);
 
    ~ScanResultList();
     
    void clear();
};

class LineScanner
{
public:
	 LineScanner(ScanLines* lines, 
		const ColorClassInfoList* colClasses,
		bool bScanUsbimage = false);
    ~LineScanner();
 
    void scan(const Image& image, ScanResultList* results )
      throw (RobotsException);
 
    void setVisualize(Image* vis) { this->vis = vis; }

protected:
	ScanLines* lines;    
    const ColorClassInfoList* colClasses; 
    Image* vis;
    bool bScanUsbimage;
    void visualize(const ScanResultList* results);
};

 // inline /////////////////////////////////////////////////////////

Transition::Transition(int type, int from, int to, int twoStep,
		 const Vec& fromPos, const Vec& toPos, bool virt)
		: type(type), from(from), to(to), twoStep(twoStep), fromPos(fromPos), 
		  toPos(toPos), virt(virt)
{}

ScanResult::ScanResult(int id) : id(id)
{}
  
#endif
