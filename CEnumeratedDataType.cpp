/* 
 * File:   CEnumeratedDataType.cpp
 * Author: arena
 * 
 * Created on May 23, 2013, 4:44 PM
 */

#include "CEnumeratedDataType.h"

using namespace std;

CEnumeratedDataType::CEnumeratedDataType()
    :
    OMDNode( OMDBase::NodeTypeEnum(OPEN_eEnumeratedDataType) )
{
}

CEnumeratedDataType::~CEnumeratedDataType()
{
}

const char * CEnumeratedDataType::getType(void) const
{
    return "EnumeratedDataType";
}

void CEnumeratedDataType::Indirect( OMDTask * pVisitor) const
{
    pVisitor->Process( this );
}

void CEnumeratedDataType::Execute(OMDTask * pVisitor ) const
{
    OMDNode::Execute( pVisitor );
}

void   CEnumeratedDataType::setName( const char * pName )
{
    if (pName) mName = pName;
}

string CEnumeratedDataType::getName(void) const
{
    return mName;
}

