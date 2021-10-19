

#ifndef _ODESIMCOMMUNICATION_H_
#define _ODESIMCOMMUNICATION_H_

#include "udpsocket_ode.h"
//#include "../macro_msg.h"
//#include "../global_defs.h"
#include <iostream>

using namespace std;
#define MAX_CLIENTS 16

struct SimState
{
  double pos[3];
  double robvel[3];
  double ball[3];
  double steer[3];
  bool kick;
  int num_obstacles;
  double obstacles[2][16]; 
  SimState()
  {
    kick=false;
    pos[0]=0;pos[1]=0;pos[2]=0;
    robvel[0]=0;robvel[1]=0;robvel[2]=0;
    ball[0]=0;ball[1]=0;ball[2]=0;
    steer[0]=0;steer[1]=0;steer[2]=0;
  }
};

struct ServerData
{
  // Adresses of connected clients
  struct sockaddr_in	serv_addr[MAX_CLIENTS];
  bool client_active[MAX_CLIENTS];
  int client_not_responding[MAX_CLIENTS];
  ServerData()
  {
    for (int i=0;i<MAX_CLIENTS;i++) 
	{
    	client_not_responding[i]=0;
	    client_active[i]=false;
	
 	}
  
  }
  int is_active_client(sockaddr_in client_addr);
  int add_client(sockaddr_in client_addr);
  void check_for_inactivity();
  

};



class OdeSimComm
{
  static const int BUFFER_MAX_LEN;
  int buffer_len;
  char buffer[8096];
  bool is_server;

public:
  UDPsocketOde sock;
  
  OdeSimComm()
  {
    simstate= new SimState();
    is_server=false;
  }

  ~OdeSimComm();
  bool use_as_server(int port);
  bool use_as_client(char const* addr, int port);

  bool use_nonblock();

  SimState        * simstate;

  bool send();
  bool receive();

  int last_client;
  ServerData serverdata;
  
};
 

#endif


