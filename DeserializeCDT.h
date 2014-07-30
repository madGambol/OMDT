/* 
 * File:   DeserializeCDT.h
 * Author: arena
 *
 * Created on May 26, 2013, 1:07 PM
 */

#ifndef DESERIALIZECDT_H
#define	DESERIALIZECDT_H

#include "Deserializer.h"

#include <string>
#include <list>

class DeserializeCDT : public Deserializer
{
    DeserializeCDT(const DeserializeCDT& orig);
    DeserializeCDT & operator=(const DeserializeCDT& orig);
    
public:
    
    DeserializeCDT( const char * pCdtName, const std::list<Deserializer*> & itemList );
    
    virtual ~DeserializeCDT();
    
    std::string getString( const char *& pData, size_t & dataLen) const;
    
private:

} ;

#endif	/* DESERIALIZECDT_H */

