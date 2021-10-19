#ifndef _ConfigReader_h_
#define _ConfigReader_h_

#include <deque>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <stack>

class ConfigReader
{
private:
	struct KVPair;
    std::deque<KVPair> key_value_table;         
    const unsigned int verbosity;               
    const char comment_char;                    
    const char assign_char;                     
    std::stack<std::string> directory_stack;    
    std::deque<std::string> files;              
    std::string home_path;                     

    bool find_first (std::string& value, const std::string& key) const;   
    bool find_all (std::deque<std::string>& values, const std::string& key) const; 
public:
 
    ConfigReader (unsigned int=0, char ='#', char ='=') throw ();
     ~ConfigReader () throw ();
 
     bool append_from_stream (std::istream&, bool =true) throw (std::bad_alloc);
 
    bool append_from_file (const char*, bool=true) throw (std::bad_alloc);

     const std::deque<std::string>& list_of_sources () const throw ();
 
    int get (const char*, int&) const throw (std::bad_alloc);                  ///< int lesen
    int get (const char*, unsigned int&) const throw (std::bad_alloc);         ///< unsigned int lesen
    int get (const char*, long int&) const throw (std::bad_alloc);             ///< long int lesen
    int get (const char*, unsigned long int&) const throw (std::bad_alloc);    ///< unsigned long int lesen
    int get (const char*, double&) const throw (std::bad_alloc);               ///< double lesen
    int get (const char*, float&) const throw (std::bad_alloc);                ///< double lesen
    int get (const char*, char&) const throw (std::bad_alloc);                 ///< char lesen
    int get (const char*, bool&) const throw (std::bad_alloc);                 ///< bool lesen (1/0/true/false)
    int get (const char*, std::string&) const throw (std::bad_alloc);          ///< string (ein Wort) lesen

    int get (const char*, std::vector<int>&) const throw (std::bad_alloc);
    int get (const char*, std::vector<unsigned int>&) const throw (std::bad_alloc);
    int get (const char*, std::vector<long int>&) const throw (std::bad_alloc);
    int get (const char*, std::vector<unsigned long int>&) const throw (std::bad_alloc);
    int get (const char*, std::vector<double>&) const throw (std::bad_alloc);
    int get (const char*, std::vector<float>&) const throw (std::bad_alloc);
    int get (const char*, std::vector<char>&) const throw (std::bad_alloc);
    int get (const char*, std::vector<bool>&) const throw (std::bad_alloc);
    int get (const char*, std::vector<std::string>&) const throw (std::bad_alloc);

    void set (const char*, int) throw (std::bad_alloc);                   
    void set (const char*, unsigned int) throw (std::bad_alloc);          
    void set (const char*, long int) throw (std::bad_alloc);             
    void set (const char*, unsigned long int) throw (std::bad_alloc);     
    void set (const char*, double) throw (std::bad_alloc);               
    void set (const char*, float) throw (std::bad_alloc);                
    void set (const char*, char) throw (std::bad_alloc);                 
    void set (const char*, bool) throw (std::bad_alloc);                  
    void set (const char*, const std::string&) throw (std::bad_alloc);    
    void set (const char*, const char*) throw (std::bad_alloc);           

    void set (const char*, const std::vector<int>&) throw (std::bad_alloc);
    void set (const char*, const std::vector<unsigned int>&) throw (std::bad_alloc);
    void set (const char*, const std::vector<long int>&) throw (std::bad_alloc);
    void set (const char*, const std::vector<unsigned long int>&) throw (std::bad_alloc);
    void set (const char*, const std::vector<double>&) throw (std::bad_alloc);
    void set (const char*, const std::vector<float>&) throw (std::bad_alloc);
    void set (const char*, const std::vector<char>&) throw (std::bad_alloc);
    void set (const char*, const std::vector<bool>&) throw (std::bad_alloc);
    void set (const char*, const std::vector<std::string>&) throw (std::bad_alloc);

    bool write (std::ostream&, const char*) const throw ();

    
    bool ConfigReader::replace_config_stream (std::ostream&, std::istream&, bool =true) throw (std::bad_alloc);
     
    bool ConfigReader::replace_config_file (const char*, bool =true) throw (std::bad_alloc);
protected:

};
#endif

