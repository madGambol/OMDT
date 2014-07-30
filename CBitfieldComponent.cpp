

#include "CBitfieldComponent.h"

using namespace std;

CBitfieldComponent::CBitfieldComponent()
    :
    CAttributeList( OMDBase::NodeTypeEnum(OPEN_eBitfieldComponent) )
{
}

CBitfieldComponent::~CBitfieldComponent()
{
}

const char * CBitfieldComponent::getType(void) const
{
    return "BitfieldComponent";
}

void CBitfieldComponent::Indirect( OMDTask * pVisitor) const
{
    pVisitor->Process( this );
}

void CBitfieldComponent::Execute(OMDTask * pVisitor ) const
{
    OMDNode::Execute( pVisitor );
}
