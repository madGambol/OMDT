
#ifndef _OMDNode_h
#define _OMDNode_h

#include <string>
#include <vector>
#include <sstream>

#include "OMDBase.h"

//
// The data model for the OMD data structure "scaffold" is the node:
// the node can have a parent (except the root node), and
// children; each node has a type (for simplicity, the yytokentype
// is used to identify the node from the parsing processing).
//
// Because of the ubiquity of the notes in the OMD file,
// the note reference is stored in the OMDBase class.
//
//
class OMDNode : public OMDBase
{
public:
    
    OMDNode(OMDBase::NodeTypeEnum, const Loc * pLoc = NULL);

    virtual ~OMDNode();

    //
    // Get a string value of this node. This is performed recursively.
    //
    const std::string getString(void) const;

    //   virtual
    //   bool operator==(const OMDBase & rhs) const;

    //   virtual
    //   bool operator!=(const OMDBase & rhs) const;

    virtual
    bool equalSameOrder(const OMDBase & rhs) const;

    virtual
    bool equalAnyOrder(const OMDBase & rhs) const;

    virtual
    bool equal(const OMDBase & rhs) const;

    virtual
    bool notEqual(const OMDBase & rhs) const;

    //
    // Get a child based on the index. NULL is returned
    // if the index is out of range.
    //
    OMDBase * getChild(size_t index) const;

    //
    // Get the child with attribute kind (such as DataType)
    // returns the OMDBase pointer to the object or NULL
    // if not found.
    //
          OMDBase * getChild(const std::string     & kind);
    const OMDBase * getChild(const std::string     & kind) const;
    
          OMDBase * getChild(OMDBase::NodeTypeEnum   kind);

    //
    // Get the count of the children
    //
    size_t getChildCount(void) const;

    //
    // Add a child to the list of children
    //
    void addChild(OMDBase * pChild);

    void Add(OMDBase * pChild);

    //
    // This returns "OMDNode" but could do more (be virtual?)
    //
    const char * getType(void) const;

    //
    // This method supports the visitor design pattern
    //
    virtual void Execute(OMDTask * pTask) const;

    const std::vector<OMDBase *> & getChildren() const;

    std::ostream & print(std::ostream & out) const;

protected:

    std::vector<OMDBase *> m_Children;

};
#endif

// Local Variables:
// mode:C++
// End:
