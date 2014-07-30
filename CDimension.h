/* 
 * File:   CDimension.h
 * Author: arena
 *
 * Created on May 22, 2013, 5:49 PM
 */

#ifndef CDIMENSION_H
#define	CDIMENSION_H

#include <string>
#include "Attribute.h"

class CDimension : public CAttributeList
{
public:
    CDimension();

    virtual ~CDimension();

    virtual const char * getType(void) const;

    virtual void Execute (OMDTask * pVisitor ) const;
    virtual void Indirect(OMDTask * pTask    ) const;
};



#endif	/* CDIMENSION_H */

