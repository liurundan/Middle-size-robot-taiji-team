#include "StdAfx.h"
#include "udpsocket.h"
#include "Journal.h"
#include <iostream>
#include <string.h>

using namespace std;

#define bzero(a, b) memset(a, 0, b)

const int UDPsocket::MAXMESG = 8096;

bool UDPsocket::init_socket_fd(int port) 
{
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
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	int fd;
	if( (fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
	{
		JERROR("can't open socket");
		return false ;        
	}
	
	socket_fd = fd;
	
	if (port <= 0)
		return true;
	
	struct sockaddr_in cli_addr;
	bzero((char *) &cli_addr, sizeof(cli_addr)) ;
	cli_addr.sin_family           = AF_INET ;
	cli_addr.sin_addr.s_addr      = htonl(INADDR_ANY) ;
	cli_addr.sin_port             = htons(port) ;
	
	if (bind(fd, (struct sockaddr *) &cli_addr, sizeof(cli_addr)) < 0)
	{
		char ebuf[100];
		sprintf(ebuf, "can't bind local address (port= %d)", port);
		JERROR(ebuf);
		return false ;  
	}
	
	return true;
}

bool UDPsocket::init_serv_addr(const char* host, int port) 
{
	struct hostent      *host_ent ;
	struct in_addr      *addr_ptr ;
	
	if((host_ent = (struct hostent *)gethostbyname(host)) == NULL) 
	{
		if((int)inet_addr(host) == -1)
		{
			JERROR("could'n initialize server address");
			return false;
		}
	}
	else 
	{
		addr_ptr = (struct in_addr *) *host_ent->h_addr_list ;
		host = inet_ntoa(*addr_ptr) ;
	}
	
	bzero((char *) &serv_addr, sizeof(serv_addr)) ;
	serv_addr.sin_family		= AF_INET ;
	serv_addr.sin_addr.s_addr	= inet_addr(host) ;
	serv_addr.sin_port		= htons(port) ;
	return true;
}

bool UDPsocket::send_msg(const char *buf, int len) 
{
	if (!buf) return false;
	//n = strlen(buf) ;
    
	int res = sendto(socket_fd, buf, len, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (res != len ) 
	{
		// perror("socket send_msg");
		JERROR("problems with sending");
		return false ;
	}
	
	return true;
}

bool UDPsocket::recv_msg(char *buf, int & len, bool redirect) 
{
	return recv_msg(buf,len,MAXMESG,redirect);
}

bool UDPsocket::recv_msg(char *buf, int & len, int max_len, bool redirect) 
{
	int addr_len ;
	struct sockaddr_in    recv_addr ;
	
	addr_len = (unsigned int) sizeof(recv_addr) ;
	len = recvfrom(socket_fd, buf, max_len, 0,(struct sockaddr *)&recv_addr, &addr_len);
	if(len < 0) 
	{
		len = 0;
		buf[0]= '\0';
		return false ;
	}
	buf[len] = '\0' ;
	
#if 1
	if ( redirect && 
		(serv_addr.sin_addr.s_addr != recv_addr.sin_addr.s_addr  
		|| serv_addr.sin_port != recv_addr.sin_port ))
	{          
		JMESSAGE("Now sender redirected from port");
		serv_addr = recv_addr;
	}
#else  
	if ( redirect  
		&& serv_addr.sin_addr.s_addr == recv_addr.sin_addr.s_addr  
		&& serv_addr.sin_port != recv_addr.sin_port ) 
	{          
		JMESSAGE("\nNow sender redirected from port " << ntohs(serv_addr.sin_port)
			<< " to port " << ntohs(recv_addr.sin_port) << " on same server");
		serv_addr = recv_addr;
    }
#endif  
	return true;
	
}

void UDPsocket::set_fd_nonblock(int fd) 
{
  u_long para = 1;
  ioctlsocket(fd,FIONBIO,&para);	
}

void UDPsocket::set_fd_sigio(int fd) 
{
	
}

void UDPsocket::close_socket_fd() 
{
	closesocket(socket_fd);
	WSACleanup( );
}

