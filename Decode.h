/* 
 * File:   Decode.h
 * Author: arena
 *
 * Created on May 26, 2013, 12:15 PM
 */

#ifndef DECODE_H
#define	DECODE_H

#include <string>

class Decode
{
    Decode(const Decode& orig);
    Decode & operator=(const Decode& orig);
    
public:
    
    virtual ~Decode();
    
    virtual
    std::string getString( const char * pClassName, 
                           const char * pAttributeName, 
                           const char * pData, 
                           size_t dataLen 
                         ) = 0;
    
    static
    Decode * factory( const char * pOMDFileName);
};

#endif	/* DECODE_H */

