#ifndef _udp_server_h_
#define _udp_server_h_

#include "RobotsUdpCommunication.h"
#include "WorldModel.h"
#include "Player.h"

class RobotsUdpServer: public RobotsUdpCommunication
{
public:
	static RobotsUdpServer *get_instance_pointer(int port=6012);
    static void delete_instance();
    ~RobotsUdpServer();

    bool processRequests(WorldModel &_wm, Player & _pl);
    bool processXMLRequests(WorldModel &_wm, Player & _pl);
    static bool hasInstance();

protected:
	static RobotsUdpServer  * tribotsUdpCom;

    RobotsUdpServer(int port=6012);

};

#endif