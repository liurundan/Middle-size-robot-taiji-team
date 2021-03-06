#include "StdAfx.h"
#include "RobotsUdpCommunication.h"
#include "Journal.h"
#include <sstream>

// #define DEBUG_MSG
const char RobotsUdpCommunication::PingTag            = 'a';
const char RobotsUdpCommunication::RequestSettingsTag = 'b';
const char RobotsUdpCommunication::RemoteCtrTag       = 'c';
const char RobotsUdpCommunication::GameStateTag       = 'd';
const char RobotsUdpCommunication::OwnHalfTag         = 'e';
const char RobotsUdpCommunication::FieldGeometryTag   = 'f';
const char RobotsUdpCommunication::BallLocationTag    = 'g';
const char RobotsUdpCommunication::RobotPropertiesTag = 'h';
const char RobotsUdpCommunication::RobotLocationTag   = 'i';
const char RobotsUdpCommunication::PlayerTypeTag      = 'j';
const char RobotsUdpCommunication::RobotDataTag       = 'k';
const char RobotsUdpCommunication::XmlStringTag       = 'x';

const int RobotsUdpCommunication::PingIdx            = 0;
const int RobotsUdpCommunication::RequestSettingsIdx = 1;
const int RobotsUdpCommunication::RemoteCtrIdx       = 2;
const int RobotsUdpCommunication::GameStateIdx       = 3;
const int RobotsUdpCommunication::OwnHalfIdx         = 4;
const int RobotsUdpCommunication::FieldGeometryIdx   = 5;
const int RobotsUdpCommunication::BallLocationIdx    = 6;
const int RobotsUdpCommunication::RobotPropertiesIdx = 7;
const int RobotsUdpCommunication::RobotLocationIdx   = 8;
const int RobotsUdpCommunication::PlayerTypeIdx      = 9;
const int RobotsUdpCommunication::RobotDataIdx       = 10;
const int RobotsUdpCommunication::XmlStringIdx       = 11;
const int RobotsUdpCommunication::NumIdx             = 12;
const int RobotsUdpCommunication::BUFFER_MAX_LEN     = 16384;  
//  RequestSettings implementation

RobotsUdpCommunication::RequestSettings::RequestSettings()
{
	set_all(never_send);
}

void RobotsUdpCommunication::RequestSettings::clear()
{
	set_all(never_send);
}

bool RobotsUdpCommunication::RequestSettings::set_all(RequestState state) 
{
	for (int i=0; i<NumIdx; i++)
		request[i]= state;
	return true;
}

bool RobotsUdpCommunication::RequestSettings::set_request(int MsgIdx, RequestState state) 
{
	if (MsgIdx < 0 || MsgIdx >= NumIdx )
		return false;
	request[MsgIdx]= state;
	return true;
}

// RobotsUdpCommunication implementation

RobotsUdpCommunication::RobotsUdpCommunication(bool server, int port, char const* addr)
{
	is_blocking = true;
	
	RemoteCtr       = new DriveVector;
	requestSettings = new RequestSettings;
	
	gameState       = new GameState;
	ownHalf         = new int;
	fieldGeometry   = new FieldGeometry;
	ballLocation    = new BallLocation;
	robotProperties = new RobotProperties;
	robotLocation   = new RobotLocation;
	playerType      = new char[200];
	robotData       = new RobotData();
	
	com_Buffer=new RobotsUdpCommunicationBuffer(100);
	
	ReceivedXMLStrings.clear();
	ReceivedXMLCmds.clear();
	ReceivedXMLRequests.clear();
	ReceivedXMLData.clear();
	
	if(server)
	{
		bool res=0;
		int _port=port;
		unsigned int max_server=8;
		while (max_server>0 && !res)
		{
			res=use_as_server(_port);
			_port++;
			max_server--;
		}
		//std::cerr << "Inited a server on port " << _port-1 << " with res: " << res << " \n";
	} 
	else
	{
		bool res=use_as_client(addr,port);
		//std::cerr << "Inited a client on port: " << port << " Addr: " << addr << " with res: " << res << "\n";
	}
	
    use_nonblock();
}

RobotsUdpCommunication::~RobotsUdpCommunication()
{
	delete RemoteCtr;
	delete requestSettings;
	delete gameState;
	delete ownHalf;
	delete fieldGeometry;
	delete ballLocation;
	delete robotProperties;
	delete robotLocation;
	delete playerType;
	delete robotData;
	delete com_Buffer;
}

bool RobotsUdpCommunication::use_as_server(int port)
{
	return sock.init_socket_fd(port);
}

bool RobotsUdpCommunication::use_nonblock() 
{
	sock.set_fd_nonblock();
	is_blocking = false;
	return true;
}

bool RobotsUdpCommunication::use_as_client(char const* addr, int port)
{
	sock.init_socket_fd ();
	return sock.init_serv_addr(addr,port);
}

bool RobotsUdpCommunication::clear_to_send()
{
	for (int i=0; i< NumIdx; i++)
		received[i]= 0;
	buffer_len= 0;
	ReceivedXMLStrings.clear();
	ReceivedXMLCmds.clear();
	ReceivedXMLData.clear();
	return true;
}

bool RobotsUdpCommunication::udp_receive_all(int timeout_sec) 
{
	fd_set rfds;
	struct timeval tv;
	int retval;
	
	int loopcounter=0;
	
	while(true) 
	{
		
		if (is_blocking && loopcounter==0)
			tv.tv_sec=timeout_sec;
		else
			tv.tv_sec=0;
		tv.tv_usec=0;
		
		FD_ZERO(&rfds);
		FD_SET(sock.socket_fd,&rfds);
		retval=select (sock.socket_fd+1,&rfds,NULL,NULL,&tv);
		if (retval<=0)
			return loopcounter>0;
		if (!FD_ISSET(sock.socket_fd,&rfds)) 
		{
			JERROR("Error file descriptor signalled change but nothing received.");
			return false;
		}
		bool res=udp_receive();
		if(!res) 
		{
			JERROR("Error tried to receive but no result");
			return false;
		}
		loopcounter++;
		if (loopcounter>=10 && loopcounter % 10 == 0)
			//JWARNING("Receiving a whole bunch of messages: "<<loopcounter);
			JWARNING("Receiving a whole bunch of messages: ");
	}
	
	return true;
}


bool RobotsUdpCommunication::udp_receive()
{
	bool res;
	if ( buffer_len > 0) 
	{
		JWARNING("the buffer is not cleared!");
	}
	buffer_len= 0;
	
	bool redirect= true;
	//bool redirect= false;
	
	res= sock.recv_msg(buffer,buffer_len,redirect);
	if (!res) 
	{
		if ( is_blocking )
			JERROR("problems with reading from socket");
		return false;
	}
	
	/*
	stringstream _output;
	_output << "buffer_len:" << buffer_len << endl;
	com_Buffer->print_hex(&_output,buffer,buffer_len);
	JERROR(_output.str().c_str());
	*/
	com_Buffer->set_receive_buffer(buffer,buffer_len,_VERSION_UDPCOMMUNICATION_HAUPT_);
	
	char * dum;
	
	//  get_next_buffer(char * dest,unsigned int dest_maxsize)
	while ((buffer_len=com_Buffer->get_next_buffer(buffer,BUFFER_MAX_LEN)) > 0)
	{
		//	std::cerr << "  upd_receive() buffer_len=" << buffer_len << "   " << endl;
		dum= buffer;
		if ( dum[0] == RemoteCtrTag )
		{
			received[RemoteCtrIdx]++;
			dum++;
			int size= sizeof(DriveVector);
			get_from_buffer( (void*)RemoteCtr, (void*)dum, size);
		} 
		else if ( dum[0] == PingTag )
		{
		/*		unsigned short _version;
		int size= sizeof(short);
		get_from_buffer( &_version, (void*)dum, size);
			*/		;
		} 
		else if ( dum[0] == RequestSettingsTag )
		{
			received[RequestSettingsIdx]++;
			dum++;
			int size= sizeof(RequestSettings);
			get_from_buffer( (void*)requestSettings, (void*)dum, size);
		} 
		else if ( dum[0] == GameStateTag )
		{
			received[GameStateIdx]++;
			dum++;
			int size= sizeof(GameState);
			get_from_buffer( (void*)gameState, (void*)dum, size);
		} 
		else if ( dum[0] == OwnHalfTag ) 
		{
			received[OwnHalfIdx]++;
			dum++;
			int size= sizeof(int);
			get_from_buffer( (void*)ownHalf, (void*)dum, size);
		} 
		else if ( dum[0] == FieldGeometryTag ) 
		{
			received[FieldGeometryIdx]++;
			dum++;
			int size= sizeof(FieldGeometry);
			get_from_buffer( (void*)fieldGeometry, (void*)dum, size);
		} 
		else if ( dum[0] == BallLocationTag ) 
		{
			received[BallLocationIdx]++;
			dum++;
			int size= sizeof(BallLocation);
			get_from_buffer( (void*)ballLocation, (void*)dum, size);
		}
		else if ( dum[0] == RobotPropertiesTag ) 
		{
			received[RobotPropertiesIdx]++;
			dum++;
			int size= sizeof(RobotProperties);
			get_from_buffer( (void*)robotProperties, (void*)dum, size);
		}
		else if ( dum[0] == RobotLocationTag ) 
		{
			received[RobotLocationIdx]++;
			dum++;
			int size= sizeof(RobotLocation);
			get_from_buffer( (void*)robotLocation, (void*)dum, size);
		} 
		else if ( dum[0] == PlayerTypeTag )
		{
			received[PlayerTypeIdx]++;
			dum++;
			int size;
			get_from_buffer( (void*) &size, (void*)dum, sizeof(int));
			dum += sizeof(int);
			get_from_buffer((void*)playerType, (void*)dum, size);
			playerType[size] ='\0';
		} 
		else if ( dum[0] == RobotDataTag )
		{
			received[RobotDataIdx]++;
			dum++;
			int size= sizeof(RobotData);
			get_from_buffer( (void*)robotData, (void*)dum, size);
		}
		else if ( dum[0] == XmlStringTag ) 
		{
			received[XmlStringIdx]++;
			dum++;
			int size;
			get_from_buffer( (void*) &size, (void*)dum, sizeof(int));   
			dum += sizeof(int);
			std::string s;
			s.assign(dum, size);
			ReceivedXMLStrings.push_back(s);
			buffer_len-=size;
		} 
		else
		{
			ostringstream e;
			e << "wrong communication tag in buffer: \n";
			e << "Tag[" << (dum[0]==0?'0':dum[0])<< "]\n";
			e << " buffer_len=" << buffer_len << "\n";
			JERROR(e.str().c_str());
			buffer_len= 0;
			return false;
		}
		
  }
  return true;
}

bool RobotsUdpCommunication::send() 
{
	unsigned int _size=0;
	_size=com_Buffer->get_send_buffer(buffer,BUFFER_MAX_LEN,_VERSION_UDPCOMMUNICATION_HAUPT_);
	//std::cerr << "send() size=" << _size << endl;
	//com_Buffer->print_hex(&cerr,buffer,_size);
	
	if (_size>0) return sock.send_msg(buffer,_size);
	
	return false;
}

bool RobotsUdpCommunication::get_from_buffer(void * obj, void * src, int size) 
{
	if ( size > buffer_len ) 
	{
		//JERROR("malformed message, size=" << size << " buffer_len=" << buffer_len);
		JERROR("malformed message, size=");
		buffer_len=0;
		return false;
	}
	
	buffer_len-= size;
	
	if ( ! obj ) 
	{
		JWARNING("received data, but no buffer initialized");
		return false;
	}
	
	memcpy((void*)obj,(void*)src, size);
	return true;
}


bool RobotsUdpCommunication::put_to_buffer(void * src, int size, char tag,bool rueckschein,unsigned char prio)
{
	if ((signed int)(size+sizeof(int)+1)<BUFFER_MAX_LEN)
	{
		char * dum= buffer;
		int offset= 0;
		
		// Daten vorbereiten
		dum[0]= tag;
		dum++;
		offset+=1;
		
		if (tag == PlayerTypeTag || tag == XmlStringTag)
		{
			memcpy(dum,(void *) &size, sizeof(int));
			dum+=sizeof(int);
			offset+=sizeof(int);
		}
		memcpy(dum, src,size);
		com_Buffer->put_to_buffer(buffer,size+offset,rueckschein,prio);
		
	} else
	{
		JERROR("buffer to small .. cant add data");
		return false;
	}
	return true;
}

bool RobotsUdpCommunication::receive_all()
{
#ifdef DEBUG_MSG
	JERROR("----- RECEIVE ALL -------\n");
#endif
	clear_to_send();
	
	udp_receive_all();
	
	processXmlStrings();
	
	bool received_something=false;
	for (int i=0; i<NumIdx; i++)
		if (received[i]>0)  
		{
			received_something = true;
		}
		
		return received_something;
}

bool RobotsUdpCommunication::setGameStateRequest()
{
	return requestSettings->set_request(GameStateIdx, send_once);
}

bool RobotsUdpCommunication::setOwnHalfRequest()
{
	return requestSettings->set_request(OwnHalfIdx, send_once);
}
bool RobotsUdpCommunication::setFieldGeometryRequest()
{
	return requestSettings->set_request(FieldGeometryIdx, send_once);
}
bool RobotsUdpCommunication::setBallLocationRequest()
{
	return requestSettings->set_request(BallLocationIdx, send_once);
}
bool RobotsUdpCommunication::setRobotPropertiesRequest()
{
	return requestSettings->set_request(RobotPropertiesIdx, send_once);
}
bool RobotsUdpCommunication::setRobotLocationRequest()
{
	return requestSettings->set_request(RobotLocationIdx, send_once);
}
bool RobotsUdpCommunication::setPlayerTypeRequest()
{
	return requestSettings->set_request(PlayerTypeIdx, send_once);
}
bool RobotsUdpCommunication::setRobotDataRequest()
{
	return requestSettings->set_request(RobotDataIdx, send_once);
}

bool RobotsUdpCommunication::setPlayerTypeListRequest()
{
	ostringstream s;
	s << "<R>"
		<< "PlayerTypeList"
		<< "</R>";
	putXmlString(s.str().c_str());
	return true;
}

bool RobotsUdpCommunication::setObstacleLocationRequest()
{
	ostringstream s;
	s << "<R>"
		<< "ObstacleLocation"
		<< "</R>";
	putXmlString(s.str().c_str());
	return true;
}

bool RobotsUdpCommunication::setVisibleObjectListRequest()
{
	ostringstream s;
	s << "<R>"
		<< "VisibleObjectList"
		<< "</R>";
	putXmlString(s.str().c_str());
	return true;
}

bool RobotsUdpCommunication::putRequests(bool rueckschein,unsigned char prio)
{
	return put_to_buffer( (void*)requestSettings ,sizeof(*requestSettings),RequestSettingsTag,rueckschein,prio);
}

bool RobotsUdpCommunication::clearRequests()
{
	requestSettings->clear();
	return true;
}

bool RobotsUdpCommunication::putRemoteCtr(DriveVector &drv,bool rueckschein,unsigned char prio)
{
	return put_to_buffer( (void*)&drv ,sizeof(drv),RemoteCtrTag,rueckschein,prio);
}
bool RobotsUdpCommunication::putGameState(GameState &gs,bool rueckschein,unsigned char prio)
{
	return put_to_buffer( (void*)&gs,sizeof(gs),GameStateTag,rueckschein,prio);
}

bool RobotsUdpCommunication::putOwnHalf(int &oh,bool rueckschein,unsigned char prio)
{
	return put_to_buffer( (void*)&oh,sizeof(oh),OwnHalfTag,rueckschein,prio);
}

bool RobotsUdpCommunication::putFieldGeometry(FieldGeometry& fg,bool rueckschein,unsigned char prio)
{
	return put_to_buffer( (void*)&fg,sizeof(fg),FieldGeometryTag,rueckschein,prio);
}

bool RobotsUdpCommunication::putBallLocation(BallLocation& bl,bool rueckschein,unsigned char prio)
{
	return put_to_buffer( (void*)&bl,sizeof(bl),BallLocationTag,rueckschein,prio);
}

bool  RobotsUdpCommunication::putRobotProperties(RobotProperties& rp,bool rueckschein,unsigned char prio)
{
	return put_to_buffer( (void*)&rp,sizeof(rp),RobotPropertiesTag,rueckschein,prio);
}

bool RobotsUdpCommunication::putRobotLocation(RobotLocation& rl,bool rueckschein,unsigned char prio)
{
	return put_to_buffer( (void*)&rl,sizeof(rl),RobotLocationTag,rueckschein,prio);
}

bool RobotsUdpCommunication::putPlayerType(char const * pt,bool rueckschein,unsigned char prio)
{
	return put_to_buffer((void *) pt, sizeof(char)*strlen(pt), PlayerTypeTag,rueckschein,prio); 
}
bool RobotsUdpCommunication::putRobotData(const RobotData& rd,bool rueckschein,unsigned char prio)
{
	return put_to_buffer( (void*)&rd,sizeof(rd),RobotDataTag,rueckschein,prio);
}

bool RobotsUdpCommunication::putXmlString(const char * s,bool rueckschein,unsigned char prio)
{
	return put_to_buffer((void *) s, sizeof(char)*strlen(s), XmlStringTag,rueckschein,prio);
}

bool RobotsUdpCommunication::getRemoteCtr(DriveVector &drv)
{
	if (received[RemoteCtrIdx]>0)
    {
		drv=*(RemoteCtr);
		return true;
    }
	return false;
}


bool RobotsUdpCommunication::getGameState(GameState &gs)
{
	if (received[GameStateIdx]>0)
	{
		gs=*(gameState);
		return true;
	}
	return false;
}

bool RobotsUdpCommunication::getOwnHalf(int &oh)
{
	if (received[OwnHalfIdx]>0)
	{
		oh=*(ownHalf);
		return true;
	}
	return false;
}

bool RobotsUdpCommunication::getFieldGeometry(FieldGeometry& fg)
{
	if (received[FieldGeometryIdx]>0)
	{
		fg=*(fieldGeometry);
		return true;
	}
	return false;
}

bool RobotsUdpCommunication::getBallLocation(BallLocation& bl)
{
	if (received[BallLocationIdx]>0)
	{
		bl=*(ballLocation);
		return true;
	}
	return false;
}

bool RobotsUdpCommunication::getRobotProperties(RobotProperties& rp)
{
	if (received[RobotPropertiesIdx]>0)
	{
		rp=*(robotProperties);
		return true;
	}
	return false;
}

bool RobotsUdpCommunication::getRobotLocation(RobotLocation& rl)
{
	if (received[RobotLocationIdx]>0)
	{
		rl=*(robotLocation);
		return true;
	}
	return false;
}
bool RobotsUdpCommunication::getPlayerType(std::string &pt)
{
	if (received[PlayerTypeIdx]<=0) return false;
	pt = std::string(playerType);
	return true;
}
bool RobotsUdpCommunication::getPlayerType(char * pt)
{
	if (received[PlayerTypeIdx]>0)
	{
		sprintf(pt, "%s", playerType);
		return true;
	}
	return false;
}
bool RobotsUdpCommunication::getRobotData(RobotData& rd)
{
	if (received[RobotDataIdx]>0)
	{
		rd=*(robotData);
		return true;
	}
	return false;
}

bool RobotsUdpCommunication::processXmlStrings()
{
	for (unsigned int i=0; i<ReceivedXMLStrings.size(); i++)
    {
		//std::cerr << "Received XmlString " << (int) i << " : [" <<  ReceivedXMLStrings[i] << "]\n";
		// Hole alle Befehle aus XML Strings
		getXmlSubstringList("!", ReceivedXMLStrings[i], ReceivedXMLCmds);
		getXmlSubstringList("@", ReceivedXMLStrings[i], ReceivedXMLData);
		getXmlSubstringList("R", ReceivedXMLStrings[i], ReceivedXMLRequests);
    }
	
	//  ostringstream e;
	//   e << "XMLSTRING:\n";
	//   for (unsigned int i=0; i<ReceivedXMLStrings.size(); i++)
	//     {
	//       e << "Received CMDXmlString " << i << " : [" << ReceivedXMLStrings[i] << "]\n";
	//     }
	//   e << "XMLREQ:\n";
	//    for (unsigned int i=0; i<ReceivedXMLRequests.size(); i++)
	//     {
	//       e << "Received CMDXmlString " << i << " : [" << ReceivedXMLRequests[i] << "]\n";
	//     }
	//   JERROR(e.str().c_str());
	return true;
}

bool RobotsUdpCommunication::putSLHint(float x_mm, float y_mm, float heading_rad,bool rueckschein,unsigned char prio)
{
	std::ostringstream s;
	s << "<!>"
		<< "<SLHint3>"
		<< "<x>" << x_mm << "</x>"
		<< "<y>" << y_mm << "</y>"
		<< "<heading>" << heading_rad << "</heading>"
		<< "</SLHint3>"
		<< "</!>";
	putXmlString(s.str().c_str(),rueckschein,prio);
	return true;
}
bool RobotsUdpCommunication::putSLHint(float x_mm, float y_mm,bool rueckschein,unsigned char prio)
{
	std::ostringstream s;
	s << "<!>"
		<< "<SLHint2>"
		<< "<x>" << x_mm << "</x>"
		<< "<y>" << y_mm << "</y>"
		<< "</SLHint2>"
		<< "</!>";
	putXmlString(s.str().c_str(),rueckschein,prio);
	return true;
}

bool RobotsUdpCommunication::putPlayerTypeList(const vector<string>& ptl,bool rueckschein,unsigned char prio)
{
	std::ostringstream s;
	s << "<@>"
		<< "<PlayerTypeList>";
	for (unsigned int i=0; i<ptl.size(); i++)   
	{
		s << "<PlayerType>"
			<< ptl[i].c_str()
			<< "</PlayerType>";
	}
	s << "</PlayerTypeList>"
		<< "</@>";
	
	// ostringstream e;
	//   e << "Received Answer Request: " << s.str() << "\n";  JERROR(e.str().c_str());
	
	putXmlString(s.str().c_str(),rueckschein,prio);
	return true;
}

bool RobotsUdpCommunication::getPlayerTypeList(vector<string>& ptl)
{
	vector<string> data;
	for (unsigned int i=0;i<ReceivedXMLData.size(); i++)
	{
		getXmlSubstringList("PlayerTypeList",ReceivedXMLData[i] , data);
	}
	if (data.size() < 1) 
		return false;
	ptl.clear();
	getXmlSubstringList("PlayerType", data[data.size()-1], ptl);
	return true;
}


bool RobotsUdpCommunication::putMessage(const string& msg,bool rueckschein,unsigned char prio)
{
	std::ostringstream s;
	s << "<@>"
		<< "<Message>" << msg.c_str() << "</Message>"
		<< "</@>";
#ifdef DEBUG_MSG
	std::ostringstream o;
	o << "Put Message:[" << msg.c_str() << "]\n";
	JERROR(o.str().c_str());
#endif
	putXmlString(s.str().c_str(),rueckschein,prio);
	return true;
}

bool RobotsUdpCommunication::putMessageList(const vector<string>& msgList,bool rueckschein,unsigned char prio)
{
	for (unsigned int i=0; i<msgList.size(); i++)
	{
		putMessage(msgList[i],rueckschein,prio);
	}
	return true;
}

bool RobotsUdpCommunication::getMessageList(vector<string>& msgList)
{
	msgList.clear();
	for (unsigned int i=0;i<ReceivedXMLData.size(); i++)
	{
		getXmlSubstringList("Message",ReceivedXMLData[i], msgList);
	}
	
#ifdef DEBUG_MSG
	for (unsigned int i=0; i<msgList.size(); i++)
    {
		std::ostringstream o;
		o << "Get Message:[" << msgList[i].c_str() << "]\n";
		JERROR(o.str().c_str());
    }
#endif
	
	return (msgList.size()>0);
}

bool RobotsUdpCommunication::getSLHint(float &x_mm, float &y_mm, float &heading_rad)
{
	std::vector<std::string> params;
	
	for (unsigned int i=0;i<ReceivedXMLCmds.size(); i++)
	{
		//std::cerr << "Searching for com SLHint3\n";
		getXmlSubstringList("SLHint3",ReceivedXMLCmds[i] , params);
	}
	if (params.size()<1)  
	{
		//std::cerr << "Can't find com SLHint3\n";
		return false;
	}
	//std::cerr << "Found com SLHint3\n";
	// Just read the first received command for the moment
	std::vector<std::string> param;
	
	getXmlSubstringList("x",params[0], param);
	if (param.size()<1) 
	{ return false;}
	x_mm = atof(param[0].c_str());
	
	param.clear();
	getXmlSubstringList("y",params[0], param);
	if (param.size()<1) {return false;}
	y_mm = atof(param[0].c_str());
	
	param.clear();
	getXmlSubstringList("heading",params[0], param);
	if (param.size()<1) 
	{ return false;}
	heading_rad = atof(param[0].c_str());
	
	return true;
}

bool RobotsUdpCommunication::getSLHint(float &x_mm, float &y_mm)
{
	std::vector<std::string> params;
	
	for (unsigned int i=0;i<ReceivedXMLCmds.size(); i++)
	{ 
		getXmlSubstringList("SLHint2",ReceivedXMLCmds[i] , params);
	}
	if (params.size()<1)
	{
		return false;
	}
	// Just read the first received command for the moment
	std::vector<std::string> param;
	
	getXmlSubstringList("x",params[0], param);
	if (param.size()<1) 
	{ return false;}
	x_mm = atof(param[0].c_str());
	
	param.clear();
	getXmlSubstringList("y",params[0], param);
	if (param.size()<1) 
	{ return false;}
	y_mm = atof(param[0].c_str());
	
	return true;
}

void RobotsUdpCommunication::getXmlSubstringList(std::string tag, std::string src, std::vector<std::string> &target)
{
	unsigned int lstb = 0, lste = 0;
	unsigned int letb = 0, lete = 0;
	
	std::string beginTag = "<"+tag , endTag= "</"+tag+">";
	
	//std::cerr << "Searching in [" << src << "] for [" << tag << "]\n";
	
	bool found=true;
	do {
		lstb = src.find(beginTag,lete);
		if (lstb==std::string::npos){found = false;  break;}
		lste = src.find(">",lstb);
		if (lste==std::string::npos){found = false;  break;}
		letb = src.find(endTag , lste);
		if (letb==std::string::npos){found = false;  break;}
		lete = src.find(">" , letb);
		if (lete==std::string::npos){found = false;  break;}
		
		std::string sub = src.substr(lste+1, letb - lste - 1);
		//std::cerr << "Found substring: [" << sub << "]\n";
		
		target.push_back(sub);
		
	} while (found);
	
}


bool RobotsUdpCommunication::putInGame(bool& ig,bool rueckschein,unsigned char prio) {
	std::ostringstream s;
	s << "<!>"
		<< "<InGame>" << (ig ? "YES" : "NO") << "</InGame>"
		<< "</!>";
	putXmlString(s.str().c_str(),rueckschein,prio);
	return true;
}

bool RobotsUdpCommunication::getInGame(bool& ig) 
{
	std::vector<std::string> params;
	for (unsigned int i=0;i<ReceivedXMLCmds.size(); i++)
    { 
		getXmlSubstringList("InGame",ReceivedXMLCmds[i] , params);
    }
	if (params.size()<1){
		return false;
	}
	ig = (params[0]=="YES");
	return true;
}

bool RobotsUdpCommunication::putRefboxSignal (RefboxSignal& rs,bool rueckschein,unsigned char prio) {
	std::ostringstream s;
	s << "<!>"
		<< "<RefBox>" << rs << "</RefBox>"
		<< "</!>";
	putXmlString(s.str().c_str(),rueckschein,prio);
	return true;
}

bool RobotsUdpCommunication::getRefboxSignal (RefboxSignal& rs)  
{
	std::vector<std::string> params;
	for (unsigned int i=0;i<ReceivedXMLCmds.size(); i++)
    { 
		getXmlSubstringList("RefBox",ReceivedXMLCmds[i] , params);
    }
	if (params.size()<1){
		return false;
	}
	rs = RefboxSignal(atoi(params[0].c_str()));
	return true;
}


bool RobotsUdpCommunication::putObstacleLocation(const ObstacleLocation & ol,bool rueckschein,unsigned char prio)
{
	std::ostringstream s;
	s << "<@>"
		<< "<ObstacleLocation>";
	ol.writeAt(s);
	s << "</ObstacleLocation>"
		<< "</@>";
	putXmlString(s.str().c_str(),rueckschein,prio);
	return true;
}

bool RobotsUdpCommunication::getObstacleLocation(ObstacleLocation & ol)
{
	std::vector<std::string> params;
	for (unsigned int i=0;i<ReceivedXMLData.size(); i++)
    { 
		getXmlSubstringList("ObstacleLocation",ReceivedXMLData[i] , params);
    }
	if (params.size()<1){
		return false;
	}
	
	std::stringstream s;
	s << params[0].c_str();
	return (ol.readFrom(s) > 0);
}

bool RobotsUdpCommunication::putVisibleObjectList(const VisibleObjectList &ol,bool rueckschein,unsigned char prio)
{
	std::ostringstream s;
	s << "<@>"
		<< "<VisibleObjectList>";
	ol.writeAt(s);
	s << "</VisibleObjectList>"
		<< "</@>";
	putXmlString(s.str().c_str(),rueckschein,prio);
	return true;
}

bool RobotsUdpCommunication::getVisibleObjectList(VisibleObjectList &ol)
{
	std::vector<std::string> params;
	for (unsigned int i=0;i<ReceivedXMLData.size(); i++)
    { 
		getXmlSubstringList("VisibleObjectList",ReceivedXMLData[i] , params);
    }
	if (params.size()<1)
	{
		return false;
	}
	
	std::stringstream s;
	s << params[0].c_str();
	return (ol.readFrom(s) > 0);
}

unsigned int RobotsUdpCommunication::getVersion()
{
	return _VERSION_UDPCOMMUNICATION_HAUPT_;
}
