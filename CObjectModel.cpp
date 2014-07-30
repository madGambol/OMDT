

#include "Attribute.h"
#include "CObjectModel.h"

using namespace std;

CObjectModel::CObjectModel()
    :
    CAttributeList( OMDBase::NodeTypeEnum(OPEN_eObjectModel) )
{
}

CObjectModel::~CObjectModel()
{
}

const char * CObjectModel::getType(void) const
{
    return "ObjectModel";
}

void CObjectModel::Indirect( OMDTask * pVisitor) const
{
    pVisitor->Process( this );
}

void CObjectModel::Execute(OMDTask * pVisitor ) const
{
    OMDNode::Execute( pVisitor );
}

