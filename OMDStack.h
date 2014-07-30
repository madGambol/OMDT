
#ifndef _OMDStack_h
#define _OMDStack_h

#include "OMDBase.h"
#include "OMDNode.h"

#include <stack>

class OMDStack : public std::stack<OMDBase *>
{
  public:

  OMDStack();

  ~OMDStack();

  void PopAndAdd(void);

  void Push(OMDBase * pItem);

};



#endif
