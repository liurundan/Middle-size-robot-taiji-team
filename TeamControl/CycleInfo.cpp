#include "StdAfx.h"
#include "CycleInfo.h"

CycleInfo::CycleInfo () {;}

CycleInfo::CycleInfo (const CycleInfo& ci) : rloc_vis(0), bloc_vis(0), rloc_exec(0), bloc_exec(0), ppos(0) 
{
  cycle_num = ci.cycle_num;
  time_msec = ci.time_msec;
  rloc_vis = ci.rloc_vis;
  bloc_vis = ci.bloc_vis;
  rloc_exec = ci.rloc_exec;
  bloc_exec = ci.bloc_exec;
  oloc = ci.oloc;
  vloc = ci.vloc;
  gs = ci.gs;
  logmsg = ci.logmsg;
  paintmsg = ci.paintmsg;
  ppos = ci.ppos;
}

const CycleInfo& CycleInfo::operator= (const CycleInfo& ci) 
{
  cycle_num = ci.cycle_num;
  time_msec = ci.time_msec;
  rloc_vis = ci.rloc_vis;
  bloc_vis = ci.bloc_vis;
  rloc_exec = ci.rloc_exec;
  bloc_exec = ci.bloc_exec;
  oloc = ci.oloc;
  vloc = ci.vloc;
  gs = ci.gs;
  logmsg = ci.logmsg;
  paintmsg = ci.paintmsg;
  ppos = ci.ppos;
  return (*this);
}