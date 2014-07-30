
#ifndef _singleton_h
#define _singleton_h


#include <string>

#ifndef  _OMDBase_h
#include "OMDBase.h"
#endif

class OMDTask;

class Singleton : public OMDBase
{
  std::string m_Value;

 public:

  Singleton(const char * pStr);

  virtual ~Singleton();

  const std::string getString(void) const;

  const char * getType(void) const;

  const std::string getValue(void) const;

  virtual
  bool equal(const OMDBase & rhs) const;

  virtual
  bool notEqual(const OMDBase & rhs) const;

  void Execute (OMDTask * pTask    ) const;
  void Indirect(OMDTask * pVisitor ) const;
 
};
#endif

// Local Variables:
// mode:C++
// End:
