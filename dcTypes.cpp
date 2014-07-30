
#include <iostream>
#include <string>
#include <ostream>
#include <sstream>
#include <vector>
#include <list>
#include <map>

using namespace std;

#include "OMDBase.h"
#include "dcTypes.h"


CTypeDescr::CTypeDescr ( const char * pName, long size )
  :
  mName    ( pName ),
  mSize    ( size  ),
  mpOMDBase( NULL  )
{
}

void CTypeDescr::setNode( const OMDBase * pOMDBase )
{
  mpOMDBase = pOMDBase;
}

const OMDBase * CTypeDescr::getNode(void) const
{
  return mpOMDBase;
}

bool CTypeDescr::isSimple(void) const
{
  return false;
}

bool CTypeDescr::isEnumeration(void) const
{
  return false;
}

bool CTypeDescr::isComplex(void) const
{
  return false;
}


CSimpleType::CSimpleType(const char * pName, long size)
  :
  CTypeDescr( pName, size )
{
}

bool CSimpleType::isSimple(void) const
{
  return true;
}


CComplexType::CComplexType(const char * pName, long size)
  :
  CTypeDescr( pName, size )
{
}

bool CComplexType::isComplex(void) const
{
  return true;
}


CEnumerationType::CEnumerationType(const char * pName, long size)
  :
  CTypeDescr( pName, size )
{
}

bool CEnumerationType::isEnumeration(void) const
{
  return true;
}

