
#include <sstream>

#include "eprint.hh"

using namespace std;

std::stringstream & operator<<( std::stringstream & out, const EPrint & item)
{
    stringstream str;
    
    item.eprint( str );
    
    out << str.str();
    
    return out;
}

