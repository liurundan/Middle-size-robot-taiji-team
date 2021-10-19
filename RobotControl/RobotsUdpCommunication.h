#ifndef _UDPCOMMUNICATION_H_
#define _UDPCOMMUNICATION_H_

// Versionsnummer vom Protokoll
#define _VERSION_UDPCOMMUNICATION_HAUPT_ 12


#include "udpsocket.h"
#include "DriveVector.h"
#include "FieldGeometry.h"
#include "BallLocation.h"
#include "RobotProperties.h"
#include "RobotLocation.h"
#include "RobotData.h"
#include "GameState.h"
#include "ObstacleLocation.h"
#include "VisibleObject.h"
#include "RobotsUdpCommunicationBuffer.h"

#include <vector>
#include <string>

using namespace std;

class RobotsUdpCommunication
{
public:
  RobotsUdpCommunication(bool server=true, int port=6012, char const* addr=0);
  ~RobotsUdpCommunication();
   
  bool clear_to_send();
  bool send();

  bool receive_all();

  bool putRequests(bool rueckschein=false,unsigned char prio=1);// the request set will be put to buffer for sending client will use this
  bool clearRequests();                  // old requests will be cleared client will use this
  bool setGameStateRequest();            // set the requests for information client will use this
  bool setOwnHalfRequest();              // ...
  bool setFieldGeometryRequest();
  bool setBallLocationRequest();
  bool setRobotPropertiesRequest();
  bool setRobotLocationRequest();
  bool setPlayerTypeRequest();
  bool setRobotDataRequest();
  bool setObstacleLocationRequest();
  bool setVisibleObjectListRequest();
  // No Request for Messages ... tested and sent every cycle

  bool setPlayerTypeListRequest();

  bool putInGame (bool& ig,bool rueckschein=true,unsigned char prio=0);
  bool getInGame (bool& ig);

  bool putRefboxSignal (RefboxSignal& rs,bool rueckschein=true,unsigned char prio=0); 
  bool getRefboxSignal (RefboxSignal& rs); 

  bool getRemoteCtr(DriveVector &drv);
  bool putRemoteCtr(DriveVector &drv,bool rueckschein=false,unsigned char prio=1);

  bool getGameState(GameState &gs);
  bool putGameState(GameState &gs,bool rueckschein=false,unsigned char prio=0);

  bool getOwnHalf(int &oh);
  bool putOwnHalf(int &oh,bool rueckschein=false,unsigned char prio=0);

  bool getFieldGeometry(FieldGeometry& fg);
  bool putFieldGeometry(FieldGeometry& fg,bool rueckschein=false,unsigned char prio=1);

  bool getBallLocation(BallLocation& bl);
  bool putBallLocation(BallLocation& bl,bool rueckschein=false,unsigned char prio=1);

  bool getRobotProperties(RobotProperties& rp);
  bool putRobotProperties(RobotProperties& rp,bool rueckschein=false,unsigned char prio=1);

  bool getRobotLocation(RobotLocation& rl);
  bool putRobotLocation(RobotLocation& rl,bool rueckschein=false,unsigned char prio=1);

  bool getPlayerType(char * pt);
  bool getPlayerType(std::string &pt);
  bool putPlayerType(char const * pt,bool rueckschein=false,unsigned char prio=1);

  bool getPlayerTypeList(vector<string>& ptl);
  bool putPlayerTypeList(const vector<string>& ptl,bool rueckschein=true,unsigned char prio=1);

  bool getRobotData(RobotData& rd);
  bool putRobotData(const RobotData& rd,bool rueckschein=false,unsigned char prio=1);

  bool putMessage(const string& msg,bool rueckschein=false,unsigned char prio=1);
  bool putMessageList(const vector<string>& msgList,bool rueckschein=false,unsigned char prio=1);
  bool getMessageList(vector<string>& msgList);

  bool putObstacleLocation(const ObstacleLocation & ol,bool rueckschein=false,unsigned char prio=2);
  bool getObstacleLocation(ObstacleLocation & ol);

  bool putVisibleObjectList(const VisibleObjectList &ol,bool rueckschein=false,unsigned char prio=2);
  bool getVisibleObjectList(VisibleObjectList &ol);

  // Some Xml func to try com, use for new commands
  bool putXmlString(const char * s,bool rueckschein=false,unsigned char prio=1);

  bool putSLHint(float x_mm, float y_mm, float heading_rad,bool rueckschein=false,unsigned char prio=1);
  bool putSLHint(float x_mm, float y_mm,bool rueckschein=false,unsigned char prio=1);

  bool getSLHint(float &x_mm, float &y_mm, float &heading_rad);
  bool getSLHint(float &x_mm, float &y_mm);

  void getXmlSubstringList(std::string tag, std::string src, std::vector<std::string> &target);
  
  unsigned int getVersion();
  
public:
  enum RequestState { never_send, send_once};   // maybe later , send_reqular };
  struct RequestSettings 
  {
    RequestSettings();
    RequestState request[12];
    void clear();
    bool set_all(RequestState state);
    bool set_request(int MsgIdx, RequestState state);
  };
  static const int NumIdx;

protected:

	 // some tags for communication
  static const char PingTag;
  static const char RequestSettingsTag;
  static const char RemoteCtrTag;
  static const char GameStateTag;
  static const char OwnHalfTag;
  static const char FieldGeometryTag;
  static const char BallLocationTag;
  static const char RobotPropertiesTag;
  static const char RobotLocationTag;
  static const char PlayerTypeTag;
  static const char RobotDataTag ;
  static const char XmlStringTag;
 
 // define some indexes and structs for storing things
  static const int PingIdx;
  static const int RequestSettingsIdx ;
  static const int RemoteCtrIdx ;
  static const int GameStateIdx ;
  static const int OwnHalfIdx;
  static const int FieldGeometryIdx;
  static const int BallLocationIdx ;
  static const int RobotPropertiesIdx ;
  static const int RobotLocationIdx;
  static const int PlayerTypeIdx ;
  static const int RobotDataIdx ;
  static const int XmlStringIdx ;
  int  received[12];

  vector<string> ReceivedXMLStrings;
  vector<string> ReceivedXMLCmds;
  vector<string> ReceivedXMLRequests;
  vector<string> ReceivedXMLData;

  // the socket for communication
  UDPsocket sock;

  // buffer for communication
  static const int BUFFER_MAX_LEN;  
  //static const int BUFFER_MAX_LEN= 8000;
   // fehlerhafte pakete auf 8096 wurden empfangen, mal versuchen was passiert wenn man drunter bleibt. 
   // Da muss es ne grenze geben bei der uebertragung, mit 8000 wurden keine pakete abgeschnitte.
  
  int buffer_len;
  char buffer[16384];
  
  bool is_blocking;

  //protected functions
  bool put_to_buffer(void * src, int size, char tag,bool rueckschein,unsigned char prio);
  bool get_from_buffer(void * obj, void * src, int size);
  


  
  RequestSettings * requestSettings;
  DriveVector     * RemoteCtr;
  
  // some pointers to read from
  GameState       * gameState;
  int             * ownHalf;
  FieldGeometry   * fieldGeometry;
  BallLocation    * ballLocation;
  RobotProperties * robotProperties;
  RobotLocation   * robotLocation;
  char            * playerType;
  RobotData       * robotData;

  RobotsUdpCommunicationBuffer * com_Buffer;
     
  bool use_as_server(int port);
  bool use_as_client(char const* addr, int port);
  
  bool use_nonblock();
 
  bool udp_receive();
  bool udp_receive_all(int timeout_sec=1);

  bool processXmlStrings();
  

};

#endif