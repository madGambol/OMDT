

#include "hlaInfo.h"

using namespace std;

template <class T>
class Factory : private list<T *>
{
 public:
  Factory() {}

  ~Factory()
  {
    while (size())
    {
      T * pItem = front();
      pop_front();
      delete pItem;
    }
  }

  T * NewItem(void)
  {
    T * pRetVal = new T();

    push_back( pRetVal );

    return pRetVal;
  }

  static Factory<T> & getInstance()
  {
    static Factory<T> theOne;

    return theOne;
  }

  static T * getOne(void)
  {
    static Factory<T> & theOneRef = getInstance();

    return theOneRef.NewItem();
  }
};

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// AttrInfo ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

AttrInfo::~AttrInfo(void)
{
}

const string & AttrInfo::getAttrName(void) const
{
  return m_AttrName;
}

const string & AttrInfo::getAttrType(void) const
{
  return m_pDataTypeInfo->
}

//
// Make an AttrInfo
static
AttrInfo * AttrInfo::Factory(void)
{
  static Factory<AttrInfo> theOne;

  return theOne.getOne();
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// AttrInfoList ////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// ClassInfo ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
ClassInfo::ClassInfo(const char * pClassName, ClassType 
		     classType = eCLASSTYPEUNDEFINED)
  :
  m_ClassType   ( classType  ),
  m_ClassName   ( pClassName ),
  m_ID          ( 0          ), // not a valid # for a defined class
  m_SuperClassID( 0          ),
  m_PSIRMode    ( eUNDEFINED )    
{
  m_InhAttrList.reserve(32); // some number
  m_DirAttrList.reserve(32);
}

ClassInfo::ClassInfo(const ClassInfo & item)
  :
  m_ClassType   ( item.m_ClassType  ),
  m_ClassName   ( item.m_ClassName  ),
  m_ID          ( item.m_ID         ), // not a valid # for a defined class
  m_SuperClassID( item.m_SuperClassID),
  m_InhAttrList ( item.m_InhAttrList),
  m_DirAttrList ( item.m_DirAttrList),  
  m_PSIRMode    ( item.m_PSIRMode   )
{
}

ClassInfo & ClassInfo::operator=(const ClassInfo & rhs)
{
  if (this == &rhs) return *this;

  m_ClassType    = rhs.m_ClassType;
  m_ClassName    = rhs.m_ClassName;
  m_ID           = rhs.m_ID;
  m_SuperClassID = rhs.m_SuperClassID;
  m_InhAttrList  = rhs.m_InhAttrList;
  m_DirAttrList  = rhs.m_DirAttrList;
  m_PSIRMode     = rhs.m_PSIRMode;

  return *this;
}

void ClassInfo::AddParent(const ClassInfo & parent)
{
  if (&parent == this) return; // don't add ourselves as our own parent.

  m_InhAttrList.AddAttrInfoList( parent.m_InhAttrList );
  m_InhAttrList.AddAttrInfoList( parent.m_DirAttrList );

}

void ClassInfo::AddDirAttrList( const AttrInfoList & dirAttrVec) // add attr list
{
  m_DirAttrList += dirAttrVec;
}

void ClassInfo::AddDirAttr    ( const AttrInfo     & dirAttr   ) // add 1 attr
{
  m_DirAttrList += dirAttr;
}

const std::string & ClassInfo::getClassName(void) const
{
  return m_ClassName
}

const AttrInfoList & ClassInfo::getInhAttr(void) const // inherited attr (para)
{
  return m_InhAttrList;
}

const AttrInfoList & ClassInfo::getDirAttr(void) const // direct    attr (para)
{
  return m_DirAttrList;
}

void ClassInfo::SetID(size_t id )
{
  m_ID = id;
}

void ClassInfo::SetSuperClassID(size_t superClassId)
{
  m_SuperClassID = superClassId;
}

void ClassInfo::SetPSIRMode(PSIRMode mode)
{
  switch (m_ClassType)
  {
  case eOBJECT:
    switch (mode)
    {
    case ePUBLISH:
    case eSUBSCRIBE:
    case ePUBSUBSCRIBE:
      m_PSIRMode = mode;
      break;
    default:
      break;
    }
    break;
    
  case eINTERACTION:
    switch (mode)
    {
    case eINITIATE:
    case eRECEIVE:
    case eINITIATERECEIVE:
      m_PSIRMode = mode;
      break;
    default:
      break;
    }
    break;
  case eCLASSTYPEUNDEFINED:
    switch (mode)
    {
    case eINITIATE:
    case eRECEIVE:
    case eINITIATERECEIVE:
      m_ClassType = eOBJECT;
      m_PSIRMode  = mode;
      break;
    case ePUBLISH:
    case eSUBSCRIBE:
    case ePUBSUBSCRIBE:
      m_ClassType = eINTERACTION;
      m_PSIRMode  = mode;
      break;
    default:
      break;
    }
    break;
    
  }
}

void ClassInfo::SetClassType(ClassType classType)
{
  m_ClassType = classType;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// HLASummary //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// HLAInfo /////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

