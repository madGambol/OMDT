/* 
 * File:   CObjectModel.h
 * Author: arena
 *
 * Created on May 22, 2013, 5:46 PM
 */

#ifndef COBJECTMODEL_H
#define	COBJECTMODEL_H

#include <string>
#include "Attribute.h"

//
// The CObjectModel class will order its children:
//
// ObjectModel parameters (Name, VersionNumber, Type, Purpose, etc...)
// RoutingSpace
// EnumeratedDataTypes
// ComplexDataTypes     (in dependency order)
// Objects
// Interactions
// Notes
//
class CObjectModel : public CAttributeList
{
public:
    CObjectModel();

    virtual ~CObjectModel();

    virtual const char * getType(void) const;

    virtual void Execute (OMDTask * pVisitor ) const;
    virtual void Indirect(OMDTask * pTask    ) const;
};



#endif	/* COBJECTMODEL_H */

