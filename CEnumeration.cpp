/* 
 * File:   CEnumeration.cpp
 * Author: arena
 * 
 * Created on May 23, 2013, 4:41 PM
 */

#include "CEnumeration.h"

using namespace std;

CEnumeration::CEnumeration()
    :
    CAttributeList( OMDBase::NodeTypeEnum(OPEN_eEnumeration) )
{
}

CEnumeration::~CEnumeration()
{
}

const char * CEnumeration::getType(void) const
{
    return "Enumeration";
}

void CEnumeration::Indirect(OMDTask * pVisitor ) const
{
    pVisitor->Process( this );
}

void CEnumeration::Execute(OMDTask * pVisitor ) const
{
    OMDNode::Execute( pVisitor );
}

