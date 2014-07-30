/* 
 * File:   CEnumeratedDataType.h
 * Author: arena
 *
 * Created on May 23, 2013, 4:44 PM
 */

#ifndef CENUMERATEDDATATYPE_H
#define	CENUMERATEDDATATYPE_H

#include "Attribute.h"

class CEnumeratedDataType : public OMDNode
{
    std::string mName;

public:
    
    CEnumeratedDataType();

    virtual ~CEnumeratedDataType();

    virtual const char * getType(void) const;

    virtual void Execute (OMDTask * pVisitor ) const;
    virtual void Indirect(OMDTask * pTask    ) const;

    void   setName( const char * pName );
    std::string getName(void) const;
    
};



#endif	/* CENUMERATEDDATATYPE_H */

