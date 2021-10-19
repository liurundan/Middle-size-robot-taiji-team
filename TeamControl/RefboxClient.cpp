#include "StdAfx.h"
#include "RefboxClient.h"
#include "Time.h"
#include <iostream>
#include <Winsock2.h>

using namespace std;

RefboxClient::RefboxClient (const char* lname, CWnd* pParent) throw () : logstream (lname), pOwner(pParent) 
{
	latest_signal = SIGstop;
	okayfailed    = 0;
	connected     = false;
}

RefboxClient::~RefboxClient () throw () 
{
	disconnect();
	logstream.flush();
}

namespace 
{
	inline int socket_connect (int socket, struct sockaddr *addr, int length)  // socklen_t
	{   
		return connect (socket, addr, length);
	}
}

bool RefboxClient::connect (const char* ip, int port) throw () 
{
	Time now;
	
	if (connected)
		disconnect ();     
	
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 1, 1 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) 
	{
		return false;
	}
	if ( LOBYTE( wsaData.wVersion ) != 1 ||
		HIBYTE( wsaData.wVersion ) != 1 ) 
	{
		WSACleanup( );
		return false; 
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	struct hostent* serveraddr;

	CString sMsg = "";
	if ( (serveraddr = gethostbyname(ip)) == 0) 
	{
		if (pOwner)
		{
			sMsg = "RefboxClient:  unknown host ";
			::SendMessage(pOwner->m_hWnd,connectMessage,(WPARAM)&sMsg,0);
		}
		logstream << now << " unknown host; couldn't connect to refbox" << endl;
		return false;
	}
	memcpy(&address.sin_addr, serveraddr->h_addr, serveraddr->h_length);
	address.sin_port = htons(port);
	int result = socket_connect(sockfd, (struct sockaddr *) &address, sizeof(address));
	
	if (result != 0) 
	{
		if (pOwner)
		{
			sMsg = "RefboxClient: couldn't connect to server";
			::SendMessage(pOwner->m_hWnd,connectMessage,(WPARAM)&sMsg,0);
		}

		logstream << now << " couldn't connect to refbox" << endl;
		AfxMessageBox("Could not connect to server");
		return false;
	}

	if (pOwner)
	{
		sMsg.Format(" successfully connected to %s , %d. Waiting for get welcome message... ",
						inet_ntoa(address.sin_addr), ntohs(address.sin_port) );
		::SendMessage(pOwner->m_hWnd,connectMessage,(WPARAM)&sMsg,0);
	}

	// cout << "RefboxClient: successfully connected to " << inet_ntoa(address.sin_addr) << ", " << ntohs(address.sin_port) << endl;
	logstream << now << " successfully connected to " << inet_ntoa(address.sin_addr) << ", " << ntohs(address.sin_port) << endl;
	
	// get welcome message
	char rcvd[10];
	rcvd[0]='\0';
 	Time t;
	if ( recv( sockfd, rcvd, 10, 0 ) <= 0 ) 
	{
		// cout << "RefboxClient: connection lost" << endl;
		if (pOwner)
		{
			sMsg = "RefboxClient: connection lost";
			::SendMessage(pOwner->m_hWnd,connectMessage,(WPARAM)&sMsg,0);
		}

		logstream << now << " connection lost" << endl;
		return false;
	}
	 
	connected=true;
	okayfailed=0;
	// cout << "RefboxClient: got welcome message = '" << rcvd << "'." << endl;
	if (pOwner)
	{
		CString str(rcvd);
		sMsg.Format("RefboxClient: got welcome message, consuming time was %d s", t.elapsed_sec() );
		::SendMessage(pOwner->m_hWnd,connectMessage,(WPARAM)&sMsg,0);
	}
	logstream << now << " got welcome message = '" << rcvd << "'." << endl;
	return true;
}

void RefboxClient::disconnect() throw () 
{
	Time now;
	if (connected) 
	{
		logstream << now << " disconnect from refbox" << endl;
		closesocket(sockfd);
		
		WSACleanup( );
		
		connected=false;
	}
}

RefboxSignal RefboxClient::listen() throw () 
{
	Time now;
	if (!connected)
		return SIGnop;
	
	fd_set rfds;
	struct timeval tv;
	FD_ZERO(&rfds);
	FD_SET(sockfd, &rfds);
	tv.tv_sec = 0; tv.tv_usec = 100;
	int sc = select(static_cast<int>(sockfd)+1, &rfds, NULL, NULL, &tv);
	okayfailed = ((sc==0) ? 0 : okayfailed+1);
	bool message_available = FD_ISSET(sockfd, &rfds) != 0 ? true : false;
	
	if (!message_available) 
	{
		return SIGnop;     
	}
	
	char rcvd[10];
	if (recv( sockfd, rcvd, 10, 0 ) <0)  // edit by lrd read
	{
		logstream << now << " error when listening for message from refbox" << endl;
		return SIGnop;    
	}
	
	logstream << now << " received message from refbox: '" << rcvd[0] << "' = " << static_cast<int>(rcvd[0]) << "." << endl;
	
	bool relevant_signal=true;
	RefboxSignal res = SIGnop;
	switch (rcvd [0]) 
	{
	case 'a': relevant_signal=false; res=SIGmagentaGoalScored; break;
	case 'A': relevant_signal=false; res=SIGcyanGoalScored; break;
	case 'H': res=SIGhalt; break;
	case 'S': res=SIGstop; break;
	case 's':
	case '1':
	case '2':
		if (latest_signal == SIGstop || latest_signal == SIGhalt)
			res=SIGstart;
		else
			res=SIGready;
		break;
	case 'x': res=SIGstop; break;
	case 'k': res=SIGmagentaKickOff; break;
	case 'K': res=SIGcyanKickOff; break;
	case 'f': res=SIGmagentaFreeKick; break;
	case 'F': res=SIGcyanFreeKick; break;
	case 'g': res=SIGmagentaGoalKick; break;
	case 'G': res=SIGcyanGoalKick; break;
	case 'c': res=SIGmagentaCornerKick; break;
	case 'C': res=SIGcyanCornerKick; break;
	case 't': res=SIGmagentaThrowIn; break;
	case 'T': res=SIGcyanThrowIn; break;
	case 'p': res=SIGmagentaPenalty; break;
	case 'P': res=SIGcyanPenalty; break;
	case 'N': res=SIGdroppedBall; break;
	default: relevant_signal=false; res=SIGnop;
	}
	if (relevant_signal)
		latest_signal = res;
	logstream << now << " interpret refbox message as " << refbox_signal_names[res] << endl;
	return res;
}

bool RefboxClient::is_okay () const throw () 
{
	return (okayfailed<=6) && connected;
}

bool RefboxClient::is_connected () const throw () 
{
	return connected;
}


