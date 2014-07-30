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
#include "check.h"

#include "omdt.h"
#include "dcTypes.h"
#include "circAnalysis.h"

#define ASSERT(x) assert(x)
//#define ASSERT(x)

typedef OMDBase::NodeTypeEnum NodeTypeEnum;

Check::Check( COMDocument * pOMDoc )
    :
    Document          ( pOMDoc    ),
    //mAllNoteRefs    ( 0         ),
    mSummaryLineNumber( 1         ),
    mErrorReport      ( "Errors"  ),
    mWarningReport    ( "Warnings"),
    mSummary          ( "Summary" )
{
    summarizeFrontMatter        ();
    summarizeRoutingSpace       ();
    summarizeEnumeratedDataTypes();
    summarizeComplexDataTypes   ();
    summarizeClasses            ();
    summarizeInteractions       ();
    summarizeCardinality        ();
    summarizeVariantRecords     ();
    summarizeNotes              ();

    summarizeAll                ();
}

Check::~Check(void)
{
    //
}

void Check::summarizeAll(void)
{
}

size_t Check::getErrorCount  (void) const
{
    return mErrorReport.size();
}

size_t Check::getWarningCount(void) const
{
    return mWarningReport.size();
}


// IOStream operations

ostream & Check::print( ostream & out ) const
{
    out << "Check::print() const" << endl;
    
    stringstream outStr;

    outStr << mErrorReport   << endl;
    outStr << mWarningReport << endl;
    outStr << mSummary       << endl;

    outStr << ends;

    out << outStr.str();

    return out;
}

//stringstream & Check::print( stringstream & out )
//{
//    stringstream outStr;
//
//    outStr << mErrorReport   << endl;
//    outStr << mWarningReport << endl;
//    outStr << mSummary       << endl;
//
//    outStr << ends;
//
//    out << outStr.str();
//
//    return out;
//}
//
// ----------------- Checks -------------------------

void Check::summarizeFrontMatter     (void)
{
    // for the required parts
    
    mSummary.addTBS( mSummaryLineNumber++, "Front Matter check");
}

void Check::summarizeRoutingSpace    (void)
{
    mSummary.addTBS( mSummaryLineNumber++, "Routing Space check");
}

void Check::issueDupEnumMsg( const CEnumeratedDataType * pItem, 
                             const OMDBase * pDupItem, 
                             size_t & count)
{
    issueDupMsg( mNameToEnumTypes.get( pItem ),
                 "EnumeratedDataType",
                 (const OMDBase *)pItem,
                 pDupItem,
                 count
               );
}

void Check::checkRequiredEnum()
{
    mSummary.addTBS( mSummaryLineNumber++, "Check of required EnumeratedType items");
}

void Check::checkDupEnum()
{
    ConstStringToEdtBaseMapIter item = mNameToEnumTypes.beginN();
    ConstStringToEdtBaseMapIter last = mNameToEnumTypes.endN();

    while (item != last)
    {
        const CEnumeratedDataType * pItem    = (*item).second;
        string                      name     = (*item).first;
        size_t                      dupCount = 0;

        ++item;
        ConstStringToEdtBaseMapIter itemInner = item;
 
        while (itemInner != last)
        {
            if (name != (*itemInner).first) break; // we're done

            const OMDBase * pDupItem = (*itemInner).second;

            issueDupEnumMsg( pItem, pDupItem, dupCount );

            ++itemInner;
        }
    }
}

void Check::summarizeEnumeratedDataTypes(void)
{
    checkRequiredEnum();
    checkDupEnum();

    // other checks tbs
    mSummary.addApplied(mSummaryLineNumber++, "EnumeratedDataTypes Check");
}

void Check::checkRequiredCDT()
{
    mSummary.addTBS( mSummaryLineNumber++, "Check of required ComplexDataType items");
}

void Check::checkDupCDT()
{
    ConstNameToCdtMMapIter item = mNameToCDT.beginN();
    ConstNameToCdtMMapIter last = mNameToCDT.endN();

    while (item != last)
    {
        const CComplexDataType * pCDT     = (*item).second;
        string                   name     = (*item).first;
        size_t                   dupCount = 0;

        ++item;

        while (item != last)
        {
            if (name != (*item).first) break; // we're done (this is a multimap, remember)

            const OMDBase * pDupItem = (*item).second;

            issueDupCDTMsg( pCDT, pDupItem, dupCount );

            ++item; // output all the dups
        }
    }
}

void Check::issueDupCDTMsg( const CComplexDataType * pItem, 
                            const OMDBase          * pDupItem, 
                            size_t & count)
{
    issueDupMsg( mNameToCDT.get( pItem ),
                 "ComplexDataType",
                 (const OMDBase *)pItem,
                 pDupItem,
                 count
               );
}

void Check::checkMissingObjectIDs()
{
    constIdOMDBIter item = mParentIdToObjects.beginN();
    constIdOMDBIter last = mParentIdToObjects.endN();
  
    while (item != last)
    {
        const OMDBase * pNode = (*item).second;
        classID         id    = (*item).first;

        ++item;

        const OMDBase * pParent = mIdToObjects.getN( id ); // look up parent

        if (pParent) continue; // found one

        //
        // didn't find one, issue a message
        // 
        issueMissingIDMsg( id, pNode );
    }
}

void Check::issueMissingIDMsg(classID id, 
                              const OMDBase * pItem
                             )
{
    const Loc  * pItemLoc  = pItem->getLocation();
    size_t       thisLine  = pItemLoc->first_line;
    const char * pKind     = ((pItem->getNodeType() == OPEN_eClass)?
                              "Object":"Interaction"
                             );
    stringstream msg;

    msg << mErrorReport.getLinePrefix(thisLine, "Error");

    msg << pKind << " ID (" << id << ") is missing" << ends;

    mErrorReport.addAtLine( thisLine, msg.str() );
}

void Check::issueMissingAttrTypeMsg( const OMDBase * pAttr,
                                     const string  & dataType,
                                     const OMDBase * pDataType
                                   )
{
    const Loc * pItemLoc  = pDataType->getLocation();
    size_t      thisLine  = pItemLoc->first_line;

    stringstream msg;

    msg << mErrorReport.getLinePrefix(thisLine, "Error")
        << "Missing Attribute Datatype " 
        << dataType 
        << " ";

    string attrName = mNameToAttr.get( pAttr );

    msg << attrName;

    msg << ends;

    mErrorReport.addAtLine( thisLine, msg.str() );
}

void Check::checkMissingAttrType()
{
    //
    // loop through all datatypes
    //
    ConstOMDBaseToAttrMMapIter objItem = mObjectToAttr.beginN();
    ConstOMDBaseToAttrMMapIter objLast = mObjectToAttr.endN();

    while (objItem != objLast)
    {
        const CAttribute * pAttribute   = (*objItem).second;
        //
        const string       dataTypeName = mDataTypeToAttr.get( pAttribute );

        constTypeDescrIter simple       = mTypes.find( stripQuotes(dataTypeName) );
    
        ++objItem;

        if (simple != mTypes.end()) continue; // we found a simple type

        constOMDBIter nonSimple = mNameToTypes.getLowerN( dataTypeName );

        //
        // if the type is present, no need to flag anything
        //
        if (nonSimple != mNameToTypes.getUpperN( dataTypeName ) ) continue;  

        //
        // Issue the missing type message
        //
        const OMDBase * pObjNode = pAttribute->getParent();

        issueMissingAttrTypeMsg( pObjNode, dataTypeName, pAttribute );
    }
}

void Check::checkDupAttr(void)
{
    //
    // loop through all the Objects
    //
    ConstOMDBaseToAttrMMapIter objItem = mObjectToAttr.beginN();
    ConstOMDBaseToAttrMMapIter objLast = mObjectToAttr.endN();

    while (objItem != objLast)
    {
        const OMDBase    * pObject    = (*objItem).first;
        const CAttribute * pAttribute = (*objItem).second;
        const OMDBase    * pItem      = (const OMDBase *)pAttribute;

        size_t dupCount = 0;

        string attrName = mNameToAttr.get( pItem );

        ConstOMDBaseToAttrMMapIter objInstLast = mObjectToAttr.getUpperN( pObject );

        if (objItem == objInstLast) continue;

        ++objItem;

        ConstOMDBaseToAttrMMapIter secondObj = objItem;

        while (secondObj != objInstLast)
        {
            const OMDBase * pDupItem = (const OMDBase *)(*secondObj).second;

            ++secondObj;

            string nextAttrName = mNameToAttr.get( pDupItem );

            if (attrName != nextAttrName) continue; // we're done on this check

            issueDupAttrMsg( pItem, pDupItem, dupCount );
        }
    }
}

void Check::addSummaryLine(const string & line)
{
    mSummary.insert( pair<size_t,string>(mSummaryLineNumber, line));

    mSummaryLineNumber++;
}

void Check::issueDupMsg( const string    typeName,   // Name field for this
                         const char    * pTypeStr,   // Name of the type
                         const OMDBase * pItem,      // the original item
                         const OMDBase * pDupItem,   // the duplicate item
                         size_t & count
                       )
{
    const Loc  * pDupItemLoc  = pDupItem->getLocation();
    size_t       thisLine     = pDupItemLoc->first_line;

    const Loc  * pOrigItemLoc = pItem->getLocation();

    stringstream msg;

    msg << mErrorReport.getLinePrefix(thisLine, "Error");

    if (count == 3) // issue "other duplicates exist"
    {
        msg << "other duplicates of ";
        pOrigItemLoc->print(msg);
        msg << "exist";
    }
    else if (count > 3)
    {
        // ignore after 3
    }
    else if (count < 3)
    {
        msg << "Duplicate ";
        msg << pTypeStr;
        msg << " definition: ";
        msg << typeName;
        msg << " originally declared at ";
#warning printing Loc
        //msg << *pOrigItemLoc;

        bool bDiffer = (*pItem) != (*pDupItem);

        msg << " with " << (bDiffer?"a different":"an identical") << " definition";
    }

    msg << std::ends;

    if (count < 3) mErrorReport.addAtLine( thisLine, msg.str() );

    count++;
  
}
  
void Check::checkMissingCDT()
{
    // iterate through the ComplexComponents looking for the definition
    // of the type in either the simple types (mTypes) or the CDT (mNameToCDT)
    //

    ConstNametoCcMMapIter item = mDataTypeToCC.beginN();
    ConstNametoCcMMapIter last = mDataTypeToCC.endN();

    while (item != last)
    {
        string dataTypeName             = (*item).first;   // complex component datatype
        const CComplexComponent * pItem = (*item).second;  // complex component

        ++item;

        constTypeDescrIter simple = mTypes.find( stripQuotes(dataTypeName) );

        if (simple != mTypes.end()) continue; // if found, go on

        ConstStringToEdtBaseMapIter enumType = mNameToEnumTypes.getLowerN( dataTypeName );

        if (enumType != mNameToEnumTypes.getUpperN( dataTypeName )) continue;

        ConstNameToCdtMMapIter nonSimple = mNameToCDT.getLowerN( dataTypeName );

        //
        // if the type is present, no need to flag anything
        //
        if (nonSimple != mNameToCDT.getUpperN( dataTypeName ) ) continue;  

        issueMissingCDTMsg( pItem, dataTypeName );
    }
}

void Check::issueMissingCDTMsg( const CComplexComponent * pCC,
                                const string            & dataType
                              )
{
    const Loc * pCCLoc     = pCC->getLocation();
    size_t      thisLine   = pCCLoc->first_line;

    //const CComplexComponent * pCC  = dynamic_cast<const CComplexComponent *>( pItem );
    const CComplexDataType  * pCDT = mCDTtoCC.get( pCC );

    ASSERT( pCDT );
    ASSERT( pCC  );

    string complexComp = mFieldNameToCC.get( pCC );
    string complexDT   = mNameToCDT.get( pCDT );

    stringstream msg;

    msg << mErrorReport.getLinePrefix(thisLine, "Error");

    msg << "Missing DataType "
        << dataType 
        << " in ComplexComponent " 
        << complexComp 
        << " of " 
        << complexDT 
        << ends;

    mErrorReport.addAtLine( thisLine, msg.str() );
}

void Check::checkLoopCDT()
{
    ConstNameToCdtMMapIter item = mNameToCDT.beginN();
    ConstNameToCdtMMapIter last = mNameToCDT.endN();

    CCircularityAnalysis * pCA = CCircularityAnalysis::factory();

    while (item != last)
    {
        const CComplexDataType  * pCDT = (*item).second;

        ConstCdtToCcMMapIter ccItem = mCDTtoCC.getLowerN( pCDT );
        ConstCdtToCcMMapIter ccLast = mCDTtoCC.getUpperN( pCDT );
    
        while (ccItem != ccLast)
        {
            const CComplexComponent * pCC = (*ccItem).second;
            string dataTypeName = mDataTypeToCC.get( pCC );

            pCA->addPair( (*item).first, dataTypeName );

            ++ccItem;
        }
        ++item;
    }

    pCA->reportResults( mErrorReport );
}

void Check::checkLoopObjects()
{
    CCircularityAnalysis * pCA = CCircularityAnalysis::factory();
  
    // pick up the ID/SuperID pairs
    constIdOMDBIter item = mIdToObjects.beginN();
    constIdOMDBIter last = mIdToObjects.endN();

    while (item != last)
    {
        classID           id = (*item).first;
        const OMDBase * pObj = (*item).second;
        classID      superId = mParentIdToObjects.get( pObj );
    
        stringstream idStr;
        stringstream superIdStr;

        idStr      << id      << ends;
        superIdStr << superId << ends;

        pCA->addPair( idStr.str(), superIdStr.str() );

        ++item;
    }

    pCA->reportResults( mErrorReport );
}

void Check::checkLoopInteractions()
{
    CCircularityAnalysis * pCA = CCircularityAnalysis::factory();
  
    // pick up the ID/SuperID pairs
    constIdOMDBIter item = mIdToIa.beginN();
    constIdOMDBIter last = mIdToIa.endN();

    while (item != last)
    {
        classID           id = (*item).first;
        const OMDBase * pObj = (*item).second;
        classID      superId = mParentIdToIa.get( pObj );
    
        stringstream idStr;
        stringstream superIdStr;

        idStr      << id      << ends;
        superIdStr << superId << ends;

        pCA->addPair( idStr.str(), superIdStr.str() );

        ++item;
    }

    pCA->reportResults( mErrorReport );
}

set<const CComplexDataType *> Document::getCdtSet(void)
{
    set<const CComplexDataType *> retVal;

    // examine mNameToCDT to get all the CDT's

    ConstNameToCdtMMapIter item = mNameToCDT.beginN();
    ConstNameToCdtMMapIter last = mNameToCDT.endN();

    while (item != last)
    {
        cout << "in Check::getCdtSet(); adding \"" << (*item).first << "\"" << endl;
        
        retVal.insert( (*item).second );
        
        ++item;
    }

    return retVal;
}

void Check::checkOrphanedCDT()
{
    // for all CDT, if there is no CDT for which a CDT is a complex component,
    // then that CDT must be an attribute or parameter type.
    // For any CDT that is not a CC or attribute or Paramter, it is an orphane.
    //
    set<const CComplexDataType *> allCDT = getCdtSet();

    ConstCdtToCcMMapIter cdtItem = mCDTtoCC.beginN();
    ConstCdtToCcMMapIter cdtLast = mCDTtoCC.endN();

    //
    // Eliminate all CDT that are referenced as a complex component
    //
    while (cdtItem != cdtLast)
    {
        const CComplexDataType  * pCdt = (*cdtItem).first;

        ++cdtItem;

        allCDT.erase( pCdt );
    }

    //
    // Eliminate all CDT's that are referred to as data types for Attributes
    //
    // TBS
    // 
    //constNameAttrIter nameIter = mDataTypeToAttr;
    //constNameAttrIter nameLast = mDataTypeToAttr;

    //
    // Eliminate all CDT's that are referred to as data types for Parameters
    //
    // TBS
    
}

// void Check::doCircularityAnalysis( list<CDTcount> & cdtCountList )
// {
// }

// bool Check::dependsOnlyOn(const CComplexDataType * pCDT, 
//                                const dependsMap       & noLoops) const
// {
//   constCDTCCIter item = mCDTtoCC.getLowerN( pCDT );
//   constCDTCCIter last = mCDTtoCC.getUpperN( pCDT );

//   while (item != last)
//   {
//     const CComplexDataType  * pInnerCDT = (*item).first;
//     const CComplexComponent * pCC       = (*item).second;

//     string dataTypeName = mDataTypeToCC.get( pCC ); // get the datatype name
//     string noQuotes     = stripQuotes(dataTypeName);

//     ++item;

//     //cerr << "Inner check : simpletypes : " << dataTypeName << endl;

//     constTypeDescrIter simple = mTypes.find( noQuotes );
    
//     if (simple != mTypes.end()) continue; // we found a simple type

//     // cerr << "Inner check : enumTypes   : " << dataTypeName << endl;

//     constEnumIter enumType = mNameToEnumTypes.getLowerN( dataTypeName );

//     if (enumType != mNameToEnumTypes.getUpperN( dataTypeName )) 
//     {
//       // cerr << "Inner check : quitting loop on enum case" << endl;

//       continue; // its an enum
//     }

//     // cerr << "Inner check : cdtTypes    : " << dataTypeName << endl;

//     dependsMapIter noLoopsItem = noLoops.find( pInnerCDT );

//     if (noLoopsItem != noLoops.end()) continue; // found in no loops

//     // cerr << "Depends on " << dataTypeName << endl;
  
//     return false; // 
//   }

//   return true;
// }

void Check::summarizeComplexDataTypes(void)
{
    checkRequiredCDT();
    checkDupCDT();
    checkMissingCDT();
    checkLoopCDT();

    mSummary.addApplied(mSummaryLineNumber++, "ComplexDataType Check");
}

void Check::checkMissingParmType()
{
    //
    // loop through all datatypes
    //
    ConstOMDBaseToParamMMapIter intItem = mIaToParm.beginN();
    ConstOMDBaseToParamMMapIter intLast = mIaToParm.endN();

    while (intItem != intLast)
    {
        //const OMDBase    * pInteraction = (*intItem).first;
        const CParameter * pParameter   = (*intItem).second;

        //
        const string       dataTypeName = mDataTypeToParm.get( pParameter );

        constTypeDescrIter simple = mTypes.find( stripQuotes(dataTypeName) );

        ++intItem;

        if (simple != mTypes.end()) continue; // we found a simple type

        constOMDBIter nonSimple = mNameToTypes.getLowerN( dataTypeName );

        //
        // if the type is present, no need to flag anything
        //
        if (nonSimple != mNameToTypes.getUpperN( dataTypeName ) ) continue;  

        //
        // Issue the missing type message
        //
        const OMDBase * pIntNode = pParameter->getParent();

        issueMissingParmTypeMsg( pIntNode, dataTypeName, pParameter );
    }
}

void Check::checkMissingIaIDs(void)
{
    constIdOMDBIter item = mParentIdToIa.beginN();
    constIdOMDBIter last = mParentIdToIa.endN();

    while (item != last)
    {
        const OMDBase * pNode = (*item).second;
        classID         id    = (*item).first;

        ++item;

        const OMDBase * pParent = mIdToIa.getN( id ); // look up parent

        if (pParent) continue; // found one

        //
        // didn't find one, issue a message
        // 
        issueMissingIDMsg( id, pNode );
    }
}

void Check::checkDupParm(void)
{
    //
    // loop through all the Interactions
    //
    ConstOMDBaseToParamMMapIter intItem = mIaToParm.beginN();
    ConstOMDBaseToParamMMapIter intLast = mIaToParm.endN();

    while (intItem != intLast)
    {
        const OMDBase    * pInteraction = (*intItem).first;
        const CParameter * pParameter   = (*intItem).second;
        const OMDBase    * pItem        = (const OMDBase *)pParameter;

        size_t dupCount = 0;

        string parmName = mNameToParm.get( pItem );

        ConstOMDBaseToParamMMapIter intInstLast = mIaToParm.getUpperN( pInteraction );

        if (intItem == intInstLast) continue;

        ++intItem;

        ConstOMDBaseToParamMMapIter secondInt = intItem;

        while (secondInt != intInstLast)
        {
            const OMDBase * pDupItem = (const OMDBase *)(*secondInt).second;

            ++secondInt;

            string nextParmName = mNameToParm.get( pDupItem );

            if (parmName != nextParmName) continue; // we're done on this check

            issueDupParmMsg( pItem, pDupItem, dupCount );
        }
    }
}

void Check::issueMissingParmTypeMsg( const OMDBase      * pParm,
                                     const std::string  & dataType,
                                     const OMDBase      * pDataType
                                   )
{
    const Loc * pItemLoc = pDataType->getLocation();
    size_t      thisLine = pItemLoc->first_line;

    stringstream msg;

    msg << mErrorReport.getLinePrefix(thisLine, "Error") << ends;

    msg << "Missing Parameter Datatype " << dataType << " ";

    string parmName = mNameToParm.get( pParm );

    msg << parmName;

    msg << ends;

    mErrorReport.addAtLine(thisLine, msg.str() );
}

void Check::issueDupAttrMsg( const OMDBase * pItem, 
                             const OMDBase * pDupItem, 
                             size_t & count
                           ) 
{
    issueDupMsg( mNameToAttr.get( pItem ),
                 "Attribute",
                 pItem,
                 pDupItem,
                 count
               );
}


void Check::issueDupParmMsg  ( const OMDBase * pItem, 
                               const OMDBase * pDupItem, 
                               size_t & count
                             )
{
    issueDupMsg( mNameToParm.get( pItem ),
                 "Parameter",
                 pItem,
                 pDupItem,
                 count
               );
}

void Check::checkRequiredObjectProperties()
{
    mSummary.addTBS( mSummaryLineNumber++, "Check of required object property items");
}

void Check::summarizeClasses         (void)
{
    checkRequiredObjectProperties();
    checkDupAttr();
    checkMissingAttrType();
    checkMissingObjectIDs();
    checkLoopObjects();

    mSummary.addApplied(mSummaryLineNumber++, "Objects Check");
}

void Check::checkRequiredIaProperties()
{
    mSummary.addTBS( mSummaryLineNumber++, "Check of required interaction property items");
}

void Check::summarizeInteractions    (void)
{
    checkRequiredIaProperties();
    checkDupParm();
    checkMissingParmType();
    checkMissingIaIDs();
    checkLoopInteractions();

    mSummary.addApplied(mSummaryLineNumber++, "Interactions Check");
}

void Check::checkCardinalitySyntax   ()   // syntax checking
{
    // run through all cardinality expressions: CC, attribute, parameter

    // CC:

    {
        StrToCcConstIter item = mDataTypeToCC.beginN();
        StrToCcConstIter last = mDataTypeToCC.endN();

        while (item != last)
        {
            string                    dataType = (*item).first;
            const CComplexComponent * pCC      = (*item).second;

            stringstream str;

            str << *pCC;

            if (str.str().size())
            {
                cerr << "[[" << str.str() << "]]" << endl;
            }
            
            ++item;
        }
    }

    // attributes

    {
    }

    // parameters

    {
    }

}

void Check::checkCardinalitySemantics()   // proper definition, indefinite use
{
}

void Check::summarizeCardinality(void) // check expressions, indefinite use
{
    checkCardinalitySyntax();
    checkCardinalitySemantics();

    mSummary.addApplied(mSummaryLineNumber++, "Cardinality Check");
}

void Check::summarizeVariantRecords(void)
{
    mSummary.addTBS(mSummaryLineNumber++, "Variant Record Check");
}

void Check::summarizeNotes           (void)
{
    mSummary.addTBS( mSummaryLineNumber++, "Note Items Check");
}

