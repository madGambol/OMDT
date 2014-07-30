
#include "CBitfieldDataType.h"

using namespace std;


CBitfieldDataType::CBitfieldDataType()
    :
    CAttributeList( OMDBase::NodeTypeEnum(OPEN_eBitfieldDataType) )
{
    cout << getType() << std::endl;
}

CBitfieldDataType::~CBitfieldDataType()
{
}

const char * CBitfieldDataType::getType(void) const
{
    return "BitfieldDataType";
}

bool CBitfieldDataType::equal(const OMDBase & rhs) const
{
    return OMDNode::equalSameOrder( rhs );
}

bool CBitfieldDataType::notEqual(const OMDBase & rhs) const
{
    return !equal( rhs );
}

void CBitfieldDataType::Indirect( OMDTask * pVisitor) const
{
    pVisitor->Process( this );
}

void CBitfieldDataType::Execute(OMDTask * pVisitor ) const
{
    OMDNode::Execute( pVisitor );
}

