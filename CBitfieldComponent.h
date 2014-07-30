/* 
 * File:   CBitfieldComponent.h
 * Author: arena
 *
 * Created on May 22, 2013, 5:55 PM
 */

#ifndef CBITFIELDCOMPONENT_H
#define	CBITFIELDCOMPONENT_H

#include <string>
#include "Attribute.h"

class CBitfieldComponent : public CAttributeList
{
public:

    CBitfieldComponent();

    virtual ~CBitfieldComponent();

    virtual const char * getType(void) const;

    virtual void Execute (OMDTask * pVisitor ) const;
    virtual void Indirect(OMDTask * pTask    ) const;
};



#endif	/* CBITFIELDCOMPONENT_H */

