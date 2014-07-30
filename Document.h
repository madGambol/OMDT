

#ifndef _DocumentCheck_h
#define _DocumentCheck_h

#include <sstream>
#include "OMDTask.h"
#include <list>
#include <map>
#include <string>
#include <vector>
#include "Attribute.h"
#include "report.h"
#include "baseMap.h"

//using namespace std;

/*

  Attribute Check:

  An HLA object model shall support representation of the following 
  characteristics for attributes in the attribute table:

  ·      Object class
  ·      Attribute name
  ·      Datatype
  ·      Cardinality
  ·      Units
  ·      Resolution
  ·      Accuracy
  ·      Accuracy condition
  ·      Update type
  ·      Update rate/condition
  ·      Transferable/acceptable
  ·      Updateable/reflectable
  ·      Routing space

*/

class CTypeDescr;
class NodeMap;

typedef unsigned int classID;
typedef unsigned int interactionID;

typedef std::pair  < std::string, std::string > StringStringPair;
typedef std::set   < StringStringPair         > StringStringPairSetMap;
typedef std::list  < std::string              > StringList;
typedef std::list  < const CComplexDataType * > CdtList;
typedef std::map   < std::string, CdtList     > NameToCdtListMap;
typedef NameToCdtListMap::const_iterator        ConstNameToCdtListMapIter;
typedef std::set   < std::string              > StringSet;
typedef std::vector< std::string              > StringVector;
typedef std::map < std::string, StringSet     > StringToStringSetMap;
typedef std::set < StringList                 > StringListSet;
typedef std::list< const CAttributeValuePair *> AttrValuePairList;
typedef std::map < std::string, NodeMap *     > StringToNodeMapMap;
typedef std::map < std::string, CTypeDescr *  > StringToTypeDescrMap;
typedef StringToTypeDescrMap::const_iterator ConstStringToTypeDescrMapIter;

typedef CBaseMap < std::string, const CEnumeratedDataType *> StringToEnumDataTypeBaseMap;
typedef CBaseMap < std::string, const CComplexDataType *   > StringToCdtBaseMap;
typedef CBaseMap < std::string, const CComplexComponent *  > StringToCcBaseMap;
typedef CBaseMap < std::string, const OMDBase *            > StringToOmdBaseBaseMap;
typedef CBaseMap < std::string, const CAttribute *         > StringToAttrBaseMap;
typedef CBaseMap < std::string, const CParameter *         > StringToParamBaseMap;
typedef CBaseMap < classID,     const OMDBase *            > ClassIdToOmdBaseBaseMap;
typedef CBaseMap < const OMDBase *, const CAttribute *     > OmdBaseToAttrBaseMap;
typedef CBaseMap < const OMDBase *, const CParameter *     > OmdBaseToParmBaseMap;

typedef CBaseMap < const CComplexDataType  *, const CComplexComponent  *> CdtToCcBaseMap;
typedef CBaseMap < const CBitfieldDataType *, const CBitfieldComponent *> BdtToBfcBaseMap;
typedef CBaseMap < const CComplexDataType  *, const CBitfieldComponent *> CdtToBfcBaseMap;

typedef std::map < const CComplexDataType  *, bool>   dependsMap;
typedef dependsMap::const_iterator dependsMapIter;

typedef StringToCcBaseMap::const_TtoNiterator StrToCcConstIter;

typedef std::multimap< std::string, const CEnumeratedDataType *> NameToEnumDataMulitMap;
typedef std::multimap< std::string, const CComplexDataType * >   NameToCdtMultiMap;

//typedef std::multimap< std::string, const CComplexComponent * >::const_iterator constIter;
typedef NameToEnumDataMulitMap::const_iterator constEnumIter;
typedef NameToCdtMultiMap     ::const_iterator constCDTIter;
//typedef std::multimap< const CComplexDataType *, const CComplexComponent * >::const_iterator constCDTCCIter;
// multimap<OMDBase *, const CAttribute *>::const_iter
typedef OmdBaseToAttrBaseMap::const_TtoNiterator constAttrIter;
typedef OmdBaseToParmBaseMap::const_TtoNiterator constParmIter;

typedef std::multimap< std::string, const OMDBase *    >::const_iterator constOMDBIter;
typedef std::multimap< classID,     const OMDBase *    >::const_iterator constIdOMDBIter;
typedef std::map     < std::string, CTypeDescr *       >::const_iterator constTypeDescrIter;
typedef std::multimap< std::string, const CAttribute * >::const_iterator constNameAttrIter;

//      CBaseMap< const OMDBase *, const CParameter *>
// class CContext : public std::vector< const OMDBase * >
// {
// public:
//   CContext(const OMDBase * pCurrent);
//   CContext(const CContext & context);
//   CContext & operator=(const CContext & rhs);
//   ~CContext();
// };

//template <class T, class N> typedef std::multimap< T, N > TtoNmap;

typedef std::string String;
typedef std::ostream Ostream;
typedef std::map< String, CTypeDescr * > TypeNameToTypeDescrMap;
typedef std::pair <String,String> StringPair;
typedef std::list <String>        StringList;

typedef std::set  <String>        StringSet;
typedef StringSet::iterator       StringSetIter;
typedef StringSet::const_iterator ConstStringSetIter;

typedef std::set  <StringPair>        StringPairSet;
typedef StringPairSet::iterator       StringPairSetIter;
typedef StringPairSet::const_iterator ConstStringPairSetIter;

typedef std::map <String, StringSet     >    StringToStringSetMap;
typedef StringToStringSetMap::iterator       StringToStringSetMapIter;
typedef StringToStringSetMap::const_iterator ConstStringToStringSetMapIter;

typedef std::map <String, StringPairSet > StringToStringPairSetMap;

typedef std::map  <String, StringPairSet >       NameToAttrTypePairSetMap;
typedef NameToAttrTypePairSetMap::iterator       NameToAttrTypePairSetMapIter;
typedef NameToAttrTypePairSetMap::const_iterator ConstNameToAttrTypePairSetMapIter;

typedef std::map  <String, StringPairSet >        NameToParamTypePairSetMap;
typedef NameToParamTypePairSetMap::iterator       NameToParamTypePairSetMapIter;
typedef NameToParamTypePairSetMap::const_iterator ConstNameToParamTypePairSetMapIter;

//
// Gather OMD/OMT document information
//
class Document : public OMDTask
{
    void internalInitialize(void);
 
protected:
    
    COMDocument * m_pOMDoc;

    struct DefRefContext
    {
        OMDBase * pReference;
        OMDBase * pDefine;
    };

    typedef unsigned int classID;
    typedef unsigned int interactionID;

    TypeNameToTypeDescrMap                          mTypes;
    //std::map< String, NodeMap *>                  mMaps;
 
    const CObjectModel                            * mpObjectModel;
    //std::list< const CAttributeValuePair *>       mFrontMatter;

    std::vector< String >                           mRoutingSpace; // vector of subspace names
    String                                          mRoutingSpaceName;

    //std::list< const CEnumeratedDataType *>            mEnumeratedDataTypes;
    //std::map < String, const CEnumeratedDataType *> mEnumNameMap;
    //StringList                           mEDT_Errors;
    //StringList                           mEDT_Warnings;

    //std::list< const CComplexDataType *>               mComplexDataTypes;

    //std::list< const CClass *>                         mObjects;
    //std::multimap< classID, DefRefContext *>           mClassIdXref;
    //std::multimap< classID, String >              mClassIdToName;
    //std::multimap< String, classID >              mClassNameToId;
    //String                                        mClassName;
    //classID                                            mClassID;

    //std::list< const CInteraction *>                   mInteractions;
    //std::multimap< interactionID, DefRefContext *>     mIaIdXref;
    //std::multimap< interactionID, String >        mIaIdToName;
    //std::multimap< String, interactionID >        mIaNameToId;
    //String                                        mIaName;
    //interactionID                                      mIaID;

    //std::multimap< String, OMDBase *>             mTypeDefinitions;
    //std::multimap< String, OMDBase *>             mTypeReferences;

    //std::list< const CNote * >                         mNotes;
    //CNoteList                                          mAllNoteRefs;

    //CContext                                           mContext;

    CBaseMap< String, const CEnumeratedDataType *>  mNameToEnumTypes;

    //
    // CDT name to CComplexDataType *. This CBaseMap contains all CDT's.
    //
    CBaseMap< String, const CComplexDataType    *>  mNameToCDT;
    CBaseMap< String, const CComplexComponent   *>  mFieldNameToCC;
    CBaseMap< String, const CComplexComponent   *>  mDataTypeToCC;

    typedef  CBaseMap< String,
                       const CComplexComponent *>::const_TtoNiterator StrToCcConstIter;

    CBaseMap< String, const OMDBase *>              mNameToTypes;
    CBaseMap< const CComplexDataType  *,
              const CComplexComponent *>            mCDTtoCC;
  
    CBaseMap< const CBitfieldDataType  *,
              const CBitfieldComponent *>           mBFDTtoBFC;
  
    CBaseMap< const CComplexDataType   *,
              const CBitfieldComponent *>           mCDTtoBF;
  
    CBaseMap< String,  const OMDBase *>             mNameToObjects;
    CBaseMap< String,  const OMDBase *>             mNameToAttr;
    CBaseMap< classID, const OMDBase *>             mIdToObjects;
    CBaseMap< classID, const OMDBase *>             mParentIdToObjects;
    CBaseMap< String,  const CAttribute *>          mDataTypeToAttr;
  //CBaseMap< String,  const CCardinalityExpr *>    mCardinalityToAttr;
    
    CBaseMap< const OMDBase *, const CAttribute *>  mObjectToAttr;

    CBaseMap< String, const OMDBase *>              mNameToIa;
    CBaseMap< String, const OMDBase *>              mNameToParm;
    CBaseMap< classID,const OMDBase *>              mIdToIa;
    CBaseMap< classID,const OMDBase *>              mParentIdToIa;
    CBaseMap< String, const CParameter *>           mDataTypeToParm;
    CBaseMap< const OMDBase *, const CParameter *>  mIaToParm;

    typedef std::map <const CComplexDataType *, bool> dependsMap;
    typedef dependsMap::const_iterator                dependsMapIter;

    typedef std::multimap< String, const CComplexComponent *> NametoCcMMap;
    typedef NametoCcMMap::const_iterator ConstNametoCcMMapIter;
    typedef NametoCcMMap::iterator       NametoCcMMapIter;
    
    typedef std::multimap< String, const CEnumeratedDataType *> NameToEdtMMap;
    typedef NameToEdtMMap::const_iterator ConstNameToEdtMMapIter;
    typedef NameToEdtMMap::iterator       NameToEdtMMapIter;
    
    typedef CBaseMap< String, const CEnumeratedDataType *>::const_TtoNiterator ConstStringToEdtBaseMapIter;

    typedef std::multimap< String, const CComplexDataType *> NameToCdtMMap;
    typedef NameToCdtMMap::const_iterator ConstNameToCdtMMapIter;
    typedef NameToCdtMMap::iterator       NameToCdtMMapIter;
    
    typedef std::multimap< const CComplexDataType *, const CComplexComponent *> CdtToCcMMap;
    typedef CdtToCcMMap::const_iterator  ConstCdtToCcMMapIter;
    typedef CdtToCcMMap::iterator        CdtToCcMMapIter;
    
    typedef std::multimap< const OMDBase *, const CAttribute *> OMDBaseToAttrMMap;
    typedef OMDBaseToAttrMMap::const_iterator ConstOMDBaseToAttrMMapIter;
    typedef OMDBaseToAttrMMap::iterator OMDBaseToAttrMMapIter;
    
    typedef std::multimap< const OMDBase *, const CParameter *> OMDBaseToParamMMap;
    typedef OMDBaseToParamMMap::const_iterator ConstOMDBaseToParamMMapIter;
    typedef OMDBaseToParamMMap::const_iterator OMDBaseToParamMMapIter;
    
    typedef std::multimap< String,  const OMDBase *   >::const_iterator constOMDBIter;
    typedef std::multimap< classID, const OMDBase *   >::const_iterator constIdOMDBIter;
    typedef std::multimap< String,  const CAttribute *>::const_iterator constNameAttrIter;
    
    typedef std::map     < String,  CTypeDescr *      >::const_iterator constTypeDescrIter;

    //   struct CDTcount
    //   {
    //     const CComplexDataType * mpCDT;
    //     size_t mCount;
    //     CDTcount();
    //     CDTcount( const CComplexDataType * pCDT);
    //     CDTcount( const CDTcount & cdtCount);
    //     CDTcount & operator=(const CDTcount & rhs);
    //   };

    // void doCircularityAnalysis( std::list<CDTcount> & cdtCountList);

    std::set<const CComplexDataType *> getCdtSet(void);
  
public:

    Document(COMDocument *);

    virtual ~Document();

    //<editor-fold desc="visitor callback methods">
    virtual int  Process ( const CAttribute          * pItem );
    virtual int  Process ( const CAttributeList      * pItem );
    virtual int  Process ( const CAttributeValuePair * pItem );
    virtual int  Process ( const CBitfieldComponent  * pItem );
    virtual int  Process ( const CBitfieldDataType   * pItem );
    virtual int  Process ( const CClass              * pItem );
    virtual int  Process ( const CComplexComponent   * pItem );
    virtual int  Process ( const CComplexDataType    * pItem );
    virtual int  Process ( const CDimension          * pItem );
    virtual int  Process ( const CEnumeratedDataType * pItem );
    virtual int  Process ( const CEnumeration        * pItem );
    virtual int  Process ( const CInteraction        * pItem );
    virtual int  Process ( const CNote               * pItem );
    virtual int  Process ( const CNoteText           * pItem );
    virtual int  Process ( const CNoteList           * pItem );
    virtual int  Process ( const COMDocument         * pItem );
    virtual int  Process ( const CObjectModel        * pItem );
    virtual int  Process ( const CParameter          * pItem );
    virtual int  Process ( const CRoutingSpace       * pItem );
    virtual int  Process ( const OMDBase             * pItem );
    virtual int  Process ( const OMDNode             * pItem );
    virtual int  Process ( const Singleton           * pItem );
    virtual int  Process ( const CAttributeName      * pItem );
    virtual int  Process ( const CAttributeValue     * pItem );
    //</editor-fold>

    String        getFQObjName      ( const OMDBase * pItem ) const;
    String        getFQIntName      ( const OMDBase * pItem ) const;
    
    StringList    getFQObjNameList  ( const OMDBase * pItem ) const;
    StringList    getFQIntNameList  ( const OMDBase * pItem ) const;

    StringList    getObjAttrNameList( const String & objName ) const;

    StringSet     getObjectAttrSet  ( const OMDBase * ) const;

    StringPairSet getObjectAttrTypePairSet( const OMDBase * ) const;

    StringPairSet getInteractionParamTypePairSet( const OMDBase * ) const;

    StringSet     getIaParamSet     ( const OMDBase * ) const;

    //
    // return the full list of all the attributes including those of the
    // parent classes in the set of attributes returned.
    //
    StringSet     getClosedObjectAttrSet        ( const OMDBase * pObj) const;
    StringPairSet getClosedObjectAttrTypePairSet( const OMDBase * pObj) const;

    StringToStringSetMap     getObjectToAttrSets      (void) const;
    StringToStringSetMap     getClosedObjectToAttrSets(void) const;
    NameToAttrTypePairSetMap getClosedObjectToAttrTypePairSets(void) const;
    
    //
    // return the full list of all the attributes including those of the
    // parent classes in the set of attributes returned.
    //
    //StringSet         getClosedObjectAttrSet( const OMDBase * ) const;
    
    //
    // return the full list of all the parameter including those of the
    // parent classes in the set of parameters returned.
    //
    StringSet     getClosedInteractionParamSet        ( const OMDBase * pInt) const;
    StringPairSet getClosedInteractionParamTypePairSet( const OMDBase * pInt) const;
    
    StringToStringSetMap      getInteractToParamSets      (void) const;
    StringToStringSetMap      getClosedInteractToParamSets(void) const;
    NameToParamTypePairSetMap getClosedInteractToParamTypePairSets(void) const;
        
    StringSet  getFQObjNames(void) const;
    StringSet  getFQIntNames(void) const;

    std::set<StringPair> getFQObjNameAndNamePairs(void) const;
    std::set<StringPair> getFQIntNameAndNamePairs(void) const;

    std::set<StringList> getFQObjNamesLists(void) const;
    std::set<StringList> getFQIntNamesLists(void) const;
    
    String getCardinality( const OMDBase * ) const;

	const char * getSpaceName( int indx ) const;
	int          getDims     ( void     ) const;

    virtual Ostream & print(Ostream & out) const;
    //virtual stringstream & print( stringstream & out);
};

bool operator==(const OMDBase & lhs, const OMDBase & rhs);
bool operator!=(const OMDBase & lhs, const OMDBase & rhs);

String stripQuotes(const String & input);
String stripQuotes(const char   * input);


#endif
// Local Variables:
// mode:C++
// End:
