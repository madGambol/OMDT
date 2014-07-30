
#include "CComplexComponent.h"

using namespace std;

CComplexComponent::CComplexComponent()
    :
    CAttributeList( OMDBase::NodeTypeEnum(OPEN_eComplexComponent) )
{
}

CComplexComponent::~CComplexComponent()
{
}

const char * CComplexComponent::getType(void) const
{
    return "ComplexComponent";
}

string CComplexComponent::getDataType(void) const
{
    const OMDBase * pBase = getKind("DataType");

    const CAttributeValuePair * pAVP = dynamic_cast<const CAttributeValuePair *>( pBase );
    
    string retVal = "";

    if (pAVP)
    {
	retVal = pAVP->getValue();
    }

    return retVal;
}

string CComplexComponent::getCardinality(void) const
{
    const OMDBase * pBase = getKind("Cardinality");

    const CAttributeValuePair * pAVP = dynamic_cast<const CAttributeValuePair *>( pBase );
    
    string retVal = "";

    if (pAVP)
    {
	retVal = pAVP->getValue();
    }

    return retVal;
}

void CComplexComponent::Indirect( OMDTask * pVisitor) const
{
    pVisitor->Process( this );
}

void CComplexComponent::Execute(OMDTask * pVisitor ) const
{
    OMDNode::Execute( pVisitor );
}


