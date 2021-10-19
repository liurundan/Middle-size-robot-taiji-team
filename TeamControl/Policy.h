#ifndef _Policy_h
#define _Policy_h

#include "ConfigReader.h"
#include "Table.h"
#include "RobotsException.h"
#include "RemoteTeamState.h"
#include <string>
#include <stdexcept>

class Policy
{
public:
    Policy (const char* nm) throw (std::bad_alloc, RobotsException) : name(nm) {;}
    Policy (const Policy& src) throw (std::bad_alloc) : name (src.name) {;}
    virtual ~Policy () throw () {;}
    virtual Policy* clone () const throw (std::bad_alloc) =0;
    virtual void   update (RemoteTeamState&) throw () {;}
    virtual const std::string& get_name () const throw () { return name; }
    virtual const Table<std::string> get_roles () const throw () =0;

protected:
	    std::string name;
private:
};

class StaticPolicy : public Policy
{
public:
	StaticPolicy (  ConfigReader&, const char* nm) throw (std::bad_alloc, RobotsException);
    StaticPolicy (const StaticPolicy&) throw (std::bad_alloc);
    Policy* clone () const throw (std::bad_alloc);
    const Table<std::string> get_roles () const throw ();

protected:
	    Table<std::string> roles;

private:
};

class DynamicDefendOffendPolicy : public Policy
{
public:
	DynamicDefendOffendPolicy (  ConfigReader&, const char*) throw (std::bad_alloc, RobotsException);
    DynamicDefendOffendPolicy (const DynamicDefendOffendPolicy&) throw (std::bad_alloc);
    Policy* clone () const throw (std::bad_alloc);
    ~DynamicDefendOffendPolicy () throw ();
    void update (RemoteTeamState&) throw ();
    const Table<std::string> get_roles () const throw ();

protected:
	double	hyst_x1, hyst_x2;
    Policy* defend_policy;
    Policy* offend_policy;
    bool	was_defend;

private:
};

#endif

