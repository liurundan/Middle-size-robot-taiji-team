#ifndef _CMDGENERATOR_H_
#define _CMDGENERATOR_H_

#include "DriveVector.h"
#include "RobotsException.h"
#include "Time.h"
#include <string>

class CmdGenerator
{
public:
    virtual ~CmdGenerator() throw () {};
    virtual bool checkCommitmentCondition(const Time&) throw()
    { return true; }
    virtual void getCmd(DriveVector *dv, const Time&) throw(RobotsException)=0;
    virtual void gainControl(const Time&) throw(RobotsException) {}
    virtual void looseControl(const Time&) throw(RobotsException) {}

    virtual const char* getName() const { return name.c_str(); }
protected:
    CmdGenerator(const char* name1="CmdGenerator") : name(name1) {} ///< standard constructor
    std::string name;
};

#endif

