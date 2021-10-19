#ifndef _UDPCOMMUNICATION_BUFFER_H_
#define _UDPCOMMUNICATION_BUFFER_H_

#include <sstream>

#define MAX_NUTZDATENLAENGE 8192
// 0-400 Linien+hindernisse ~8KB

#define MAX_NUTZDATEN   100

#define _SENDEVERSUCHE_ 50
// Server und client sind unterschiedlich schnell!!

#define _ROBOTS_UDPCOMMUNICATION_BUFFER_STAT_
#define _ROBOTS_UDPCOMMUNICATION_LOG

using namespace std;

class RobotsUdpCommunicationBuffer
{
public:
	RobotsUdpCommunicationBuffer(unsigned int eintraege);
	~RobotsUdpCommunicationBuffer();
	bool put_to_buffer(char * src,unsigned int size,bool rueckschein,unsigned char prio);
	unsigned int get_send_buffer(char * dest,unsigned int maxsize,unsigned int version); 
	bool set_receive_buffer(char * src,unsigned int src_size,unsigned int version); 
	unsigned int get_next_buffer(char * dest,unsigned int dest_maxsize); 
	
	// debug
	void print_hex(ostream * output,char * src,unsigned int size);
	
public:
	static const unsigned char art_leer;
	static const unsigned char art_ausgang_normal;
	static const unsigned char art_ausgang_rueckschein;
	static const unsigned char art_ausgang_rueckscheinantwort;
	static const unsigned char art_eingang_normal;
	static const unsigned char art_version_info;

	static const unsigned char prio_wichtig	;
	static const unsigned char prio_normal	;
	static const unsigned char prio_optional;

protected:
	unsigned int max_eintraege;
	
	unsigned int versionsnummer_uebertragungen;
	
	unsigned long e_send_buffer_full;
	unsigned long e_put_to_buffer_full;
	unsigned long e_put_to_buffer_tobig;
	unsigned long e_set_receive_buffer;
	unsigned long e_set_receive_buffer_tobig;
	unsigned long e_get_next_buffer_tobig;
	unsigned long e_receive_unknow_data;
	unsigned long e_receive_data_checksum;
	unsigned long w_receive_to_old;
	
#ifdef _ROBOTS_UDPCOMMUNICATION_BUFFER_STAT_
	unsigned long stat_data_send;
	unsigned long stat_data_receive;
	unsigned int stat_max_nutzdatenlaenge_send;
	char stat_max_nutzdatenlaenge_send_art;
	unsigned int stat_max_bufferlaenge_send;
	unsigned long stat_data_send_prio[3];
#endif
	
	unsigned int fortlaufende_paketnummer;
	unsigned int fortlaufende_paketnummer_remote;
	
	struct eintraege 
	{
		unsigned char art; 
		unsigned int paketnummer;
		unsigned char checksum;
		unsigned int nutzdatenlaenge;
		char nutzdaten[MAX_NUTZDATENLAENGE];
		unsigned char prio;
		unsigned int wiederholungen; 
	};

	eintraege * puffer[MAX_NUTZDATEN];

	unsigned int find_next_art(unsigned char art);
	void put_to_buffer_rueckscheinantwort(unsigned int paketnummer,unsigned char checksum,unsigned int nutzdatenlaenge);
	void del_from_buffer_rueckschein(unsigned int paketnummer,unsigned char checksum,unsigned int nutzdatenlaenge);
	unsigned char checksum(char * src,unsigned int size);
	void log_status();
	void print_inhalt(char * src,unsigned int size, unsigned short art);

};


#endif