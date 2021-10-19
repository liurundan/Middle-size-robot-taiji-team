#ifndef _MessageBoard_h
#define _MessageBoard_h

#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>

class MessageBoard
{
public:
	 
	MessageBoard () throw (std::bad_alloc);

	const std::vector<std::string> get_incoming () const throw ();

	const std::string& scan_for_prefix (const std::string&) const throw ();
	const std::string& scan_for_prefix (const char*) const throw ();
	const std::vector<std::string> get_outgoing () throw ();

	void clear () throw ();

	void publish (const std::string&) throw ();
	void publish (const char*) throw ();
	std::ostream& publish_stream () throw ();

	void receive (const std::string&) throw ();
	void receive (const std::vector<std::string>&) throw ();
	void receive (const char*) throw ();
 
private:
	std::vector<std::string> incoming;
    std::vector<std::string> outgoing;
    std::stringstream outgoing_stream;
    const std::string null_string;
	
};

#define MOUT WorldModel::get_main_world_model().get_message_board().publish_stream()

#endif