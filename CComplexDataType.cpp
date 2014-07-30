
#include "CComplexDataType.h"
using namespace std;

CComplexDataType::CComplexDataType()
    :
    CAttributeList( OMDBase::NodeTypeEnum(OPEN_eComplexDataType) )
{
}

CComplexDataType::~CComplexDataType()
{
}

const char * CComplexDataType::getType(void) const
{
    return "ComplexDataType";
}

bool CComplexDataType::equal(const OMDBase & rhs) const
{
    return OMDNode::equalSameOrder( rhs );
}

bool CComplexDataType::notEqual(const OMDBase & rhs) const
{
    return !equal( rhs );
}

void CComplexDataType::Indirect( OMDTask * pVisitor) const
{
    pVisitor->Process( this );
}

void CComplexDataType::Execute(OMDTask * pVisitor ) const
{
    OMDNode::Execute( pVisitor );
}


