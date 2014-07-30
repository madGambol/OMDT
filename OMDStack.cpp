

#include "OMDStack.h"

#include <string>

#include <assert.h>

using namespace std;

const char * tokenLookup(int token);
size_t getLine();

OMDStack::OMDStack()
{
}

OMDStack::~OMDStack()
{
}

void OMDStack::PopAndAdd(void)
{
  if (size() == 0) 
  {
    printf("OMDStack is empty\n");
    return;
  }

  OMDBase * pChild = top();
      
  pop();

  if (size() == 0) 
  {
    printf("OMDStack is empty after pop\n");
    return;
  }

  OMDNode * pParent = dynamic_cast<OMDNode *>(top());

  if (!pParent)
  {
    printf("Type about to assert: %s at line number %d\n", top()->getType(), getLine() );

    printf("getString() returns:  %s\n", top()->getString().c_str() );
  }

  assert(pParent);

//   printf("OMDStack::PopAndAdd: Adding [%s/%s] to top [%s/%s]\n", 
// 	 pChild->getType(),
// 	 tokenLookup( pChild->getNodeType() ),
// 	 top()->getType(),
// 	 tokenLookup( top() ->getNodeType() )
// 	);
  
  pParent->addChild( pChild );
}

void OMDStack::Push(OMDBase * pItem)
{
  if (pItem == NULL)
  {
    printf("item to push is NULL \n");
    return;
  }

//  string nodeName = tokenLookup( pItem->getNodeType() );

//    printf("Stack Depth = %d\n", size() );
//    printf("Pushing       [%s]\n", nodeName.c_str() );
//    printf("type          [%s]\n", pItem->getType() );

  push( pItem );

}
