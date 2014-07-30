//
// class OMDBase declaration
//

#ifndef _OMDBase_h
#define _OMDBase_h

//#include <string>
//#include <iostream>
//#include <ostream>
//#include <sstream>


//#include <vector>

// class CAttributeValuePair;
// class CAttributeList;
// struct yyAVP;
// struct yyAL;
// struct yyStr;
// struct yyTOK;

#include <ostream>
#include <string>
#include <sstream>

//using namespace std;

#include "CPrint.h"

struct Loc : public CPrint
{
    int first_line;
    int first_column;
    int last_line;
    int last_column;
  
    Loc();
    Loc( const Loc & rLoc );

    const Loc & operator=(const Loc & rhs);

    virtual ~Loc();

    virtual std::ostream & print(std::ostream & out) const;
};

class CAttribute;
class CAttributeList;
class CAttributeValuePair;
class CBitfieldComponent;
class CBitfieldDataType;
class CClass;
class CComplexComponent;
class CComplexDataType;
class CDimension;
class CEnumeratedDataType;
class CEnumeration;
class CInteraction;
class CNote;
class CNoteList;
class CNoteText;
class COMDocument;
class CObjectModel;
class CParameter;
class CRoutingSpace;
class OMDNode;

#include "omdt.h"
typedef yytokentype NodeTypeEnum;

class OMDTask;

//
// This implementation supports a single "document" which is
// rooted with the g_pRoot static member of this class.
//

class OMDBase : public CPrint
{
public:

    typedef ::NodeTypeEnum NodeTypeEnum;

    OMDBase(NodeTypeEnum nodeType, const Loc * pLoc = NULL);

    virtual ~OMDBase(void);

    //
    // Get the string representation of this node (recursive).
    // The classes derived from this must implement this method.
    //
    virtual const std::string getString() const = 0;

    NodeTypeEnum getNodeType() const
    {
      return m_eNodeType;
    }

    //
    // get the type of node. This is the const char * equivalent of the token.
    //
    virtual const char * getType(void) const = 0;

    //
    // For the visitor design pattern
    //
    virtual void Execute (OMDTask * pTask) const = 0;

    //
    // For the visitor design pattern
    //
    virtual void Indirect(OMDTask * pTask) const = 0;

    //
    // Get the parent of this node. If this is the
    // root, NULL is returned.
    //
    OMDBase * getParent(void) const;

    //
    // Set the parent of this node.
    //
    void setParent(OMDBase * pParent);

    //
    // get the input file location struct
    //
    const Loc * getLocation(void) const;

    //   virtual
    //   bool operator==(const OMDBase & rhs) const = 0;

    //   virtual
    //   bool operator!=(const OMDBase & rhs) const = 0;

    virtual
    bool equal(const OMDBase & rhs) const = 0;

    virtual
    bool notEqual(const OMDBase & rhs) const = 0;

    //
    // Return the root of the document. There is only one
    // document attached to root in this implementation.
    // Use getTree() to clear the root for another document.
    //
    static OMDBase * getRoot(void);

    //
    // Get the root of the document and clear the global 
    // root pointer (g_pRoot) to prepare for another document.
    //
    static OMDBase * getTree(void);

    std::ostream & print(std::ostream & out) const;

private:

    //
    // This is the token from the parsing process.
    //
    NodeTypeEnum          m_eNodeType;

    OMDBase             * m_pParent;

    Loc                   m_Loc;

    static OMDBase      * g_pRoot;

    static size_t         g_Count;
    static size_t         g_MaxCount;

};

//std::ostream & operator<<(std::ostream & out, const CPrint & item);

#endif

// Local Variables:
// mode:C++
// End:
