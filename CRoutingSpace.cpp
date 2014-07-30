
#include "Attribute.h"
#include "CRoutingSpace.h"

using namespace std;

CRoutingSpace::CRoutingSpace()
    :
    CAttributeList( OMDBase::NodeTypeEnum(OPEN_eRoutingSpace) )
{
}

CRoutingSpace::~CRoutingSpace()
{
}

const char * CRoutingSpace::getType(void) const
{
    return "RoutingSpace";
}

bool CRoutingSpace::equal(const OMDBase & rhs) const
{
    return equalSameOrder( rhs );
}

bool CRoutingSpace::notEqual(const OMDBase & rhs) const
{
    return !equalSameOrder( rhs );
}

void CRoutingSpace::Indirect( OMDTask * pVisitor) const
{
    pVisitor->Process( this );
}

void CRoutingSpace::Execute(OMDTask * pVisitor ) const
{
    OMDNode::Execute( pVisitor );
}

