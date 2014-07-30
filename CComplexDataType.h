/* 
 * File:   CComplexDataType.h
 * Author: arena
 *
 * Created on May 22, 2013, 5:50 PM
 */

#ifndef CCOMPLEXDATATYPE_H
#define	CCOMPLEXDATATYPE_H

#include <string>
#include "Attribute.h"

class CComplexDataType : public CAttributeList
{
public:

    CComplexDataType();

    virtual ~CComplexDataType();

    virtual const char * getType(void) const;

    virtual
    bool equal(const OMDBase & rhs) const;

    virtual
    bool notEqual(const OMDBase & rhs) const;

    virtual void Execute (OMDTask * pVisitor ) const;
    virtual void Indirect(OMDTask * pTask    ) const;

};



#endif	/* CCOMPLEXDATATYPE_H */

