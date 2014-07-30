/* 
 * File:   CBitfieldDataType.h
 * Author: arena
 *
 * Created on May 22, 2013, 5:53 PM
 */

#ifndef CBITFIELDDATATYPE_H
#define	CBITFIELDDATATYPE_H

#include <string>
#include "Attribute.h"

class CBitfieldDataType : public CAttributeList
{
public:

    CBitfieldDataType();

    virtual ~CBitfieldDataType();

    virtual const char * getType(void) const;

    virtual
    bool equal(const OMDBase & rhs) const;

    virtual
    bool notEqual(const OMDBase & rhs) const;

    virtual void Execute (OMDTask * pVisitor ) const;
    virtual void Indirect(OMDTask * pTask    ) const;

};



#endif	/* CBITFIELDDATATYPE_H */

