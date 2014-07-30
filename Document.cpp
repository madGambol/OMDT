
#include <stdlib.h>
#include <assert.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <list>
#include <string>
#include <set>

using namespace std;

#include <stdlib.h>

#include "OMDNode.h"
#include "OMDBase.h"
#include "Attribute.h"
#include "CBitfieldComponent.h"
#include "CBitfieldDataType.h"
#include "CComplexComponent.h"
#include "CComplexDataType.h"
#include "CDimension.h"
#include "CObjectModel.h"
#include "COMDocument.h"
#include "CRoutingSpace.h"
#include "CClass.h"
#include "CEnumeratedDataType.h"
#include "CEnumeration.h"
#include "Document.h"
#include "omdt.h"
#include "dcTypes.h"
#include "circAnalysis.h"

#define ASSERT(x) assert(x)
//#define ASSERT(x)

typedef OMDBase::NodeTypeEnum NodeTypeEnum;

string stripQuotes(const char * input)
{
    return stripQuotes( string( input ) );
}

int StringToInt(const string & str)
{
    return atoi(str.c_str());
}

void Document::internalInitialize(void)
{
    mTypes[ "string"             ] = new CSimpleType( "string",             0 );
    mTypes[ "char"               ] = new CSimpleType( "char",               1 );
    mTypes[ "unsigned char"      ] = new CSimpleType( "unsigned char",      1 );
    mTypes[ "octet"              ] = new CSimpleType( "octet",              1 );
    mTypes[ "any"                ] = new CSimpleType( "any",                1 ); // ??
    mTypes[ "short"              ] = new CSimpleType( "short",              2 );
    mTypes[ "unsigned short"     ] = new CSimpleType( "unsigned short",     2 );
    mTypes[ "boolean"            ] = new CSimpleType( "boolean",            4 );
    mTypes[ "int"                ] = new CSimpleType( "int",                4 );
    mTypes[ "unsigned int"       ] = new CSimpleType( "unsigned int",       4 );
    mTypes[ "long"               ] = new CSimpleType( "long",               4 );
    mTypes[ "unsigned long"      ] = new CSimpleType( "unsigned long",      4 );
    mTypes[ "float"              ] = new CSimpleType( "float",              4 );
    mTypes[ "double"             ] = new CSimpleType( "double",             8 );
    mTypes[ "long long"          ] = new CSimpleType( "long long",          8 );
    mTypes[ "unsigned long long" ] = new CSimpleType( "unsigned long long", 8 );
    mTypes[ "long double"        ] = new CSimpleType( "long double",       12 );
}

Document::Document(COMDocument * pOMDoc)
    :
    m_pOMDoc( pOMDoc )
{
    internalInitialize();
    
    Process( pOMDoc );
}

Document::~Document()
{
}

string Document::getFQObjName ( const OMDBase * pItem ) const
{
    string retVal;

    do
    {
        if (pItem == NULL) break;

        classID parentClassId = mParentIdToObjects.get( pItem );

        retVal = getFQObjName( mIdToObjects.getN( parentClassId ) );

        if (retVal.size()) retVal += string( "." );

        retVal += stripQuotes( mNameToObjects.get( pItem ) );

    } while (0);
    
    return retVal;
}

string Document::getFQIntName ( const OMDBase * pItem ) const
{
    string retVal;

    do
    {
        if (pItem == NULL) break;

        classID parentClassId = mParentIdToIa.get( pItem );

        retVal = getFQIntName( mIdToIa.getN( parentClassId ) );

        if (retVal.size()) retVal += string( "." );

        retVal += stripQuotes( mNameToIa.get( pItem ) );

    } while (0);
    
    return retVal;
}
    
list<string> Document::getFQObjNameList( const OMDBase * pItem ) const
{
    list<string> retVal;
    
    do
    {
        if (pItem == NULL) break;

        classID parentClassId = mParentIdToObjects.get( pItem );

        retVal = getFQIntNameList( mIdToObjects.getN( parentClassId ) );

        retVal.push_front( stripQuotes(mNameToObjects.get( pItem )) );

    } while (0);
    
    return retVal;
}

list<string> Document::getFQIntNameList( const OMDBase * pItem ) const
{
    list<string> retVal;
    
    do
    {
        if (pItem == NULL) break;

        classID parentClassId = mParentIdToIa.get( pItem );

        retVal = getFQIntNameList( mIdToIa.getN( parentClassId ) );

        retVal.push_front( stripQuotes(mNameToIa.get( pItem )) );

    } while (0);
    
    return retVal;
}

set< list<string> > Document::getFQObjNamesLists(void) const
{
    set< list<string> > retVal;
    
    CBaseMap< std::string, const OMDBase *>::const_TtoNiterator item = mNameToObjects.beginN();
    CBaseMap< std::string, const OMDBase *>::const_TtoNiterator last = mNameToObjects.endN();

    while (item != last)
    {
        list<string> fqNameList = getFQObjNameList( (*item).second );

        retVal.insert( fqNameList );
            
        ++item;
    }
    
    return retVal;
    
}

set< list<string> > Document::getFQIntNamesLists(void) const
{
    set< list<string> > retVal;
    
    CBaseMap< std::string, const OMDBase *>::const_TtoNiterator item = mNameToIa.beginN();
    CBaseMap< std::string, const OMDBase *>::const_TtoNiterator last = mNameToIa.endN();

    while (item != last)
    {
        list<string> fqNameList = getFQIntNameList( (*item).second );

        retVal.insert( fqNameList );
            
        ++item;
    }
    
    return retVal;
}

set<string>  Document::getFQObjNames(void) const
{
    set<string> retVal;
    
    //
    // generate a list of all objects

    CBaseMap< std::string, const OMDBase *>::const_TtoNiterator item = mNameToObjects.beginN();
    CBaseMap< std::string, const OMDBase *>::const_TtoNiterator last = mNameToObjects.endN();

    while (item != last)
    {
        string fqName = getFQObjName( (*item).second );
            
        //cout << "Object Name: " << fqName << endl;
            
        retVal.insert( fqName );
            
        ++item;
    }
    
    return retVal;
}

set<string>  Document::getFQIntNames(void) const
{
    set<string> retVal;
    
    //
    // generate a list of all interactions
        
    CBaseMap< std::string, const OMDBase *>::const_TtoNiterator item = mNameToIa.beginN();
    CBaseMap< std::string, const OMDBase *>::const_TtoNiterator last = mNameToIa.endN();

    while (item != last)
    {
        string fqName = getFQIntName( (*item).second );

        //cout << "Interaction Name: " << fqName << endl;
            
        retVal.insert( fqName );
            
        ++item;
    }
    
    return retVal;
}

set<pair<string,string> > Document::getFQObjNameAndNamePairs(void) const
{
    set<pair<string,string> > retVal;

    //
    // generate a list of all objects

    CBaseMap< std::string, const OMDBase *>::const_TtoNiterator item = mNameToObjects.beginN();
    CBaseMap< std::string, const OMDBase *>::const_TtoNiterator last = mNameToObjects.endN();

    while (item != last)
    {
        string fqName = getFQObjName( (*item).second );
            
        retVal.insert( pair<string,string>( (*item).first, fqName) );
            
        ++item;
    }

    return retVal;
}
    
set<pair<string,string> > Document::getFQIntNameAndNamePairs(void) const
{
    set<pair<string,string> > retVal;

    //
    // generate a list of all interactions
        
    CBaseMap< std::string, const OMDBase *>::const_TtoNiterator item = mNameToIa.beginN();
    CBaseMap< std::string, const OMDBase *>::const_TtoNiterator last = mNameToIa.endN();

    while (item != last)
    {
        string fqName = getFQIntName( (*item).second );

        retVal.insert( pair<string,string>( (*item).first, fqName) );
            
        ++item;
    }
    
    return retVal;
}

set<string> Document::getIaParamSet( const OMDBase * pObj ) const
{
    set<string> retVal;

    ConstOMDBaseToParamMMapIter item; // = mObjectToAttr.beginN();
    ConstOMDBaseToParamMMapIter last; // = mObjectToAttr.endN();

    mIaToParm.getRange( pObj, item, last );
    
    while (item != last)
    {
        retVal.insert( (*item).second->getName() );
        
        ++item;
    }

    return retVal;
}

StringPairSet Document::getObjectAttrTypePairSet( const OMDBase * pObj ) const
{
    StringPairSet  retVal;

    ConstOMDBaseToAttrMMapIter item; // = mObjectToAttr.beginN();
    ConstOMDBaseToAttrMMapIter last; // = mObjectToAttr.endN();

    mObjectToAttr.getRange( pObj, item, last );
    
    while (item != last)
    {
        const CAttribute * pAttr = (*item).second;
        
        StringPair temp( pAttr->getName(), pAttr->getDataType() );
        
        retVal.insert( temp );
        
        ++item;
    }

    return retVal;
}

StringPairSet Document::getClosedObjectAttrTypePairSet( const OMDBase * pObj) const
{
    StringPairSet retVal;

    while (pObj)
    {
        StringPairSet temp = getObjectAttrTypePairSet( pObj );

        retVal.insert( temp.begin(), temp.end() ); 

        classID parentClassId = mParentIdToObjects.get( pObj );
        
        pObj = mIdToObjects.getN( parentClassId );
    }

    return retVal;
}  


NameToAttrTypePairSetMap Document::getClosedObjectToAttrTypePairSets(void) const
{
    NameToAttrTypePairSetMap retVal;
    
    CBaseMap< std::string, const OMDBase *>::const_TtoNiterator item = mNameToObjects.beginN();
    CBaseMap< std::string, const OMDBase *>::const_TtoNiterator last = mNameToObjects.endN();

    while (item != last)
    {
        string      fqName  = getFQObjName    ( (*item).second );

        //StringPairSet objAttrSet = getClosedObjectAttrTypePairSet( (*item).second );
            
        retVal[ fqName ] = getClosedObjectAttrTypePairSet( (*item).second );
            
        ++item;
    }
    return retVal;
}

set<string> Document::getObjectAttrSet( const OMDBase * pObj ) const
{
    set<string> retVal;

    ConstOMDBaseToAttrMMapIter item; // = mObjectToAttr.beginN();
    ConstOMDBaseToAttrMMapIter last; // = mObjectToAttr.endN();

    mObjectToAttr.getRange( pObj, item, last );
    
    while (item != last)
    {
        const CAttribute * pAttr = (*item).second;
        
        retVal.insert( pAttr->getName() );
        
        ++item;
    }

    return retVal;
}

//
// return the full list of all the attributes including those of the
// parent classes in the set of attributes returned.
//
set<string> Document::getClosedObjectAttrSet( const OMDBase * pObj) const
{
    set<string> retVal;

    while (pObj)
    {
        set<string> temp = getObjectAttrSet( pObj );

        retVal.insert( temp.begin(), temp.end() ); 

        classID parentClassId = mParentIdToObjects.get( pObj );
        
        pObj = mIdToObjects.getN( parentClassId );
    }

    return retVal;
}  
  
StringToStringSetMap Document::getObjectToAttrSets(void) const
{
    StringToStringSetMap retVal;

    CBaseMap< std::string, const OMDBase *>::const_TtoNiterator item = mNameToObjects.beginN();
    CBaseMap< std::string, const OMDBase *>::const_TtoNiterator last = mNameToObjects.endN();

    while (item != last)
    {
        string      fqName  = getFQObjName    ( (*item).second );

        set<string> objAttrSet = getObjectAttrSet( (*item).second );
            
        retVal[ fqName ].insert( objAttrSet.begin(), objAttrSet.end() );
            
        ++item;
    }

    return retVal;
}

StringToStringSetMap Document::getClosedObjectToAttrSets(void) const
{
    StringToStringSetMap retVal;

    CBaseMap< string, const OMDBase *>::const_TtoNiterator item = mNameToObjects.beginN();
    CBaseMap< string, const OMDBase *>::const_TtoNiterator last = mNameToObjects.endN();

    while (item != last)
    {
        string fqName  = getFQObjName( (*item).second );

        StringSet objAttrSet = getClosedObjectAttrSet( (*item).second );
            
        retVal[ fqName ] = objAttrSet;
            
        ++item;
    }

    return retVal;    
}

//
// return the full list of all the parameter including those of the
// parent classes in the set of parameters returned.
//
StringSet Document::getClosedInteractionParamSet( const OMDBase * pObj ) const
{
    StringSet retVal;

    while (pObj)
    {
        StringSet temp = getIaParamSet( pObj );

        retVal.insert( temp.begin(), temp.end() );

        classID parentClassId = mParentIdToIa.get( pObj );
        
        pObj = mIdToIa.getN( parentClassId );
    }

    return retVal;	
}

StringPairSet Document::getInteractionParamTypePairSet( const OMDBase * pInt) const
{
    StringPairSet  retVal;

    ConstOMDBaseToParamMMapIter item; // = mObjectToAttr.beginN();
    ConstOMDBaseToParamMMapIter last; // = mObjectToAttr.endN();

    mIaToParm.getRange( pInt, item, last );
    
    while (item != last)
    {
        const CParameter * pParam = (*item).second;
        
        StringPair temp( pParam->getName(), pParam->getDataType() );
        
        retVal.insert( temp );
        
        ++item;
    }

    return retVal;
}

StringPairSet Document::getClosedInteractionParamTypePairSet( const OMDBase * pInt) const
{
    StringPairSet retVal;

    while (pInt)
    {
        StringPairSet temp = getInteractionParamTypePairSet( pInt );

        retVal.insert( temp.begin(), temp.end() ); 

        classID parentClassId = mParentIdToIa.get( pInt );
        
        pInt = mIdToIa.getN( parentClassId );
    }

    return retVal;
}

StringToStringSetMap Document::getClosedInteractToParamSets(void) const
{
	StringToStringSetMap retVal;

    CBaseMap< string, const OMDBase *>::const_TtoNiterator item = mNameToIa.beginN();
    CBaseMap< string, const OMDBase *>::const_TtoNiterator last = mNameToIa.endN();

    while (item != last)
    {
        string fqName  = getFQIntName( (*item).second );

        StringSet intParamSet = getClosedInteractionParamSet( (*item).second );
            
        retVal[ fqName ] = intParamSet;
            
        ++item;
    }

	return retVal;
}

StringToStringSetMap      Document::getInteractToParamSets      (void) const
{
    StringToStringSetMap retVal;
    
    cerr << "Document::getInteractToParamSets not implemented";
    
    return retVal;
}

NameToParamTypePairSetMap Document::getClosedInteractToParamTypePairSets(void) const
{
    NameToParamTypePairSetMap retVal;
    
    CBaseMap< std::string, const OMDBase *>::const_TtoNiterator item = mNameToIa.beginN();
    CBaseMap< std::string, const OMDBase *>::const_TtoNiterator last = mNameToIa.endN();

    while (item != last)
    {
        string      fqName  = getFQIntName    ( (*item).second );

        //StringPairSet objAttrSet = getClosedObjectAttrTypePairSet( (*item).second );
            
        retVal[ fqName ] = getClosedInteractionParamTypePairSet( (*item).second );
            
        ++item;
    }
    
    return retVal;
}


list<string> Document::getObjAttrNameList(const string & objName ) const
{
    list<string> retVal;
    
    // pick up all the attributes for an object definition
    const OMDBase * pBase = NULL;

    string leafName = objName;

    size_t pos = objName.rfind('.');

    if (pos < string::npos)
    {
        leafName = objName.substr(pos+1);
    }

    do
    {
        std::multimap< std::string, const OMDBase *>::const_iterator itemName;
        std::multimap< std::string, const OMDBase *>::const_iterator lastName;

        mNameToObjects.getRange( leafName, itemName, lastName);

        if (itemName == lastName) break;

        pBase = (*itemName).second;

        if (pBase == NULL) break;

        std::multimap<const OMDBase *, const CAttribute *>::const_iterator itemAttr;
        std::multimap<const OMDBase *, const CAttribute *>::const_iterator lastAttr;
        
        mObjectToAttr.getRange( pBase, itemAttr, lastAttr );

        while (itemAttr != lastAttr)
        {
            retVal.push_back( (*itemAttr).second->getName() );
            
            ++itemAttr;
        }

    } while (0);

    return retVal;
}       

int Document::Process ( const COMDocument         * pItem )
{ 
    // CContextScope local( mContext, pItem );

    pItem->Execute( this );

    return 0;
}

int Document::Process ( const CObjectModel        * pItem )
{
    // CContextScope local( mContext, pItem );

    pItem->Execute( this );

    return 0;
}

int Document::Process ( const CRoutingSpace       * pItem )
{
    //CContextScope local( mContext, pItem );

    //mRoutingSpace.push_back( pItem );

    pItem->Execute( this );

    return 0;
}

int Document::Process ( const CDimension          * pItem )
{
    pItem->Execute( this );
	return 0;
}

int Document::Process ( const CEnumeratedDataType * pItem )
{
    // CContextScope local( mContext, pItem );

    //mEnumeratedDataTypes.push_back( pItem );

//    const OMDBase * pBase = pItem->getChild("Name");

//    const CAttributeValuePair * pAVP = 
//        dynamic_cast<const CAttributeValuePair *>( pBase );

    //     const char * pName = "???";
  
    //     if (pAVP)
    //     {
    //         pName = stripQuotes(pAVP->getValue()).c_str();
    //     }

    //cerr << "CEnumeration : name = \"" << pName << "\"" << endl;

    pItem->Execute( this );

    return 0;
}

int Document::Process ( const CEnumeration        * pItem )
{
    const CEnumeratedDataType * pEDT =
        dynamic_cast<const CEnumeratedDataType *>( pItem->getParent() );

    if (pEDT)
    {
        // add info about this enumeration
    }
    
    return 0;
}

int Document::Process ( const CComplexDataType    * pItem )
{
    pItem->Execute( this );

    return 0;
}

int Document::Process ( const CBitfieldDataType   * pItem )
{
    pItem->Execute( this );

    return 0;
}

int Document::Process ( const CComplexComponent   * pItem )
{
    // CContextScope local( mContext, pItem );

    const CComplexDataType * pCDT = 
        dynamic_cast<const CComplexDataType *>( pItem->getParent() );

    if (pCDT) mCDTtoCC.add( pCDT, pItem );

    pItem->Execute( this );

    return 0;
}

int Document::Process ( const CBitfieldComponent  * pItem )
{
    // CContextScope local( mContext, pItem );

    const CBitfieldDataType * pBFDT = 
        dynamic_cast<const CBitfieldDataType *>( pItem->getParent() );

    if (!pBFDT) 
	{
		// error: didn't find a BitfieldComponent under BitfieldDataType
		return -1;
	}

	mBFDTtoBFC.add( pBFDT, pItem );

    pBFDT->Execute( this );

    return 0;
}

int Document::Process ( const CAttributeValuePair * pItem )
{
    //const OMDBase * pThis        = (const OMDBase *)pItem;

    const OMDBase * pParent        = pItem  ->getParent();
    
    NodeTypeEnum    itemNodeType   = pItem  ->getNodeType();
    NodeTypeEnum    parentNodeType = pParent->getNodeType();
    
    const string    itemValue      = stripQuotes(pItem->getValue());
    const string    itemName       = stripQuotes(pItem->getName());

    switch (parentNodeType)
    {
    case OPEN_eObjectModel:
        // add to front matter
        //mFrontMatter.push_back( pItem );
        break;

    case OPEN_eComplexDataType:
        //
        // add name to multimap for CDT's
        //
        switch (itemNodeType)
        { 
        case OPEN_eName:
            {
                const CComplexDataType * pCDT = dynamic_cast<const CComplexDataType *>( pParent );

                mNameToTypes.add( itemValue, pParent);
                mNameToCDT.  add( itemValue, pCDT   );
                //
                // Have to process the mTypes into the CTypeDescr later
                //
            }
            break;
            
        default:
            break;
        }
        break;

    case OPEN_eEnumeratedDataType:
        //    
        // add name to multimap for EDT's
        //
        switch (itemNodeType)
        {
        case OPEN_eName:
            {
                mNameToTypes.add( itemValue, pParent );

                const CEnumeratedDataType * pEnum = 
                    dynamic_cast<const CEnumeratedDataType *>( pParent );

                if (mNameToEnumTypes.add( itemValue, pEnum ))
                {
                    // report multiple entries
                    cerr << "Redefinition of \"" << itemName << "\"" << endl;
                }

                //
                // Have to process the mTypes into the CTypeDescr later
                //
            }
            break;

        case OPEN_eEnumeration:
            {
                cerr << "OPEN_eEnumeration" << endl;
            }
            break;
        default:
            break;
        }
        break;

    case OPEN_eClass:
        switch (itemNodeType)
        {
        case OPEN_eName:
            mNameToObjects.add( itemValue, pParent );
            break;

        case OPEN_eID:
            mIdToObjects.add( StringToInt(itemValue), pParent );
            break;

        case OPEN_eSuperClass:
            mParentIdToObjects.add( StringToInt(itemValue), pParent );
            break;

        default:
            break;
        }
        break;

    case OPEN_eInteraction:
        switch (itemNodeType)
        {
        case OPEN_eName:
            mNameToIa.add( itemValue, pParent );
            break;
        case OPEN_eID:
            mIdToIa.add( StringToInt(itemValue), pParent );
            break;
        case OPEN_eSuperInteraction:
            mParentIdToIa.add( StringToInt(itemValue), pParent );
            break;
        default:
            break;
        }
        break;

    case OPEN_eAttribute:
        switch (itemNodeType)
        {
        case OPEN_eName:
            {
                mNameToAttr.add( itemValue, pParent );
            }        
            break;
            
        case OPEN_eDataType:
            {
                const CAttribute * pAttr = dynamic_cast<const CAttribute *>( pParent );
                mDataTypeToAttr.add( itemValue, pAttr );
            }
            break;
        case OPEN_eCardinality:
           {
	     //const CAttribute * pAttr = dynamic_cast<const CAttribute *>( pParent );
	     //mCardinalityToAttr.add( itemValue, pAttr );
           }
           break;
        default:
            break;
        }
        break;

    case OPEN_eParameter:
        switch (itemNodeType)
        {
        case OPEN_eName:
            {
                mNameToParm.add( itemValue, pParent );
            }
            break;
            
        case OPEN_eDataType:
            {
                const CParameter * pParm = dynamic_cast<const CParameter *>( pParent );
                mDataTypeToParm.add( itemValue, pParm );
            }
            break;
            
        default:
            break;
        }
        break;

    case OPEN_eRoutingSpace:
        if (itemNodeType == OPEN_eName)
        {
            mRoutingSpaceName = itemValue;
        }    
        break;

    case OPEN_eComplexComponent:
        switch (itemNodeType)
        {
        case OPEN_eDataType:
            {
                const CComplexComponent * pCC = dynamic_cast<const CComplexComponent *>( pParent );
                mDataTypeToCC.add( itemValue, pCC );
            }
            break;
            
        case OPEN_eFieldName:
            {
                const CComplexComponent * pCC = dynamic_cast<const CComplexComponent *>( pParent );

                mFieldNameToCC.add( itemValue, pCC);
            }
            break;
            
        default:
            break;
        }
        break;

    case OPEN_eDimension:
		switch (itemNodeType)
		{
        case OPEN_eName:
			mRoutingSpace.push_back( itemValue );
			break;
		default:
			break;
		}
		break;

    case OPEN_eEnumeration:
    case OPEN_eNote:
    default:
        break;
    }

    return 0;
}

String Document::getCardinality( const OMDBase * pObj ) const
{
    String retVal;
    
    
    
    return retVal;
}

int Document::Process ( const CClass              * pItem )
{
    // CContextScope local( mContext, pItem );

    //mObjects.push_back( pItem );

    pItem->Execute( this );

    return 0;
}

int Document::Process ( const CAttribute          * pItem )
{
    // CContextScope local( mContext, pItem );

    const OMDBase * pParent = pItem->getParent();

    mObjectToAttr.add( pParent, pItem );

    pItem->Execute( this );

    return 0;
}

int Document::Process ( const CInteraction        * pItem )
{
    // CContextScope local( mContext, pItem );

    //mInteractions.push_back( pItem );

    pItem->Execute( this );

    return 0;
}

int Document::Process ( const CParameter          * pItem )
{
    // CContextScope local( mContext, pItem );

    const OMDBase * pParent = pItem->getParent();

    mIaToParm.add( pParent, pItem );

    pItem->Execute( this );

    return 0;
}

int Document::Process ( const CNote               * pItem )
{
    // CContextScope local( mContext, pItem );

    //mNotes.push_back( pItem );

    pItem->Execute( this );

    return 0;
}

int Document::Process ( const CNoteList           * pItem )
{
    //mAllNoteRefs.Add( pItem );

    pItem->Execute( this );

    return 0;
}

int Document::Process ( const CNoteText           * pItem )
{
    pItem->Execute( this );

    return 0;
}

int Document::Process ( const CAttributeName      * pItem )
{
    pItem->Execute( this );

    return 0;
}

int Document::Process ( const CAttributeValue     * pItem )
{
    return 0;
}

//
// ------------ shouldn't be called --------------------------
//
int Document::Process ( const OMDBase             * pItem )
{
    return 0;
}

int Document::Process ( const OMDNode             * pItem )
{
    return 0;
}

int Document::Process ( const Singleton           * pItem )
{
    return 0;
}

int Document::Process ( const CAttributeList      * pItem )
{
    cerr << "Process ( const CAttributeList * pItem) called " << endl;
  
    const OMDBase * pParent        = pItem  ->getParent();
    //NodeTypeEnum    itemNodeType   = pItem  ->getNodeType();
    NodeTypeEnum    parentNodeType = pParent->getNodeType();
    OMDBase       * pBase          = pItem  ->getChild(0);
    OMDNode       * pNode          = dynamic_cast<OMDNode *>( pBase );

    switch (parentNodeType)
    {
    case OPEN_eEnumeration:
        cerr << "OPEN_eEnumeration with name = \"" << pNode->getType() << "\"" << endl;
        break;
    default:
        break;
    }  
    return 0;
}

// ----------- end shouldn't be called -----------------------

const char * Document::getSpaceName( int indx ) const
{
	const char * pRetVal = "";

	if (indx < mRoutingSpace.size())
	{
		pRetVal = mRoutingSpace[indx].c_str();
	}

	return pRetVal;
}

ostream & Document::print( ostream & out ) const
{
    stringstream str;
    
    str << "Document::print() const" << endl;
    
    out << str.str();
    
    return out;
}

//stringstream & Document::print( stringstream & out )
//{
//    stringstream outStr;
//
//    out << outStr.str();
//
//    return out;
//}

bool isName( const OMDBase * pItem)
{
    return pItem->getNodeType() == OPEN_eName;
}


bool operator==(const OMDBase & lhs, const OMDBase & rhs)
{
    return lhs.equal( rhs );
}

bool operator!=(const OMDBase & lhs, const OMDBase & rhs)
{
    return lhs.notEqual( rhs );
}
