/* 
 * File:   CEnumeration.h
 * Author: arena
 *
 * Created on May 23, 2013, 4:41 PM
 */

#ifndef CENUMERATION_H
#define	CENUMERATION_H

#include "Attribute.h"

class CEnumeration : public CAttributeList 
{
public:
    
    CEnumeration();

    virtual ~CEnumeration();

    virtual const char * getType(void) const;

    virtual void Execute (OMDTask * pVisitor ) const;
    virtual void Indirect(OMDTask * pTask    ) const;
    
};


#endif	/* CENUMERATION_H */

