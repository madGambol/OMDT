

#ifndef _CPrint_h
#define _CPrint_h

#include <iostream>
#include <sstream>

class CPrint
{
public:

    CPrint();

    virtual ~CPrint();

    virtual std::ostream & print( std::ostream & out ) const = 0;  

};

std::stringstream & operator<<(std::stringstream & out, const CPrint & item);
std::ostream      & operator<<(std::ostream      & out, const CPrint & item);

#endif
// Local Variables:
// mode:C++
// End:
