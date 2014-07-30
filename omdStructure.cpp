
#include <string.h>

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <list>
#include <map>
#include <set>

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <netinet/in.h>

#include "cardinality.hpp"

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
#include "OMDTask.h"
#include "singleton.h"

using namespace std;

#include "OMDCardinalityParser.h"
#include "omdStructure.h"
#include "convert.h"

using namespace std;

typedef unsigned int uint32;
typedef int int32;

string stripQuotes(const string & input);

OMDCardinalityParser & getParser(void)
{
    static OMDCardinalityParser cardinalityParser;

    return cardinalityParser;
}

COMDStructure::COMDStructure(COMDocument * pDoc)
:
m_pOMDoc(pDoc),
mpCurrentClass(NULL),
mpCurrentInteraction(NULL)
{
    mBasicTypesToSize["short" ] = 2;
    mBasicTypesToSize["unsigned short" ] = 2;
    mBasicTypesToSize["long" ] = 4;
    mBasicTypesToSize["unsigned long" ] = 4;
    mBasicTypesToSize["int32" ] = 4;
    mBasicTypesToSize["uint32" ] = 4;
    mBasicTypesToSize["long long" ] = 8;
    mBasicTypesToSize["unsigned long long" ] = 8;
    mBasicTypesToSize["octet" ] = 1;
    mBasicTypesToSize["char" ] = 1;
    mBasicTypesToSize["unsigned char" ] = 1;
    mBasicTypesToSize["any" ] = 1; // same as octet
    mBasicTypesToSize["string" ] = 1; // no min length
    mBasicTypesToSize["double" ] = 8;
    mBasicTypesToSize["float" ] = 4;
    mBasicTypesToSize["boolean" ] = 4;

    Process();
}

COMDStructure::~COMDStructure()
{
    //
}

bool COMDStructure::isBasicType(const string & type) const
{
    bool bRetVal = false;

    map<string, size_t>::const_iterator item = mBasicTypesToSize.find(type);

    bRetVal = (item != mBasicTypesToSize.end());

    return bRetVal;
}

template <typename K, typename V>
V lookup(const map< K, V > & rMap, const K & key)
{
    V retVal(0);

    typename map< K, V >::const_iterator item = rMap.find(key);

    if (item != rMap.end())
    {
        retVal = (*item).second;
    }

    return retVal;
};

string getValuePairValue(const OMDNode * pList, const string & item)
{
    string retVal;

    do
    {
        if (pList == NULL) break;

        const OMDBase * pItem = pList->getChild(item);

        if (pItem == NULL) break;

        const CAttributeValuePair * pAVPItem = dynamic_cast<const CAttributeValuePair *> (pItem);

        if (pAVPItem == NULL) break;

        retVal = stripQuotes(pAVPItem->getValue());

    }
    while (0);

    return retVal;
}

bool   COMDStructure::testDiscriminantRecord( const OMDNode * pCDT, string & discrimVar ) const
{
    bool bRetVal = false;

    do
    {
        if (!pCDT) break;

        size_t count = pCDT->getChildCount();

        if (count == 0) break; // no children --> nothing to process

        // a discriminant record must have a fixed cardinality enum and
        // references to types (simple or complex) that are of type
        //
        bool bHasDiscrimCases = false; // cases using
        bool bHasDiscrimEnum  = false; // enum usable for discriminant var
        string tempVar;
        list<string> discrimVarList;

        OMDCardinalityParser & rCardParser = getParser();

        for (size_t indx = 0; indx < count; indx++)
        {
            OMDBase * pBase = pCDT->getChild(indx);

            if (!pBase) continue;

            const CComplexComponent * pCC = dynamic_cast<const CComplexComponent *> ( pCDT->getChild(indx) );
            
            string type = pBase->getType();

            //const OMDNode * pInnerCDT = lookup(mNameToCDT, type);
            const OMDNode * pEDT      = lookup(mNameToEDT, type);

            if (isBasicType(type))
            {
                // nothing to look at here
            }
            else if (pCC)
            {
                string cardValue = getValuePairValue(pCC, "Cardinality");
                CCardinalityExpr * pCard = rCardParser.IsOK( cardValue );

                if (pCard->getCardinalityType() == eTYPE1) // could be discrimVar
                {
                    if (pEDT)
                    {
                        string temp = pCC->getName();
                        cout << "" << temp << endl;
                        discrimVarList.push_back( temp );
                        bHasDiscrimEnum = false;
                    }
                }
                else if (pCard->getCardinalityType() == eTYPE5a) // could be ...
                {

                }
            }
        }

        bRetVal    = bHasDiscrimCases && bHasDiscrimEnum;

        if (bRetVal)
        {
            discrimVar = tempVar;
        }

    } while(0);

    return bRetVal;
}

string COMDStructure::getDiscrimVar         ( const OMDNode * pCDT ) const
{
  string retVal;

  return retVal;
}

ostream & COMDStructure::traverseType(ostream & out,
                                      const string & type,
                                      size_t & offset,
                                      size_t & bytesRemaining,
                                      unsigned char * & pData) const
{
    stringstream str;

    //<editor-fold desc="enclosing do-while">
    do
    {
        //<editor-fold desc="Base Type handling">
        if (isBasicType(type))
        {
            // basic types

            size_t size = lookup(mBasicTypesToSize, type);
            //<editor-fold desc="check for zero size">
            if (size == 0)
            {
                // didn't find it !! how's that possible?
                break;
            }
            //</editor-fold>

            str << "@ " << setw(3) << left << offset << ": " << type;

            //<editor-fold desc="basic types handling">
            if (pData)
            {
                if (type == "short")
                {
                    short shortVal = 0;

                    toNativeOrderBasic<sizeof (short), short>(shortVal, (const unsigned char * &) pData, bytesRemaining);

                    str << shortVal;
                }
                else if (type == "unsigned short")
                {
                    unsigned short uShortVal = 0;

                    toNativeOrderBasic<sizeof (unsigned short), unsigned short>(uShortVal, (const unsigned char * &) pData, bytesRemaining);

                    str << uShortVal;
                }
                else if (type == "long")
                {
                    long longVal = 0;

                    toNativeOrderBasic<sizeof (long), long>(longVal, (const unsigned char * &) pData, bytesRemaining);

                    str << longVal;
                }
                else if (type == "unsigned long")
                {
                    unsigned long uLongVal = 0;

                    toNativeOrderBasic<sizeof (unsigned long), unsigned long>(uLongVal, (const unsigned char * &) pData, bytesRemaining);

                    str << uLongVal;
                }
                else if (type == "int32")
                {
                    int32 int32Val = 0;

                    toNativeOrderBasic<sizeof (int32), int32 > (int32Val, (const unsigned char * &) pData, bytesRemaining);

                    str << int32Val;
                }
                else if (type == "uint32")
                {
                    uint32 uInt32Val = 0;

                    toNativeOrderBasic<sizeof ( uint32), uint32 > (uInt32Val, (const unsigned char * &) pData, bytesRemaining);

                    str << uInt32Val;
                }
                else if (type == "long long")
                {
                    long long longLongVal = 0;

                    toNativeOrderBasic<sizeof (long long), long long>(longLongVal, (const unsigned char * &) pData, bytesRemaining);

                    str << longLongVal;
                }
                else if (type == "unsigned long long")
                {
                    unsigned long long uLongLongVal = 0;

                    toNativeOrderBasic<sizeof (unsigned long long), unsigned long long>
                        (uLongLongVal, (const unsigned char * &) pData, bytesRemaining);

                    str << uLongLongVal;
                }
                else if ((type == "octet") || (type == "unsigned char") || (type == "any"))
                {
                    //const unsigned char * pOctet = (const unsigned char *)pData;

                    // ???
                }
                else if (type == "char")
                {
                    const char * pChar = (const char *) pData;

                    str << int( *pChar);
                }
                else if (type == "string")
                {
                    const char * pChar = (const char *) pData;

                    size_t charSize = strlen(pChar);
                    size = charSize;

                    str << string(pChar, size);
                }
                else if (type == "double")
                {
                    double dblVal = 0;

                    toNativeOrderBasic<sizeof (double), double>(dblVal, (const unsigned char * &) pData, bytesRemaining);

                    str << dblVal;
                }
                else if (type == "float")
                {
                    float floatVal = 0;

                    toNativeOrderBasic<sizeof (float), float>(floatVal, (const unsigned char * &) pData, bytesRemaining);

                    str << floatVal;
                }
                else if (type == "boolean")
                {
                    const unsigned int * pBool = (const unsigned int *) pData;

                    str << (*pBool ? "true" : "false");
                }
                else
                {
                    mErrorSummary << "COMDStructure::traverseType : didn't recognize type \""
                        << type
                        << "\""
                        << endl;
                }
                pData += size; // advance the pointer
            }

            offset += size;
            bytesRemaining -= size;

            break;

        } //  isBasicType( type )
        // </editor-fold>
        // iterate through the CDT        
        const OMDNode * pCDT = lookup(mNameToCDT, type);
        const OMDNode * pEDT = lookup(mNameToEDT, type);

        if ((pCDT == NULL) && (pEDT == NULL))
        {
            str << "COMDStructure::traverseType : couldn't find type \"" << type << "\"" << endl;
            break;
        }

        if (pCDT == NULL) pCDT = pEDT;

        size_t count = pCDT->getChildCount();

        size_t indx;

        //size_t bytesRemaining = 9999;

        // test if this is a discriminant record
        string discrimVar;
        bool   bIsDiscrim = testDiscriminantRecord(pCDT, discrimVar);

        size_t discrimSize = offset;

        for (indx = 0; indx < count; indx++)
        {
            const CComplexComponent * pCC = dynamic_cast<const CComplexComponent *> (pCDT->getChild(indx));

            if (!pCC) continue;

            CStructBlock sb;

            string fieldName = getValuePairValue(pCC, "FieldName"  );
            string dataType  = getValuePairValue(pCC, "DataType"   );
            string cardValue = getValuePairValue(pCC, "Cardinality");

            OMDCardinalityParser & rCardParser = getParser();

            CCardinalityExpr * pCardExpr = rCardParser.IsOK(cardValue);

            str << sb
                << "FieldName : "
                << fieldName
                << ", DataType : "
                << dataType
                << ", Cardinality : "
                << cardValue
                << " ("
                << ((pCardExpr) ? pCardExpr->getExpr() : "")
                << "); ";

            if (bIsDiscrim)
            {
                offset = discrimSize; // reset to establish same offset for this pass
            }
            else
            {
                discrimSize = offset; // save this for when we decide it a discrimated record
            }

            traverseType(str, dataType, offset, bytesRemaining, pData);
        }

    }
    while (0);
    //</editor-fold>
    out << str.str();

    return out;
}

string idChainToFullName(size_t id,
                         const map<size_t, size_t> & idToParentId,
                         const map<size_t, string> & idToName
                         )
{
    string retVal;

    static size_t depth = 0;

    depth++;

    //<editor-fold desc="enclosing DO-WHILE">
    do
    {
        if (depth > 64) break;

        map<size_t, size_t>::const_iterator idItem = idToParentId.find(id);

        if (idItem == idToParentId.end()) break;

        map<size_t, string>::const_iterator nameItem = idToName.find(id);

        if (nameItem == idToName.end()) break;

        size_t parentId = (*idItem).second;

        retVal = idChainToFullName( parentId, idToParentId, idToName );

        if (retVal.size() > 0) retVal += ".";

        retVal += (*nameItem).second;

    } while (0);
    //</editor-fold>

    if (depth > 0) depth--;

    return retVal;
}

template <typename T>
const char * getRoot(const T*);

template <>
const char * getRoot(const CAttribute *)
{
    return "objectRoot.";
};

template <>
const char * getRoot(const CParameter *)
{
    return "interactionRoot.";
};

template <typename T>
void connectDots(const map<size_t, size_t> & idToParentId,
                 const map<size_t, string> & idToName,
                 map <string, size_t > & fullNameToId,
                 map <size_t, string > & idToFullName,
                 stringstream & errorSummary
                 )
{
    map<size_t, string>::const_iterator item = idToName.begin();
    map<size_t, string>::const_iterator last = idToName.end();

    while (item != last)
    {
        size_t id = (*item).first;

        ++item;

        string fullName = getRoot((const T*) NULL) + idChainToFullName(id, idToParentId, idToName);

        typename map <string, size_t>::const_iterator outItem = fullNameToId.find(fullName);

        if (outItem != fullNameToId.end())
        {
            errorSummary << "full name \"" << fullName << "\" already processed" << endl;
        }

        fullNameToId.insert(pair<string, size_t > (fullName, id));
        idToFullName.insert(pair<size_t, string > (id, fullName));
    }
}

//
// Define the OMDTask interfaces
//

void COMDStructure::Process(void) // do the work
{
    Process(m_pOMDoc); // this will visit all the object and interactions, generating structure info

    // connect the dots

    connectDots<CAttribute > (mObjectIdtoParentId,
        mObjectIdToName,
        mFullObjectNameToId,
        mObjectIdToFullName,
        mErrorSummary
        );

    connectDots<CParameter > (mIntIdtoParentId,
        mIntIdToName,
        mFullInteractionNameToId,
        mInteractionIdToFullName,
        mErrorSummary
        );
}

int COMDStructure::Process(const CAttribute * pItem)
{
    if (mCurrentId == 0)
    {
        return -1;
    }

    mObjectIdToAttributeList[ mCurrentId ].push_back(pItem);

    //pItem->Execute( this );   // continue processing

    return 0;
}

int COMDStructure::Process(const CParameter * pItem)
{
    if (mCurrentId == 0)
    {
        return -1;
    }

    mInteractionIdToParameterList[ mCurrentId ].push_back(pItem);

    //pItem->Execute( this );   // continue processing

    return 0;
}

int COMDStructure::Process(const CAttributeList * pItem)
{
    return 0;
}

int COMDStructure::Process(const CAttributeValuePair * pItem)
{
    string name = pItem->getName();
    string value = pItem->getValue();

    if (name == "ID")
    {
        mCurrentId = atoi(value.c_str());
    }
    else if ((name == "SuperClass") || (name == "SuperInteraction"))
    {
        mCurrentParentId = atoi(value.c_str());
    }
    else if (name == "Name")
    {
        mCurrentName = stripQuotes(value);
    }

    return 0;
}

int COMDStructure::Process(const CClass * pItem)
{
    mpCurrentClass = pItem;
    mCurrentId = 0;
    mCurrentParentId = 0;

    pItem->Execute(this);

    if (mCurrentId == 0)
    {
        mErrorSummary << "Current ID not set up in Process(const CClass...)" << endl;
        return -1;
    }

    mObjectIdToName    [ mCurrentId ] = mCurrentName;
    mObjectIdtoParentId[ mCurrentId ] = mCurrentParentId;
    mIdToClass         [ mCurrentId ] = mpCurrentClass;

    mpCurrentClass = NULL;
    mCurrentId = 0;
    mCurrentParentId = 0;

    return 0;
}

int COMDStructure::Process(const CInteraction * pItem)
{
    mpCurrentInteraction = pItem;
    mCurrentId = 0;
    mCurrentParentId = 0;

    pItem->Execute(this);

    if (mCurrentId == 0)
    {
        mErrorSummary << "Current ID not set up in Process(const CInteraction ...)" << endl;
        return -1;
    }

    mIntIdToName     [ mCurrentId ] = mCurrentName;
    mIntIdtoParentId [ mCurrentId ] = mCurrentParentId;
    mIdToInteraction [ mCurrentId ] = mpCurrentInteraction;

    mpCurrentInteraction = NULL;
    mCurrentId           = 0;
    mCurrentParentId     = 0;

    return 0;
}

int COMDStructure::Process(const CComplexComponent * pItem)
{
    return 0;
}

int COMDStructure::Process(const CBitfieldComponent * pItem)
{
    return 0;
}

int COMDStructure::Process(const CBitfieldDataType * pItem)
{
    if (pItem == NULL) return -1;

    string name = getValuePairValue(pItem, "Name");

    if (name.size() == 0) return -1;

    mNameToBFDT[ name ] = pItem;

    return 0;
}

int COMDStructure::Process(const CComplexDataType * pItem)
{
    if (pItem == NULL) return -1;

    string name = getValuePairValue(pItem, "Name");

    if (name.size() == 0) return -1;

    mNameToCDT[ name ] = pItem;

    return 0;
}

int COMDStructure::Process(const CDimension * pItem)
{
    return 0;
}

int COMDStructure::Process(const CEnumeratedDataType * pItem)
{
    if (pItem == NULL) return -1;

    string name = getValuePairValue(pItem, "Name");

    if (name.size() == 0) return -1;

    mNameToEDT[ name ] = pItem;

    return 0;
}

int COMDStructure::Process(const CEnumeration * pItem)
{
    return 0;
}

int COMDStructure::Process(const CNote * pItem)
{
    return 0;
}

int COMDStructure::Process(const CNoteList * pItem)
{
    return 0;
}

int COMDStructure::Process(const CNoteText * pItem)
{
    return 0;
}

int COMDStructure::Process(const COMDocument * pItem)
{
    pItem->Execute(this);

    return 0;
}

int COMDStructure::Process(const CObjectModel * pItem)
{
    pItem->Execute(this);

    return 0;
}

int COMDStructure::Process(const CRoutingSpace * pItem)
{
    return 0;
}

int COMDStructure::Process(const OMDBase * pItem)
{
    return 0;
}

int COMDStructure::Process(const OMDNode * pItem)
{
    return 0;
}

int COMDStructure::Process(const Singleton * pItem)
{
    return 0;
}

int COMDStructure::Process(const CAttributeName * pItem)
{
    return 0;
}

int COMDStructure::Process(const CAttributeValue * pItem)
{
    return 0;
}

list<const CAttribute *> COMDStructure::getObjectAttributeList(size_t id) const
{
    list<const CAttribute *> retVal;

    map<size_t, list<const CAttribute *> >::const_iterator item = mObjectIdToAttributeList.find(id);

    if (item != mObjectIdToAttributeList.end())
    {
        retVal = (*item).second;
    }

    return retVal;
}

list<const CParameter *> COMDStructure::getInteractionParameterList(size_t id) const
{
    list<const CParameter *> retVal;

    map<size_t, list<const CParameter *> >::const_iterator item = mInteractionIdToParameterList.find(id);

    if (item != mInteractionIdToParameterList.end())
    {
        retVal = (*item).second;
    }

    return retVal;
}

list<const CAttribute *> COMDStructure::getList(size_t id, const CAttribute *) const
{
    return getObjectAttributeList(id);
}

list<const CParameter *> COMDStructure::getList(size_t id, const CParameter *) const
{
    return getInteractionParameterList(id);
}

template <typename T>
const char * getItemTitle(const T *);

template <>
const char * getItemTitle(const CAttribute *)
{
    return "ObjectName";
};

template <>
const char * getItemTitle(const CParameter *)
{
    return "InteractionName";
};

ostream & COMDStructure::printNameAndTypeAndCard(const CAttributeList * pList, ostream & out) const
{
    stringstream str;

    static size_t nameWidth = 0;
    static size_t typeWidth = 0;
    static size_t cardWidth = 0;

    do
    {
        if (pList == NULL) break;

        const OMDBase * pName = pList->getChild("Name");

        const CAttributeValuePair * pAVPName = dynamic_cast<const CAttributeValuePair *> (pName);

        if (pAVPName == NULL) break;

        string name = stripQuotes(pAVPName->getValue());

        if (nameWidth < name.size()) nameWidth = name.size();

        str << "Name : " << setw(nameWidth) << left << name;

        const OMDBase * pType = pList->getChild("DataType");

        const CAttributeValuePair * pAVPType = dynamic_cast<const CAttributeValuePair *> (pType);

        if (pAVPType == NULL) break;

        string dataType = stripQuotes(pAVPType->getValue());

        if (typeWidth < dataType.size()) typeWidth = dataType.size();

        str << ", DataType : " << setw(typeWidth) << left << dataType;

        const OMDBase * pCard = pList->getChild("Cardinality");

        const CAttributeValuePair * pAVPCard = dynamic_cast<const CAttributeValuePair*> (pCard);

        if (pAVPCard == NULL) break;

        string card = stripQuotes(pAVPCard->getValue());

        if (cardWidth < card.size()) cardWidth = card.size();

        str << ", Cardinality : " << setw(cardWidth) << left << card;

    }
    while (0);

    if (str.str().size() > 0) str << endl;

    out << str.str();

    return out;
}

template <typename T>
ostream & COMDStructure::dumpItems(const map<string, size_t> & fullNameToId,
                                   ostream & out
                                   ) const
{
    stringstream str;

    map<string, size_t>::const_iterator item = fullNameToId.begin();
    map<string, size_t>::const_iterator last = fullNameToId.end();

    while (item != last)
    {
        str << getItemTitle((const T*) NULL) << ": " << (*item).first << endl;

        size_t id = (*item).second;

        ++item;

        list<const T *> itemList = getList(id, (const T *) NULL);

        typename list<const T *>::const_iterator listItem = itemList.begin();
        typename list<const T *>::const_iterator listLast = itemList.end();

        if (listItem == listLast)
        {
            str << "-- is empty --" << endl << endl;
            continue;
        }

        while (listItem != listLast)
        {
            printNameAndTypeAndCard(*listItem, str);

            ++listItem;
        }

        str << endl << endl;
    }

    out << str.str();

    return out;
};

ostream & COMDStructure::print(ostream & out) const
{
    stringstream str;

    // output the Objects first

    dumpItems<CAttribute > (mFullObjectNameToId, str);

    dumpItems<CParameter > (mFullInteractionNameToId, str);

    str << endl << "Error Summary:" << endl << mErrorSummary.str() << endl << endl;

    out << str.str();

    return out;
}

size_t COMDStructure::CStructBlock::gIndent = 0;

ostream & COMDStructure::CStructBlock::print(ostream & out) const
{
    stringstream str;

    str << endl << setw(gIndent * 3) << " ";

    out << str.str();

    return out;
}
