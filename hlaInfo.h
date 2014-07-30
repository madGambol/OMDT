
#ifndef _hlaInfo_h
#define _hlaInfo_h

#include <string>
#include <vector>
#include <map>

#include "OMDTask.h"

typedef enum
  {
    eCLASSTYPEUNDEFINED,
    eOBJECT,
    eINTERACTION
  } ClassType;

class AttrInfo
{
  std::string m_AttrName;
  //
  // This is the description of the type.
  //
  ComplexDataTypeInfo     * m_pDataTypeInfo;

  std::string m_Cardinality;  // cardinality string

  CCardinalityExpr        * m_pCardinalityExpr; // for cardinality expression

  std::string m_RoutingSpace; // 
  std::string m_MessageOrdering;
  std::string m_Description;
  std::string m_Units;

  AttrInfo(void) {}

 public:

  ~AttrInfo(void);

  const std::string & getAttrName(void) const;
  const std::string & getAttrType(void) const;

  //
  // Make an AttrInfo
  static
  AttrInfo * Factory(void);
};

typedef enum
  {
    eUNDEFINED = 0,   // if we don't set anything, a value
    ePUBLISH,         // attr
    eSUBSCRIBE,       // attr
    ePUBSUBSCRIBE,    // attr 
    eINITIATE,        // para
    eRECEIVE,         // para
    eINITIATERECEIVE  // para
  } PSIRMode;

class AttrInfoList : public std::map<std::string, AttrInfo *>
{
 public:
  //
  // create some aliases for the iterator types.
  //
  typedef std::vector<AttrInfo *>::iterator       AttrInfoListIter;
  typedef std::vector<AttrInfo *>::const_iterator ConstAttrInfoListIter;

  AttrInfoList()
  {
  }

  ~AttrInfoList()
  {
  }

  // Copy CTOR
  AttrInfoList(const AttrInfoList &);
  AttrInfoList & operator=(const AttrInfoList & item);

  bool AddAttrInfoList(const AttrInfoList & attrInfoList);
  AttrInfoList & operator+ (const AttrInfoList & item);
  AttrInfoList & operator=+(const AttrInfoList & rhs );
  AttrInfoList & operator=+(const AttrInfo     & rhs );


  AttrInfoListIter      Find(const std::string & attrName);
  ConstAttrInfoListIter Find(const std::string & attrName) const;

  AttrInfoListIter      Begin(void);
  AttrInfoListIter      End  (void);

  ConstAttrInfoListIter Begin(void) const;
  ConstAttrInfoListIter End  (void) const;
};

class ClassInfo 
{
  ClassType    m_ClassType;    // object or interaction
  std::string  m_ClassName;    // fully qualified class name ("blah.foo.bar")

  size_t       m_ID;           // ID value
  size_t       m_SuperClassID; // 

  AttrInfoList m_InhAttrList;
  AttrInfoList m_DirAttrList;

  PSIRMode     m_PSIRMode;

  ClassInfo(const char * pClassName);

 public:

  ClassInfo(const ClassInfo & item);

  ClassInfo & operator=(const ClassInfo & rhs);

  ~ClassInfo(void);

  void AddParent(const ClassInfo & item);

  void AddDirAttrList( const AttrInfoList & dirAttrVec); // add attr list
  void AddDirAttr    ( const AttrInfo     & dirAttr   ); // add 1 attr

  const std::string  & getClassName(void) const;
  const AttrInfoList & getInhAttr  (void) const; // inherited attr (para)
  const AttrInfoList & getDirAttr  (void) const; // direct    attr (para)

  void      SetID(size_t id );
  size_t    GetID(void) const { return m_ID; }

  void      SetSuperClassID(size_t superClassId);
  size_t    GetSuperClassID(void) const { return m_SuperClassID; }

  //
  // This method sets the PSIR mode and ClassType if undefined.
  //
  void      SetPSIRMode(PSIRMode mode);
  PSIRMode  GetPSIRMode(void) const { return m_PSIRMode; }

  void      SetClassType(ClassType classType);
  ClassType GetClassType(void) const { return m_ClassType; }

  static
  ClassInfo  * Factory(const char * pClassName );
  
};

class HLASummary
{
  std::vector<std::string             > m_Names;
  std::map   <std::string, ClassInfo *> m_Info;

  std::map   <size_t,      std::string> m_IDtoName;

 public:

  HLASummary()
  {
  }

  size_t getInfoSize (void) const;

  const ClassInfo & getInfo(const std::string & name) const;
  const ClassInfo & getInfo(size_t index) const;

  bool addInfo( const ClassInfo & item);

};

//
// Gather and summarize HLA information about the omt/omd file.
//
class HLAInfo : public OMDTask
{
  HLASummary m_ObjectSummary;
  HLASummary m_InteractionSummary;

  std::map<string, ComplexDataType *> m_ComplexDataByName;

 public:

  HLAInfo(void);

  virtual ~HLAInfo(void);

  const HLASummary & getObjectSummary     (void) const;
  const HLASummary & getInteractionSummary(void) const;

  //
  // The following methods support the "Visitor" design pattern and
  // implements the summary of HLA info.
  //
  int Process( OMDBase * pItem );

  int Process( OMDNode * pItem );

  int Process( AttributeValuePair * pItem );

  int Process( ComplexDataType    * pItem );

  int Process( Singleton          * pItem );
};

#endif // _hlaInfo_h

// Local Variables:
// mode:C++
// End:
