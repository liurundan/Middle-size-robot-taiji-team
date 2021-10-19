#include "StdAfx.h"
#include "ControlLoop.h"

#include "Journal.h"
#include "GameState.h"
 
#include <sstream>
#include <cmath>
#include <cfloat>
//#include <unistd.h>

using namespace std;

namespace
{
  
  double potenzen10 [] = { 1,10,100,1000,10000,100000 };
  string to_string (double value, unsigned int precision =0) 
  {   
    stringstream inout;
    string sss;
    double faktor = ( precision<=5 ? potenzen10[precision] : 100000 );
    inout << floor(value*faktor+0.5)/faktor << '\n';
    inout >> sss;
    return sss;
  }

  inline bool is_inf (const double& x) throw () { return x>DBL_MAX; }
  inline bool is_nan (const double& x) throw () { return x!=x; }
  inline bool is_mininf (const double& x) throw () { return x<-DBL_MAX; }
  inline bool is_number (const double& x) throw () { return (!is_inf(x)) && (!is_mininf(x)) && (!is_nan(x)); }
    
}
 
ControlLoop::ControlLoop (  ConfigReader& conf, unsigned int ltime, CDC& pDC ) throw (RobotsException, bad_alloc):
 configuration_list (conf),num_cycles (0),loop_time(ltime)
{
  the_world_model = new WorldModel (configuration_list);
  the_robot = new Robot (configuration_list);
  the_player = new Player (configuration_list);
  the_image_processing = new ImageProcessing (configuration_list);
  the_user_interface   = new UserInterface(conf,*the_player,*the_image_processing,pDC);
  
  JMESSAGE("startup: all components alive");
   
  double dd[3];
  if (configuration_list.get ("initial_position_x", dd[0] ) > 0 &&
	  configuration_list.get ("initial_position_y", dd[1] ) > 0 )
	    
  {
	 if( configuration_list.get ("initial_position_h", dd[2]) > 0 )
	 {
        the_world_model->reset ( Vec (dd[0], dd[1]), Angle::deg_angle(dd[2]) );
	 }
     else
      the_world_model->reset (Vec (dd[0], dd[1]) );
  }
  else
     the_world_model->reset (Vec (6500, -4500),Angle::deg_angle(0)); // -90
  
  report_computation_time=report_computation_time_detailed=report_computation_time_gnuplot=false;
  configuration_list.get ("report_computation_time", report_computation_time);
  configuration_list.get ("report_computation_time_detailed", report_computation_time_detailed);
  configuration_list.get ("report_computation_time_gnuplot", report_computation_time_gnuplot);
  usec_image_processing=usec_world_model=usec_player=usec_robot=usec_user_interface=usec_idle=0;

  first_cycle_time.update();//更新时间
  time_reporter.update();
  drive_vector_delay = the_world_model->get_robot_properties().drive_vector_delay;

}

ControlLoop::~ControlLoop () throw () 
{
  the_robot->emergency_stop();
  try
  {
    if (report_computation_time) 
	{
		JMESSAGE((string("average loop time was ")+
			to_string(static_cast<double>(first_cycle_time.elapsed_msec())/static_cast<double>(num_cycles),2)+
			string(" ms")).c_str());
		JMESSAGE((string("average image processing time was ")+
			to_string(static_cast<double>(usec_image_processing)/static_cast<double>(1000*num_cycles),2)+
			string(" ms")).c_str());
		JMESSAGE((string("average world model time was ")+
			to_string(static_cast<double>(usec_world_model)/static_cast<double>(1000*num_cycles),2)+
			string(" ms")).c_str());
		JMESSAGE((string("average decision time was ")+
			to_string(static_cast<double>(usec_player)/static_cast<double>(1000*num_cycles),2)+
			string(" ms")).c_str());
		JMESSAGE((string("average robot communication time was ")+
			to_string(static_cast<double>(usec_robot)/static_cast<double>(1000*num_cycles),2)+
			string(" ms")).c_str());
		JMESSAGE((string("average user interface and communication time was ")+
			to_string(static_cast<double>(usec_user_interface)/static_cast<double>(1000*num_cycles),2)+
			string(" ms")).c_str());
		JMESSAGE((string("average idle time was ")+
			to_string(static_cast<double>(usec_idle)/static_cast<double>(1000*num_cycles),2)+
			string(" ms")).c_str());
    }
  }
  catch(exception&){;}  
  
  delete the_user_interface;
  delete the_image_processing;
  delete the_player;
  delete the_robot;
  delete the_world_model;
}

void ControlLoop::loop() throw () 
{
	
    stringstream gnuplot;
	gnuplot << "CONTROLLOOP_ELAPSED_GNUPLOT: "
	<< WorldModel::get_main_world_model().get_game_state().cycle_num
	<< " ";

	timestamp.update();
	Time expected_execution_time (timestamp);
	expected_execution_time.add_msec (loop_time+drive_vector_delay);
	num_cycles++;
	the_world_model->init_cycle(timestamp, expected_execution_time);

	if( report_computation_time ) 
		time_reporter.update();

	the_image_processing->process_image ();

	if( report_computation_time ) 
		usec_image_processing += time_reporter.elapsed_usec();
	if( report_computation_time_detailed ) 
		LOUT << "ImageProcessingTime: " << time_reporter.elapsed_usec() << " usec\n";
	if (report_computation_time_gnuplot) 
		gnuplot << time_reporter.elapsed_usec() << " ";

	if (report_computation_time) 
		time_reporter.update();

	the_world_model->update();

	if (report_computation_time) 
		usec_world_model += time_reporter.elapsed_usec();
	if (report_computation_time_detailed) 
		LOUT << "WorldModelTime: " << time_reporter.elapsed_usec() << " usec\n";
	if (report_computation_time_gnuplot) 
		gnuplot << time_reporter.elapsed_usec() << " ";

	if (report_computation_time) 
	   time_reporter.update();

	DriveVector intended_drive_vector = the_player->process_drive_vector (expected_execution_time);

	if (!is_number(intended_drive_vector.vtrans.x) || !is_number(intended_drive_vector.vtrans.y) 
	|| !is_number(intended_drive_vector.vrot)) 
	{
		JWARNING("Player liefert NAN in DriveVector");
		intended_drive_vector.vtrans.x=intended_drive_vector.vtrans.y=intended_drive_vector.vrot=0;
		intended_drive_vector.kick=false;
	} 
 	if (!the_world_model->get_game_state().in_game) 
	{  
		intended_drive_vector.vtrans.x=intended_drive_vector.vtrans.y=intended_drive_vector.vrot=0;
		intended_drive_vector.vaux[0]=intended_drive_vector.vaux[1]=intended_drive_vector.vaux[2]=intended_drive_vector.vaux[3]=0.0; 
		intended_drive_vector.kick=false;
	} 
		
	if (report_computation_time) 
		usec_player += time_reporter.elapsed_usec();
	if (report_computation_time_detailed) 
		LOUT << "PlayerTime: " << time_reporter.elapsed_usec() << " usec\n";
	if (report_computation_time_gnuplot) 
		gnuplot << time_reporter.elapsed_usec() << " ";
    
    if(report_computation_time)
		time_reporter.update(); 
	
	the_user_interface->process_messages();

    if (report_computation_time) usec_user_interface+=time_reporter.elapsed_usec();
    if (report_computation_time_detailed) 
		LOUT << "UserInterfaceTime: " << time_reporter.elapsed_usec() << " usec\n";
    if (report_computation_time_gnuplot) 
		gnuplot << time_reporter.elapsed_usec() << " ";
 
 	if (report_computation_time) 
		time_reporter.update();
	the_robot->set_drive_vector(intended_drive_vector);

	if (report_computation_time) 
		usec_robot+=time_reporter.elapsed_usec();
	if (report_computation_time_detailed) 
		LOUT << "RobotTime: " << time_reporter.elapsed_usec() << " usec\n";
	if (report_computation_time_gnuplot)
		gnuplot << time_reporter.elapsed_usec() << " ";
  
	if (report_computation_time_gnuplot) 
	{
		string str;
		getline(gnuplot, str);
		JMESSAGE(str.c_str());
	}

}