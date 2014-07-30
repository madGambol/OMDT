
#include "CDimension.h"

using namespace std;

CDimension::CDimension()
    :
    CAttributeList( OMDBase::NodeTypeEnum(OPEN_eDimension) )
{
}

CDimension::~CDimension()
{
}

const char * CDimension::getType(void) const
{
    return "Dimension";
}

void CDimension::Indirect( OMDTask * pVisitor) const
{
    pVisitor->Process( this );
}

void CDimension::Execute(OMDTask * pVisitor ) const
{
    OMDNode::Execute( pVisitor );
}

