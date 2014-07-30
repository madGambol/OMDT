/* 
 * File:   CClass.h
 * Author: arena
 *
 * Created on May 23, 2013, 4:53 PM
 */

#ifndef CCLASS_H
#define	CCLASS_H

#include "Attribute.h"

class CClass : public CAttributeList
{
public:
    
    CClass();

    virtual ~CClass();

    virtual const char * getType(void) const;

    virtual void Execute (OMDTask * pVisitor ) const;
    virtual void Indirect(OMDTask * pTask    ) const;
    
};



#endif	/* CCLASS_H */

