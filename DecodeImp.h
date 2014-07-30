/* 
 * File:   DecodeImp.h
 * Author: arena
 *
 * Created on May 26, 2013, 12:53 PM
 */

#ifndef DECODEIMP_H
#define	DECODEIMP_H

#include "Decode.h"

class DecodeImp : public Decode
{
    DecodeImp(const DecodeImp& orig);
    DecodeImp & operator=(const DecodeImp& orig);
    
public:
    
    DecodeImp();
    virtual ~DecodeImp();
    
    
    
private:

} ;

#endif	/* DECODEIMP_H */

