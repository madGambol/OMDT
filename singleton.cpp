#include <iostream>
#include <string>
#include <ostream>
#include <sstream>
#include <vector>
#include <list>
#include <map>

using namespace std;

#include "OMDTask.h"
#include "singleton.h"

using namespace std;

Singleton::Singleton(const char * pStr)
  :
  OMDBase( OMDBase::NodeTypeEnum(OPEN_eDIF) ), 
  m_Value( pStr )
{
}

Singleton::~Singleton() 
{
}

const std::string Singleton::getString(void) const
{
  return m_Value+string(" ");
}

const std::string Singleton::getValue(void) const
{
  return m_Value;
}


const char * Singleton::getType(void) const
{
  return "Singleton";
}

void Singleton::Indirect(OMDTask * pVisitor ) const
{
  pVisitor->Process( this );
}

void Singleton::Execute(OMDTask * pTask) const
{
}

  
bool Singleton::equal(const OMDBase & rhs) const
{
  const Singleton * pRHS = dynamic_cast<const Singleton *>( &rhs );

  if (pRHS == NULL) return false;

  return m_Value == pRHS->m_Value;
}

bool Singleton::notEqual(const OMDBase & rhs) const
{
  return !equal( rhs );
}
