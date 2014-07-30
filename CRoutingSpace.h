/* 
 * File:   CRoutingSpace.h
 * Author: arena
 *
 * Created on May 22, 2013, 5:48 PM
 */

#ifndef CROUTINGSPACE_H
#define	CROUTINGSPACE_H

#include <string>
#include "Attribute.h"

class CRoutingSpace : public CAttributeList
{
public:
    CRoutingSpace();

    virtual ~CRoutingSpace();

    virtual const char * getType(void) const;

    virtual
    bool equal(const OMDBase & rhs) const;

    virtual
    bool notEqual(const OMDBase & rhs) const;

    virtual void Execute (OMDTask * pVisitor ) const;
    virtual void Indirect(OMDTask * pTask    ) const;
};



#endif	/* CROUTINGSPACE_H */

