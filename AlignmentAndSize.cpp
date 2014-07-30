#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <list>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#include "cardinality.hpp"

//#include "CPrint.h"
#include "eprint.hh"

#include "OMDTask.h"
#include "singleton.h"
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

#include "expr.hh"

#include "AlignmentAndSize.h"
#include "OMDCardinalityParser.h"

string stripQuotes(const string & input)
{
  string retVal = input;

  if (retVal[0] == '"') retVal = retVal.substr(1, retVal.size()-2);

  return retVal;
}

string limitExpr(stringstream & str, size_t maxWidth, size_t & exprLimit, const string & expr)
{
  string retVal;

  if (expr.size() > maxWidth)
  {
      stringstream tmp;

      tmp << "<expr " << exprLimit << ">";

      retVal = tmp.str();

      str << endl
	  << left
	  << setw( 4 )
	  << exprLimit
	  << " : "
	  << expr
	  << endl;

      exprLimit++;
  }
  else
  {
      retVal = expr;
  }

  return retVal;
}

AlignmentAndSize::AlignmentAndSize(COMDocument * pOMDoc)
  :
    mpOMDoc   ( pOMDoc   ),
    mbDeferred( false    ),
    mbInCDT   ( false    )
{
    mTypeNameToInfo["short"              ] = AlignmentSizeRec( 2, "2" );
    mTypeNameToInfo["unsigned short"     ] = AlignmentSizeRec( 2, "2" );
    mTypeNameToInfo["long"               ] = AlignmentSizeRec( 4, "4" );
    mTypeNameToInfo["unsigned long"      ] = AlignmentSizeRec( 4, "4" );
    mTypeNameToInfo["int32"              ] = AlignmentSizeRec( 4, "4" );
    mTypeNameToInfo["uint32"             ] = AlignmentSizeRec( 4, "4" );
    mTypeNameToInfo["long long"          ] = AlignmentSizeRec( 8, "8" );
    mTypeNameToInfo["unsigned long long" ] = AlignmentSizeRec( 8, "8" );
    mTypeNameToInfo["octet"              ] = AlignmentSizeRec( 1, "1" );
    mTypeNameToInfo["char"               ] = AlignmentSizeRec( 1, "1" );
    mTypeNameToInfo["any"                ] = AlignmentSizeRec( 1, "1" );  // same as octet
    mTypeNameToInfo["string"             ] = AlignmentSizeRec( 1, "_str_" );  // no min length
    mTypeNameToInfo["double"             ] = AlignmentSizeRec( 8, "8" );
    mTypeNameToInfo["float"              ] = AlignmentSizeRec( 4, "4" );
    mTypeNameToInfo["boolean"            ] = AlignmentSizeRec( 1, "1" );
}

AlignmentAndSize::~AlignmentAndSize()
{
}

void AlignmentAndSize::processDeferred(void)
{
    do
    {
	list< const CComplexDataType *> deferred = mDeferred;

	mDeferred.clear();

	list< const CComplexDataType *>::iterator item = deferred.begin();
	list< const CComplexDataType *>::iterator last = deferred.end();

	while (item != last)
	{
	    //(*item)->Execute( this );

            Process ( *item );

	    ++item;
	}

	// put circularity check here TBS

    } while (!mDeferred.empty());

}

int AlignmentAndSize::Process ( const COMDocument         * pItem )
{
    cerr << "Starting processing ..." << endl;
    
    pItem->Execute( this );  // this should process all the simpler types

    cerr << "Starting deferred processing" << endl;

    processDeferred();

    cerr << "Done processing." << endl;
    
    return 0;
}

void AlignmentAndSize::Process(void)
{
    Process ( mpOMDoc );
}

int AlignmentAndSize::Process ( const CAttribute          * pItem )
{
    pItem->Execute( this );
    
    return 0;
}

int AlignmentAndSize::Process ( const CAttributeList      * pItem )
{
    pItem->Execute( this );
    
    return 0;
}

int AlignmentAndSize::Process ( const CAttributeValuePair * pItem )
{
    pItem->Execute( this );

    // cout << *pItem << endl;
    
    return 0;
}

int AlignmentAndSize::Process ( const CClass              * pItem )
{
    pItem->Execute( this );

    return 0;
}

int AlignmentAndSize::Process ( const CBitfieldComponent   * pItem )
{
    return 0;    // TBS 
}

int AlignmentAndSize::Process ( const CBitfieldDataType    * pItem )
{    
    return 0;
}

int AlignmentAndSize::Process ( const CComplexComponent   * pItem )
{
    // The mDataType and mCardinality should be set up. If so, process those.

    do
    {
	if (mbDeferred)
        {
            break;  // if something earlier couldn't be decoded, defer what follows
        }

	pItem->Execute( this ); // process this CC

	if (mDataType.empty() || mCardinality.empty())
        {
            cerr << "mDataType = \""
                 << mDataType
                 << "\", mCardinality = \""
                 << mCardinality
                 << "\""
                 << endl;
            
            break;
        }
        
	// Add this CC to the list of CC for this CDT
	list< const CComplexComponent *> & tempList = mCdtToCcList[ mComplexDataType ];

	if (find(tempList.begin(), tempList.end(), pItem ) == tempList.end() ) // check in case we're doing deferred processing
	{
	    mCdtToCcList[ mComplexDataType ].push_back( pItem );
	}

	// look up the data type -- if its defined add to the current picture

	AlignmentSizeRecIter item = mTypeNameToInfo.find( mDataType );

	if (item == mTypeNameToInfo.end()) 
	{
            // cout << "Deferring because of \"" << mDataType << "\"" << endl;
            
	    mbDeferred = true;
	    break;
	}

	mCurrentCC = (*item).second;

        mCurrentCC.normalize();

//         cout << ">>>>>" << (*item).second << endl;

        OMDCardinalityParser cardExpr;
        CCardinalityExpr * pBase;

        string val;

        if ((pBase = cardExpr.IsOK( mCardinality )) != NULL)
        {
            // cardinality expression is OK

            val = pBase->getExpr();

            if ((val == "1") && (mCardinality == "1"))
            {
                // skip this case
            }
            else
            {
//                 cout << "Got \"" << val << "\" from \"" << mCardinality << "\"" << endl;
            }
        }
        else
        {
            cout << "AlignmentAndSize::Process CC : couldn't parse \""
                 << mCardinality
                 << "\""
                 << ", mDataType = \""
                 << mDataType
                 << "\", mCardinality = \""
                 << mCardinality
                 << "\""
                 << endl;

            val = "_err_";
        }

//      stringstream cardinalityStr;

// 	cardinalityStr.str( mCardinality );

// 	size_t cardinality = 0;

// 	cardinalityStr >> cardinality;

// 	if (cardinality == 0)
// 	{
// 	    cout << "Warning: 0 cardinality for type "
//                  << setw( 38 )
//                  << mDataType
//                  << ", with cardinality expression = \""
//                  << cardinalityStr.str()
//                  << "\""
//                  << endl;

// 	    cardinality = 1;
// 	}

//         cout << "Adding \""
//              << mCurrentCC
//              << "\" * \""
//              << val
//              << "\" to \""
//              << mCurrentCDT
//              << "\""
//              << endl;
        
	mCurrentCDT += mCurrentCC * val;

        if (mbCDTStart)
        {
            AlignmentSizeRec dummy;
            
            if (getAlignmentAndsize( mDataType, dummy ))
            {
                mFirstCC = dummy;
            }
            mbCDTStart = false;
        }

    } while (0);
    
    return 0;
}

int AlignmentAndSize::Process ( const CComplexDataType    * pItem )
{
    mbInCDT    = true;
    mbCDTStart = true;
    
    mCurrentCDT.mAlignment = 0;
    mCurrentCDT.mSize      = "0";
    
    mFirstCC               = mCurrentCDT;

    mbDeferred       = false;
    mComplexDataType = "-not-set-";
    
    mpCurrentCdt     = pItem;

    pItem->Execute( this );

    if (mbDeferred)
    {
        static size_t rawCount = 0;
        
	mDeferred.push_back( pItem );

//         cout << "Deferring \""
//              << mComplexDataType
//              << "\" until later..."
//              << endl;

        if (rawCount > 4000) exit(0);

        rawCount++;
    }
    else
    {
        // add this one to the types...

        //mCdtToCcList[ mComplexDataType ] = list<string>();
    
        mCurrentCDT.mAlignment = mFirstCC.mAlignment;

        mTypeNameToInfo[ mComplexDataType ] = mCurrentCDT;

//         cout << "Added \""
//              << mComplexDataType
//              << "\" with data : "
//              << mCurrentCDT
//              << endl;

    }

    mComplexDataType = "";
    mbInCDT          = false;
    
    mpCurrentCdt     = NULL;

    return 0;
}

int AlignmentAndSize::Process ( const CDimension          * pItem )
{
    return 0;
}

int AlignmentAndSize::Process ( const CEnumeratedDataType * pItem )
{
    pItem->Execute( this );

    if (mComplexDataType.size() > 0)
    {
        // figure out the size

        string enumSize = getEnumerateSize( mComplexDataType );

        AlignmentSizeRec enumRec( atoi(enumSize.c_str() ), enumSize );

//         cout << "Got \"" << enumSize << "\" for \"" << mComplexDataType << "\"" << endl;

        mTypeNameToInfo[ mComplexDataType ] = enumRec;
    }
    
    return 0;
}

int AlignmentAndSize::Process ( const CEnumeration        * pItem )
{
    pItem->Execute( this );
    
    return 0;
}

int AlignmentAndSize::Process ( const CInteraction        * pItem )
{
    pItem->Execute( this );
    return 0;
}

int AlignmentAndSize::Process ( const CNote               * pItem )
{
    return 0;
}

int AlignmentAndSize::Process ( const CNoteList           * pItem )
{
    return 0;
}

int AlignmentAndSize::Process ( const CNoteText           * pItem )
{
    return 0;
}

int AlignmentAndSize::Process ( const CObjectModel        * pItem )
{
    pItem->Execute( this );
    return 0;
}

int AlignmentAndSize::Process ( const CParameter          * pItem )
{
    pItem->Execute( this );
    return 0;
}

int AlignmentAndSize::Process ( const CRoutingSpace       * pItem )
{
    return 0;
}

int AlignmentAndSize::Process ( const OMDBase             * pItem )
{
    if (mbInCDT) cerr << "Process OMDBase" << endl;
    
    return 0;
}

int AlignmentAndSize::Process ( const OMDNode             * pItem )
{
    if (mbInCDT) cerr << "Process OMDNode" << endl;
    
    return 0;
}

int AlignmentAndSize::Process ( const Singleton           * pItem )
{
    if (mbInCDT) cerr << "Process Singleton" << endl;
    
    return 0;
}

int AlignmentAndSize::Process ( const CAttributeName      * pItem )
{
    if (pItem->getName() == "Name")
    {
        mElementType = eComplexDataType;
    }
    else if (pItem->getName() == "DataType")
    {
	// got a DataType field for a ComplexComponent -- save the name

	mElementType = eDataType;
    }
    else if (pItem->getName() == "Cardinality")
    {
	mElementType = eCardinality;
    }
    else
    {
        // nothing here to see.
    }

    return 0;
}

int AlignmentAndSize::Process ( const CAttributeValue     * pItem )
{
    string value = stripQuotes(  pItem->getValue() );
    
    if (mElementType == eComplexDataType)
    {
        mComplexDataType = value;
        mElementType     = eNONE;
    }
    else if (mElementType == eDataType)
    {
	// pick up the value which will be the name of the DataType

	mDataType        = value;
        mElementType     = eNONE;
    }
    else if (mElementType == eCardinality)
    {
	mCardinality     = value;
        mElementType     = eNONE;
    }
    else
    {
	// nothing here to see.
    }
	
    return 0;
}

// template <typename T>
// class CComputeWidth
// {
//     size_t mWidth;
    
// public:
    
//     CComputeWidth()
//         :
//         mWidth(0)
//     {
//     }

//     void operator()( const T & item )
//     {
//         size_t width = item->getName().size();

//         if (mWidth < width) mWidth = width;
//     }

//     size_t getWidth(void) const
//     {
//         return mWidth;
//     }
// };

// template <typename T, typename U>
// int Oper( typename list<T>::const_iterator & item,
//           typename list<T>::const_iterator & last,
//           U & u
//         )
// {
//     while (item != last)
//     {
//         u( *item );

//         ++item;
//     }
//     return 0;
// };
ostream & AlignmentAndSize::print(ostream & out) const
{
    stringstream str;

    ConstAlignmentSizeRecIter item = mTypeNameToInfo.begin();
    ConstAlignmentSizeRecIter last = mTypeNameToInfo.end();

    size_t maxWidth = 0;

    while (item != last)
    {
        string type = (*item).first;

        if (maxWidth < type.size()) maxWidth = type.size();

        ++item;
    }

    item = mTypeNameToInfo.begin();

    while (item != last)
    {
        str << setw(maxWidth) << (*item).first;

        str << " : ";

        str << (*item).second << endl;

        ++item;
    }

    {
        OMDCardinalityParser cardExpr;

        str << endl << endl << "================== SUMMARY ==================" << endl << endl;

        map<string, list< const CComplexComponent *> >::const_iterator item = mCdtToCcList.begin();
        map<string, list< const CComplexComponent *> >::const_iterator last = mCdtToCcList.end();

        size_t nameWidth = 0;
        size_t typeWidth = 0;
        size_t exprWidth = 15;

        stringstream eSum;
        size_t exprCount = 0;

        while (item != last)
        {
            str << endl << "In " << (*item).first << ":" << endl;

            list< const CComplexComponent *> tempList = (*item).second;

            tempList.unique();

            list< const CComplexComponent *>::const_iterator listItem = tempList.begin();
            list< const CComplexComponent *>::const_iterator listLast = tempList.end();

            while (listItem != listLast)
            {
                string name = (*listItem)->getName();
                string typeName = (*listItem)->getDataType();

                if (name.size() > nameWidth) nameWidth = name.size();
                if (typeName.size() > typeWidth) typeWidth = typeName.size();

                ++listItem;
            }

            // 	    listItem = tempList.begin();

            //             CComputeWidth<const CComplexComponent *> computeWidth;

            //             Oper( listItem, listLast, computeWidth );

            //             nameWidth = computeWidth.getWidth();

            listItem = tempList.begin();

            string curSize = "0";

            while (listItem != listLast)
            {
                string name = (*listItem)->getName();
                string typeName = (*listItem)->getDataType();
                string cardinality = (*listItem)->getCardinality();

                CCardinalityExpr * pBase = cardExpr.IsOK(cardinality);

                if (pBase == NULL)
                {
                    cardinality = "_err_";
                }
                else
                {
                    switch (pBase->getCardinalityType())
                    {
                    case eTYPE1: // 123
                    {
                        CType1 * pExpr = dynamic_cast<CType1 *> (pBase);
                        stringstream str;
                        str << pExpr->getConst();
                        cardinality = str.str();
                    }
                        break;

                    case eTYPE2: // 123+
                    {
                        CType2 * pExpr = dynamic_cast<CType2 *> (pBase);
                        stringstream str;
                        str << max(1, int(pExpr->getConst()));
                        cardinality = str.str();
                    }
                        break;

                    case eTYPE3: // 123-456
                    {
                        CType3 * pExpr = dynamic_cast<CType3 *> (pBase);
                        stringstream str;
                        str << pExpr->getConstUpper();
                        cardinality = str.str();
                    }
                        break;

                    case eTYPE4: // symbolicCount
                    {
                        CType4 * pExpr = dynamic_cast<CType4 *> (pBase);
                        cardinality = pExpr->getFieldName();
                    }
                        break;

                    case eTYPE5a:// 0-1,symbol == value
                    case eTYPE5b:// SymbolicEnum : (Enumerate | Enumerate .. Enumerate | comp  Enumerate)
                    {
                        cardinality = "1";
                    }

                    default:
                        break;
                    }
                }

                map<string, AlignmentSizeRec>::const_iterator foundItem = mTypeNameToInfo.find(typeName);

                string tempExpr = limitExpr(eSum, exprWidth, exprCount, curSize);

                str << setw(4)
                    << " "
                    << setw(nameWidth)
                    << left
                    << name
                    << " : "
                    << setw(typeWidth)
                    << typeName
                    << " : offset  : "
                    << left
                    << setw(exprWidth)
                    << tempExpr;

                if (foundItem != mTypeNameToInfo.end())
                {
                    AlignmentSizeRec ccAsr = (*foundItem).second;

                    string newExpr = (string("(")
                        +
                        cardinality
                        +
                        string(")*(")
                        +
                        ccAsr.mSize
                        +
                        string(")")
                        );

                    string tempSize = normalizeExpr(newExpr);

                    if ((curSize != "0") && (tempSize == "0"))
                    {
                        cout << endl
                            << "$$$$ newExpr = \""
                            << newExpr
                            << "\", result --> \""
                            << tempSize
                            << "\""
                            << endl;
                    }

                    //curSize = tempSize;

                    CExpr * pExpr = CExpr::factory(curSize.c_str());

                    const char * pFlag = (pExpr->evalMod(ccAsr.mAlignment) ? " " : "*");

                    tempExpr = limitExpr(eSum, exprWidth, exprCount, tempSize);

                    str << " : size "
                        << setw(exprWidth)
                        << tempExpr
                        << " : align "
                        << pFlag
                        << " : "
                        << ccAsr.mAlignment
                        << endl;

                    curSize = normalizeExpr(tempSize + "+" + curSize);

                    delete pExpr;
                }
                else
                {
                    str << " -- not found" << endl;
                }

                ++listItem;
            }

            ++item;
        }
        str << endl
            << endl
            << " ****************** Expression Summary ********************"
            << eSum.str();
    }


    out << str.str();

    return out;
}

ostream & AlignmentAndSize::AlignmentSizeRec::print(ostream & out) const
{
    stringstream str;

    string normSize = normalizeExpr( mSize );

    str << "{ align = "
        << setw(2)
        << mAlignment
        << ", size = "
        << setw(3)
        << normSize
        << " }";

    out << str.str();

    return out;
}

    
// stringstream & AlignmentAndSize::print( stringstream & out )
// {
//     stringstream str;

//     str << "{ align = " << mAlignment << ", size = " << mSize << "}";
    
//     out << str.str();

//     return out;
// }



bool AlignmentAndSize::getAlignmentAndsize(const string & namedType,
                                           AlignmentSizeRec & alignmentAndSize) const
{
    bool bRetVal = false;

    ConstAlignmentSizeRecIter item = mTypeNameToInfo.find( namedType );

    do
    {
	if (item == mTypeNameToInfo.end()) break;

	alignmentAndSize = (*item).second;

	bRetVal = true;

    } while (0);

    return bRetVal;    
}

string AlignmentAndSize::getEnumerateSize( const string & enumTypeName ) const
{
    string retVal   = "4"; // in case we can't figure it out

    size_t enumPos  = enumTypeName.rfind("Enum");

    size_t totalLen = enumTypeName.size();

    do
    {
        //                   V - totalLen
        //   myEnumTypeEnum32
        //             ^
        //             enumPos
        //
        if (totalLen <= enumPos    ) break; // sum'n's wrong, quit trying
        if (totalLen -  enumPos > 6) break; // sum'n's wrong, quit trying
        if (totalLen -  enumPos < 5) break; // sum'n's wrong, quit trying

        string temp = enumTypeName.substr(enumPos+4);

        if (temp == "") break;
        
        stringstream str;

        str.str( temp );

        size_t sizeInBits = 0;

        str >> sizeInBits;

        size_t sizeInBytes = sizeInBits/8;

        if (sizeInBytes == 0) break;

        stringstream tempSS;

        tempSS << sizeInBytes;

//         cout << ">>>>" << tempSS.str() << "<<<<" << endl;

        retVal = tempSS.str();

//         cout << ">>>>" << retVal << "<<<<" << endl;

//         cout << "For \"" << enumTypeName
//              << "\", got \""
//              << temp
//              << "\", and with sizeInBytes ="
//              << sizeInBytes
//              << ", and sizeInBits = "
//              << sizeInBits
//              << ", and ret val \""
//              << retVal
//              << "\""
//              << endl;

    } while (0);
    
    return retVal;
}

string normalizeExpr(const string & size)
{
    string retVal = "0";

    do
    {
        if (size == "") break;
                
        CExpr * pExpr = CExpr::factory( size.c_str() );

        if (pExpr == NULL)
        {
            // error, oops
            cerr << "Error in AlignmentSizeRec::normalize : couldn't parse \""
                 << size
                 << "\""
                 << endl;
	    retVal = "_err_";
        }
        else
        {
            stringstream expr;

            pExpr->eprint( expr );

            retVal = expr.str();
        }

//         if (checkOrder( pExpr ))
//         {
//             cerr << "Order is OK" << endl;
//         }
//         else
//         {
//             cerr << "Order is NOT OK" << endl;
//         }

//         cout << "Input to normalize: \"" << size << "\", ==> \"" << retVal << "\"" << endl;
                
    } while(0);
            
            
    return retVal;
}

