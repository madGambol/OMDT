/* 
 * File:   Deserializer.h
 * Author: arena
 *
 * Created on May 26, 2013, 1:01 PM
 */

#ifndef DESERIALIZER_H
#define	DESERIALIZER_H

#include <string>

class Deserializer
{
    Deserializer(const Deserializer& orig);
    Deserializer & operator=(const Deserializer& orig);
public:
    virtual ~Deserializer();
    
    virtual
    std::string getString( const char *& pData, size_t & dataLen) const = 0;
};

template <class T>
class DeserializeType : public Deserializer<T>
{
public:

    std::string getString( const char *& pData, size_t & dataLen) const;

};

class DeserializeCDT : public Deserializer
{
    
};

#endif	/* DESERIALIZER_H */

