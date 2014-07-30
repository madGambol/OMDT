/* 
 * File:   CComplexComponent.h
 * Author: arena
 *
 * Created on May 22, 2013, 5:52 PM
 */

#ifndef CCOMPLEXCOMPONENT_H
#define	CCOMPLEXCOMPONENT_H

#include <string>
#include "Attribute.h"

class CComplexComponent : public CAttributeList
{
public:

    CComplexComponent();

    virtual ~CComplexComponent();

    virtual const char * getType(void) const;

    virtual std::string  getDataType(void) const;

    virtual std::string  getCardinality(void) const;

    virtual void Execute (OMDTask * pVisitor ) const;
    virtual void Indirect(OMDTask * pTask    ) const;

};



#endif	/* CCOMPLEXCOMPONENT_H */

