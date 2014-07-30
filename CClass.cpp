/* 
 * File:   CClass.cpp
 * Author: arena
 * 
 * Created on May 23, 2013, 4:53 PM
 */

#include "CClass.h"

using namespace std;

CClass::CClass()
    :
    CAttributeList( OMDBase::NodeTypeEnum(OPEN_eClass) )
{
}

CClass::~CClass()
{
}

const char * CClass::getType(void) const
{
    return "Class";
}

void CClass::Indirect(OMDTask * pVisitor ) const
{
    pVisitor->Process( this );
}

void CClass::Execute(OMDTask * pVisitor ) const
{
    OMDNode::Execute( pVisitor );
}

