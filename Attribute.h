
#ifndef _attribute_h
#define _attribute_h

#include <string>
#include <set>

//#include "cardinality.hpp"
#include "OMDBase.h"
#include "OMDTask.h"
#include "omdt.h"
#include "OMDNode.h"

class CAttributeName : public OMDBase
{
    std::string m_AttrName;

public:
    CAttributeName( const char * pName);

    virtual ~CAttributeName();

    const std::string getString(void) const;

    const char * getType(void) const;

    const std::string getName(void) const;

    //   virtual
    //   bool operator==(const OMDBase & rhs) const;

    //   virtual
    //   bool operator!=(const OMDBase & rhs) const;

    virtual
    bool equal(const OMDBase & rhs) const;

    virtual
    bool notEqual(const OMDBase & rhs) const;

    virtual void Execute ( OMDTask * pVisitor ) const;
    virtual void Indirect( OMDTask * pTask    ) const;
};

class CAttributeValue : public OMDBase
{
    std::string m_AttrValue;
public:
    
    CAttributeValue( const char   * pName);
    CAttributeValue( const std::string & rName);

    virtual ~CAttributeValue();

    const std::string getString(void) const;

    const char * getType(void) const;

    const std::string getValue(void) const;

    //   virtual
    //   bool operator==(const OMDBase & rhs) const;

    //   virtual
    //   bool operator!=(const OMDBase & rhs) const;

    virtual
    bool equal(const OMDBase & rhs) const;

    virtual
    bool notEqual(const OMDBase & rhs) const;

    virtual void Execute ( OMDTask * pVisitor ) const;
    virtual void Indirect( OMDTask * pTask    ) const;
};

class CAttributeValuePair : public OMDNode
{
    CAttributeName  m_Attr;
    CAttributeValue m_Value;

public:

    CAttributeValuePair(OMDBase::NodeTypeEnum kind, const char * pVal, const Loc * pLoc = NULL);

    ~CAttributeValuePair();

    std::string getName (void) const;
    std::string getValue(void) const;
  
    const std::string getString(void) const;

    virtual
    bool equal   (const OMDBase & rhs) const;

    virtual
    bool notEqual(const OMDBase & rhs) const;

    virtual void Execute ( OMDTask * pVisitor ) const;
    virtual void Indirect( OMDTask * pTask    ) const;

    const char * getType(void) const;
};

class CAttributeList : public OMDNode
{
    //string m_Kind;
    //multimap<string, OMDBase *> m_Map;

public:

    CAttributeList(OMDBase::NodeTypeEnum kind, const Loc * pLoc = NULL);

    virtual ~CAttributeList();

    //void Add(const CAttributeValuePair * pAVP);

    //void Add(const OMDNode * pNode);

    const char * getType      (void) const; // Attribute, Class, Interaction ..
    const std::string  getName(void) const; // name of EDT, CDT, CC
    //std::string  getName    (void) const; // name of EDT, CDT, CC
    std::string  getDataType(void) const;
    
    size_t size(void) const;

    const std::string getString() const;

    const OMDBase * getKind   (const std::string & kind) const;
    const OMDBase * operator[](const std::string & kind) const;

    virtual void Execute ( OMDTask * pVisitor ) const;
    virtual void Indirect( OMDTask * pTask    ) const;
};

class CAttribute : public CAttributeList
{
public:
    
    CAttribute();

    virtual ~CAttribute();

    virtual const char * getType(void) const;

    virtual void Execute (OMDTask * pVisitor ) const;
    virtual void Indirect(OMDTask * pTask    ) const;
    
};

class CInteraction : public CAttributeList
{
public:
    
    CInteraction();

    virtual ~CInteraction();

    virtual const char * getType(void) const;

    virtual void Execute (OMDTask * pVisitor ) const;
    virtual void Indirect(OMDTask * pTask    ) const;
    
};

class CParameter : public CAttributeList
{
public:
    
    CParameter();

    virtual ~CParameter();

    virtual const char * getType(void) const;

    virtual void Execute (OMDTask * pVisitor ) const;
    virtual void Indirect(OMDTask * pTask    ) const;
    
};

//
// This is the OMD entry of the form:
//
//   (Note (NoteNumber <nn>)
//         (NoteText "<text>"))
//
class CNote : public CAttributeList
{
public:
    
    CNote(const Loc * pLoc = NULL);

    virtual ~CNote();

    const char * getType(void) const;

    virtual void Execute (OMDTask * pVisitor ) const;
    virtual void Indirect(OMDTask * pTask    ) const;
    
};

//
// This is the OMD entry of the form:
//
//   (Note (NoteNumber <nn>)
//         (NoteText "<text>"))
//
class CNoteText : public CAttributeValuePair
{
public:
    
    CNoteText( const char * pVal, const Loc * pLoc = NULL);

    virtual ~CNoteText();

    const char * getType(void) const;

    virtual void Execute (OMDTask * pVisitor ) const;
    virtual void Indirect(OMDTask * pTask    ) const;
    
};

//
// The is the note reference and is represented in the OMD/OMT as
//
//    [<nn><optional-nn-list>]
//
// where <nn>               is a number of a note and
//       <optional-nn-list> is a comma separated list of note numbers
//
//       ,<nn><optional-nn-list>
//
// duplicates and ordering are preserved
//
class CNoteList : public OMDNode
{
    std::set<size_t> mNoteRefs;

public:

    CNoteList(int number);

    virtual ~CNoteList();

    virtual void Execute (OMDTask * pVisitor ) const;
    virtual void Indirect(OMDTask * pTask    ) const;

    const char * getType(void) const;

    const std::string getString(void) const;

    std::set<size_t> getNoteSet(void) const;

    void Add( const CNoteList * pNL );

    void Add( size_t number );
    
};

#endif
// Local Variables:
// mode:C++
// End:
