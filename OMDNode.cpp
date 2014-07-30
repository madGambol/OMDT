
#include <ostream>
#include <sstream>
#include <iostream>

using namespace std;

#include "OMDNode.h"

const char * tokenLookup(int token);

void setOMDBase  ( long omdBase );
bool checkOMDBase( long omdBase );

OMDNode::OMDNode(OMDBase::NodeTypeEnum nodeType, const Loc * pLoc)
  :
  OMDBase( nodeType, pLoc )
{
}

OMDNode::~OMDNode()
{
  while (m_Children.size())
  {
    OMDBase * pOMDBase = m_Children.back();

    m_Children.pop_back();

    delete pOMDBase;
  }
}

ostream & OMDNode::print(ostream & out) const
{
    stringstream str;

    // TBS

    out << str.str();

    return out;
}

const std::vector<OMDBase *> & OMDNode::getChildren() const
{
  return m_Children;
}

const string OMDNode::getString(void) const
{
  string retVal(""); // string("(") + nodeName + string(" ");

  size_t index;

  for (index = 0; index < m_Children.size(); index++)
  {
    OMDBase * pOMDBase = m_Children[ index ];

    retVal += string(" ") + pOMDBase->getString(); // + string("\n");
  }

  return retVal;
}

bool OMDNode::equal(const OMDBase & rhs) const
{
  return equalAnyOrder( rhs );
}

bool OMDNode::equalAnyOrder(const OMDBase & rhs) const
{
  bool bRetVal = false;

  const OMDNode * pRHS = dynamic_cast<const OMDNode *>( &rhs );

  do
  {
    if (!pRHS) break;

    const OMDNode & rRHS = dynamic_cast<const OMDNode &>( rhs );

    if (m_Children.size() != rRHS.m_Children.size()) break;

    size_t index;

    bRetVal = true;
  
    for (index = 0; index < m_Children.size(); index++)
    {
      const OMDBase * pOMDBase = m_Children[ index ];

      size_t innerIndex;

      bool bMatched = false;

      for (innerIndex=0; innerIndex < m_Children.size(); innerIndex++)
      {
	if (pOMDBase->equal( *rRHS.m_Children[ innerIndex ]) )
	{
	  bMatched = true; 
	  break;
	}
      }

      if ( !bMatched ) return false;
    }

  } while (0);

  return bRetVal;
}

bool OMDNode::equalSameOrder(const OMDBase & rhs) const
{
  bool bRetVal = false;

  const OMDNode * pRHS = dynamic_cast<const OMDNode *>( &rhs );

  do
  {
    if (!pRHS) break;

    const OMDNode & rRHS = dynamic_cast<const OMDNode &>( rhs );

    if (m_Children.size() != rRHS.m_Children.size()) break;

    size_t index;

    bRetVal = true;
  
    for (index = 0; index < m_Children.size(); index++)
    {
      const OMDBase * pOMDBase = m_Children[ index ];

      if (!pOMDBase->equal( *rRHS.m_Children[ index ]) ) return false;
    }

  } while (0);

  return bRetVal;
}

bool OMDNode::notEqual(const OMDBase & rhs) const
{
  return !equal( rhs );
}

void OMDNode::Execute( OMDTask * pTask ) const // OMDTask is the abstract visitor class
{
  size_t index;
  for (index = 0; index < m_Children.size(); index++)
  {
    //cout << "Node Type: " << m_Children[ index ]->getType() << endl;

    const OMDBase * pTemp = m_Children[ index ];

    pTemp->Indirect( pTask );

    //pTask->Process( m_Children[ index ] );
  }
}

OMDBase * OMDNode::getChild(size_t index) const
{
  OMDBase * pRetVal = NULL;

  do
  {
    if ( index >= m_Children.size()) break;

    pRetVal = m_Children[index];

  } while (0);

  return pRetVal;
}

//
// Get the child with attribute kind (such as DataType)
// returns the OMDBase pointer to the object or NULL
// if not found.
//
// OMDBase * OMDNode::getChild(const std::string     & kind)
// {
//   size_t index;
//   for (index = 0; index < m_Children.size(); index++)
//   {
//     const OMDBase * pTemp = m_Children[ index ];

//     CAttributeValuePair * pAVP = dynamic_cast<CAttributeValuePair *>( pTemp );

//     if (!pAVP) continue;

//     if (pAVP->getName() == kind) 
//     {
//       return (OMDBase *)pAVP;
//     }
//   }
  
//   return NULL;  // not found
// }

OMDBase * OMDNode::getChild(OMDBase::NodeTypeEnum   kind)
{
  return NULL;  // not implemented
}

size_t OMDNode::getChildCount(void) const
{
  return m_Children.size();
}

void OMDNode::addChild(OMDBase * pChild)
{
  //
  // Don't add NULL children
  //
  if (!pChild) return;

  //
  // Can't add ourselves as a child
  //
  if (pChild == this) return;

  //
  // If the child already has a parent, don't add
  //
  if (pChild->getParent()) return;

  //
  // Set up this as parent
  //
  pChild->setParent( this );

  //
  // add the child
  //
  m_Children.push_back( pChild );
}

void OMDNode::Add( OMDBase * pChild)
{
  addChild( pChild );
}

const char * OMDNode::getType(void) const
{
  return "OMDNode";
}
