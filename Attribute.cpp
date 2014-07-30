
#include <iostream>
#include <string>
#include <ostream>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <set>

using namespace std;

#include <FlexLexer.h>
#include "omdtLex.h"

class CNoteText;

#include "OMDBase.h"
#include "OMDNode.h"
#include "OMDTask.h"

#include "Attribute.h"

int yyparse(void);

const char * tokenLookup(int token);

string stripQuotes( const char * pStr );

CAttributeName::CAttributeName( const char * pName)
    :
    OMDBase   ( OMDBase::NodeTypeEnum( eAttributeName )),
    m_AttrName( pName )
{
}

CAttributeName::~CAttributeName()
{
}

bool CAttributeName::equal(const OMDBase & rhs) const
{
    const CAttributeName * pRHS = dynamic_cast<const CAttributeName *>( &rhs );

    if (pRHS == NULL) return false;

    return m_AttrName == pRHS->m_AttrName;
}

bool CAttributeName::notEqual(const OMDBase & rhs) const
{
    return !equal( rhs );
}

const string CAttributeName::getString(void) const
{
    return m_AttrName;
}

const char * CAttributeName::getType(void) const
{
    return "AttributeName";
}

const string CAttributeName::getName(void) const
{
    return m_AttrName;
}

void CAttributeName::Indirect( OMDTask * pVisitor) const
{
    pVisitor->Process( this );
}

void CAttributeName::Execute( OMDTask * pVisitor) const
{
}

CAttributeValue::CAttributeValue( const char * pName)
    :
    OMDBase   ( OMDBase::NodeTypeEnum( eAttributeValue )),
    m_AttrValue( pName )
{
}

CAttributeValue::CAttributeValue( const string & rName)
:
    OMDBase    ( OMDBase::NodeTypeEnum( eAttributeValue )),
    m_AttrValue( rName )
{
}


CAttributeValue::~CAttributeValue()
{
}

const string CAttributeValue::getString(void) const
{
    return m_AttrValue;
}

const char * CAttributeValue::getType(void) const
{
    return "AttributeValue";
}

const string CAttributeValue::getValue(void) const
{
    return m_AttrValue;
}

void CAttributeValue::Indirect( OMDTask * pVisitor) const
{
    pVisitor->Process( this );
}

void CAttributeValue::Execute( OMDTask * pVisitor) const
{
}

bool CAttributeValue::equal(const OMDBase & rhs) const
{
    const CAttributeValue * pRHS = dynamic_cast<const CAttributeValue *>( &rhs );

    if (pRHS == NULL) return false;

    return m_AttrValue == pRHS->m_AttrValue;
}

bool CAttributeValue::notEqual(const OMDBase & rhs) const
{
    return !equal( rhs );
}

CAttributeValuePair::CAttributeValuePair(OMDBase::NodeTypeEnum kind, 
                                         const char * pVal, 
                                         const Loc  * pLoc)
    :
    OMDNode( kind, pLoc  ),
    m_Attr ( tokenLookup( kind ) ),
    m_Value( stripQuotes( pVal ) )
{
    //cerr << "CAttributeValuePair::CAttributeValuePair( ..., " << pVal << ")" << endl;
}

CAttributeValuePair::~CAttributeValuePair()
{
}

string CAttributeValuePair::getName(void) const
{
    return m_Attr.getName();
}

string CAttributeValuePair::getValue(void) const
{
    return m_Value.getValue();
}

const char * CAttributeValuePair::getType(void) const
{
    return "AttributeValuePair";
}

const string CAttributeValuePair::getString(void) const
{
    stringstream retVal;

    retVal << "("
           << getName()
           << " "
           << getValue()
           << OMDNode::getString()
           << ")";

    return retVal.str();
}

bool CAttributeValuePair::equal(const OMDBase & rhs) const
{
    const CAttributeValuePair * pRHS = dynamic_cast<const CAttributeValuePair *>( &rhs );

    if (!pRHS) return false;

    do
    {
        if (!m_Attr. equal(pRHS->m_Attr )) break;

        if (!m_Value.equal(pRHS->m_Value)) break;

        if (!OMDNode::equal( rhs )) break;

        return true;

    } while (0);

    return false;
}

bool CAttributeValuePair::notEqual(const OMDBase & rhs) const
{
    return !equal(rhs);
}

void CAttributeValuePair::Indirect( OMDTask * pVisitor) const
{
    pVisitor->Process( this );
}

void CAttributeValuePair::Execute( OMDTask * pVisitor ) const
{
    pVisitor->Process( &m_Attr );
    pVisitor->Process( &m_Value);

    OMDNode::Execute( pVisitor );
}

OMDBase * OMDNode::getChild(const string     & kind)
{
    size_t index;
    for (index = 0; index < m_Children.size(); index++)
    {
        OMDBase * pTemp = m_Children[ index ];

        CAttributeValuePair * pAVP = dynamic_cast<CAttributeValuePair *>( pTemp );

        if (!pAVP) continue;

        string avpName = pAVP->getName();

        cerr << "Comparing \"" << avpName << "\" with \"" << kind << "\"" << endl;

        if (avpName == kind) 
        {
            return (OMDBase *)pAVP;
        }
    }
  
    return NULL;  // not found
}

const OMDBase * OMDNode::getChild(const string     & kind) const
{
    size_t index;
    for (index = 0; index < m_Children.size(); index++)
    {
        OMDBase * pTemp = m_Children[ index ];

        const CAttributeValuePair * pAVP = dynamic_cast<const CAttributeValuePair *>( pTemp );

        if (!pAVP) continue;

        if (pAVP->getName() == kind) 
        {
            return (const OMDBase *)pAVP;
        }
    }
  
    return NULL;  // not found
}

CAttributeList::CAttributeList(OMDBase::NodeTypeEnum kind, const Loc * pLoc)
    :
    OMDNode( kind, pLoc )
    // ,m_Map ()
{
}

CAttributeList::~CAttributeList()
{
}

const string CAttributeList::getString() const
{
    stringstream retVal;

    retVal << "(";

    string nodeName( tokenLookup( getNodeType() ) );

    retVal << nodeName
           << OMDNode::getString()
           << ") ;; "
           << nodeName
           << "\n";

    return retVal.str();
}

const char * CAttributeList::getType(void) const
{
    return "OMDNode";
}

std::string CAttributeList::getDataType(void) const
{
    string retVal;
    
    // get first element

    do
    {
    const OMDBase * pBase = getChild(1);

    const CAttributeValuePair * pAVP = dynamic_cast<const CAttributeValuePair *>( pBase );

    if (pAVP)
    {
        string name  = pAVP->getName();
        string value = pAVP->getValue();

        if ((name == "DataType") )
        {
            retVal = value;
        }
    }
    } while(0);

    return retVal;
}

const string CAttributeList::getName(void) const  // name of EDT, CDT, CC
{
    string retVal;
    
    // get first element

    const OMDBase * pBase = getChild(0);

    const CAttributeValuePair * pAVP = dynamic_cast<const CAttributeValuePair *>( pBase );

    if (pAVP)
    {
        string name  = pAVP->getName();
        string value = pAVP->getValue();

        if ((name == "Name") || (name == "FieldName"))
        {
            retVal = value;
        }
    }

    return retVal;
}

size_t CAttributeList::size(void) const
{
    return 0; // m_Map.size();
}

// void CAttributeList::Add(const CAttributeValuePair * pAVP)
// {
//   if (!pAVP) return;

//   push_back( pAVP );
// }

// void CAttributeList::Add(const CAttributeList * pAL)
// {
//   if (!pAL) return;

//   const_iterator item = pAL->begin();
//   const_iterator last = pAL->end();

//   while (item != last)
//   {
//     const CAttributeValuePair * pItem = *item;

//     push_back( pItem );

//     ++item;
//   }
// }

const OMDBase * CAttributeList::getKind   (const string & kind) const
{
    return getChild( kind );
}

const OMDBase * CAttributeList::operator[](const string & kind) const
{
    return getKind(kind);
}

void CAttributeList::Indirect( OMDTask * pVisitor) const
{
    pVisitor->Process( this );
}

void CAttributeList::Execute( OMDTask * pVisitor ) const
{
    OMDNode::Execute( pVisitor );
}

//////////////////////////////////////////////////////////////////////////
//////////////// Implementation of the specific //////////////////////////
//////////////// classes in the omd             //////////////////////////
//////////////////////////////////////////////////////////////////////////







CAttribute::CAttribute()
    :
    CAttributeList( OMDBase::NodeTypeEnum(OPEN_eAttribute) )
{
}

CAttribute::~CAttribute()
{
}

const char * CAttribute::getType(void) const
{
    return "Attribute";
}

void CAttribute::Indirect(OMDTask * pVisitor ) const
{
    pVisitor->Process( this );
}

void CAttribute::Execute(OMDTask * pVisitor ) const
{
    OMDNode::Execute( pVisitor );
}


CInteraction::CInteraction()
    :
    CAttributeList( OMDBase::NodeTypeEnum(OPEN_eInteraction) )
{
}

CInteraction::~CInteraction()
{
}

const char * CInteraction::getType(void) const
{
    return "Interaction";
}

void CInteraction::Indirect(OMDTask * pVisitor ) const
{
    pVisitor->Process( this );
}

void CInteraction::Execute(OMDTask * pVisitor ) const
{
    OMDNode::Execute( pVisitor );
}

CParameter::CParameter()
    :
    CAttributeList( OMDBase::NodeTypeEnum(OPEN_eParameter) )
{
}

CParameter::~CParameter()
{
}

const char * CParameter::getType(void) const
{
    return "Parameter";
}

void CParameter::Indirect(OMDTask * pVisitor ) const
{
    pVisitor->Process( this );
}

void CParameter::Execute(OMDTask * pVisitor ) const
{
    OMDNode::Execute( pVisitor );
}


CNote::CNote(const Loc * pLoc)
    :
    CAttributeList( OMDBase::NodeTypeEnum(OPEN_eNote), pLoc )
{
}

CNote::~CNote()
{
}

const char * CNote::getType(void) const
{
    return "Note";
}

void CNote::Indirect(OMDTask * pVisitor ) const
{
    pVisitor->Process( this );
}

void CNote::Execute(OMDTask * pVisitor ) const
{
    OMDNode::Execute( pVisitor );
}

CNoteText::CNoteText( const char * pVal, const Loc * pLoc )
:
    CAttributeValuePair( OMDNode::NodeTypeEnum(OPEN_eNoteText),
                         pVal,
                         pLoc
                       )
{
    //
}

CNoteText::~CNoteText(void)
{
    //
}

const char * CNoteText::getType(void) const
{
    return "NoteText";
}

void CNoteText::Execute ( OMDTask * pVisitor ) const
{
    CAttributeValuePair::Execute( pVisitor );
}

void CNoteText::Indirect( OMDTask * pTask    ) const
{
    pTask->Process( this );
}

CNoteList::CNoteList(int number)
    :
    OMDNode( OMDNode::NodeTypeEnum(OPEN_eNote) )
{
    mNoteRefs.insert( number );
}

CNoteList::~CNoteList()
{
}

void CNoteList::Indirect(OMDTask * pVisitor ) const
{
    pVisitor->Process( this );
}

void CNoteList::Execute(OMDTask * pVisitor ) const
{
    OMDNode::Execute( pVisitor );
}

const char * CNoteList::getType(void) const
{
    return "NoteList";
}


string IntToStr(size_t input);

set<size_t> CNoteList::getNoteSet(void) const
{
    return mNoteRefs;
}

const string CNoteList::getString(void) const
{
    stringstream retVal;

    set<size_t>::const_iterator item = mNoteRefs.begin();
    set<size_t>::const_iterator last = mNoteRefs.end();

    if (item != last)
    {
        retVal << "["
               << *item;

        ++item;

        while (item != last)
        {
            retVal << ", " << *item;
        
            ++item;
        }
      
        retVal << "]";
    }

    return retVal.str();
}

void CNoteList::Add( const CNoteList * pNL )
{
    if (!pNL) return;
    
    mNoteRefs.insert(pNL->mNoteRefs.begin(), pNL->mNoteRefs.end() );
}

void CNoteList::Add( size_t number )
{
    mNoteRefs.insert( number );
}
