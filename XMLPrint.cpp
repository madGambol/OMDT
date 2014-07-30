
#include <string.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <list>
#include <map>
#include <set>

using namespace std;

class CNoteText;

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
#include "XMLPrint.h"

#include <stdlib.h>

using namespace std;

#define indentToLevel(x) ""

const string OPEN ("<");
const string CLOSE(">");
const string END  ("</");
const string CLOSEOPEN("/>");

const string OPENPROCESS("<?");
const string CLOSEPROCESS("?>");

#define EASYREAD(x)

//string escape(char ch)
//{
//    switch (ch)
//    {
//    case '&':
//        return "&amp;";
//    case '<':
//        return "&lt;";
//    default:
//        break;
//    }
//    string retVal = ch;
//    return string().push_back(ch);
//}

string escapable(const string & str)
{
    // test for special characters not allowed in CDATA
    
    string tmp = str;
    
//    static map<string, string> charToSymbolMap;
//    static bool bFirst = true;
//    
//    if (bFirst)
//    {
//        charToSymbolMap["<"] = "&lt;";
//        charToSymbolMap["&"] = "&amp;";
//        charToSymbolMap[">"] = "&lt;";
//        bFirst = false;
//    }

    const char * pFound = strpbrk( tmp.c_str(), "><&");
    
    if (pFound)
    {
        tmp = "";
        char xx[2] = {0,0};
        
        for (size_t i = 0; i < str.size(); i++)
        {
            xx[0] = str[i];
            if (xx[0] == '&')
            {
                tmp += "&amp;";
            }
            else if (xx[0] == '<')
            {
                tmp += "&lt;";
            }
            else if (xx[0] == '>')
            {
              tmp += "&gt;";
            }
            else if (xx[0] == '\'')
            {
              tmp += "&apos;";
            }
            else if (xx[0] == '\"')
            {
              tmp += "&quot;";
            }
            else
            {
                tmp += xx;
            }
        }
    }    
    return tmp;
}

class CScopeLevel
{
    size_t & mrLevel;
    size_t   mOrigLevel;

public:

    CScopeLevel( size_t & level )
    :
    mrLevel   ( level ),
    mOrigLevel( level )
    {
        mrLevel += 3;
    }

    ~CScopeLevel()
    {
        mrLevel = mOrigLevel;
    }
};

XMLPrint::XMLPrint(COMDocument * pOMDoc)
:
m_pOMDoc( pOMDoc ),
m_Level ( 0 )
{
}

XMLPrint::~XMLPrint()
{
}

void XMLPrint::Process(void)
{
    Process( m_pOMDoc );
}

//                              1111111111222222222233333333334444444444555555555566666666667
//                     1234567890123456789012345678901234567890123456789012345678901234567890
//const char * pLevel = "                                                                      ";
//const size_t maxLevel = strlen(pLevel);
size_t   XMLPrint::CBlock::gLevel = 0;

void XMLPrint::blockStart( const OMDBase * pItem )
{
    mOutVal << indentToLevel(CBlock::gLevel)
            << OPEN
            << pItem->getType()
            << CLOSE;

    //   const Loc * pLoc = pItem->getLocation();

    //   if (pLoc) 
    //   {
    //     stringstream temp;
    //
    //     temp <<" ;; line ("
    //          << pLoc->first_line
    //          << ", "
    //          <<  pLoc->first_column
    //          << ", "
    //          <<  pLoc->last_line
    //          << ", "
    //          <<  pLoc->last_column
    //          << ")";
    //
    //     mOutVal << temp.str();
    //   }

    EASYREAD( mOutVal << endl );
}

void XMLPrint::blockEnd  ( const OMDBase * pItem)
{
    EASYREAD( mOutVal << indentToLevel(CBlock::gLevel) );

    mOutVal << END << pItem->getType() << CLOSE << endl;
}

XMLPrint::CBlock::CBlock( ostream & out, const OMDBase * pItem, bool bTrailing  )
    :
mOut      ( out       ),
mpItem    ( pItem     ),
mbTrailing( bTrailing )
{
    //size_t localLevel = gLevel % maxLevel;

    mOut << OPEN
         << mpItem->getType()
         << CLOSE;

    //   const Loc * pLoc = pItem->getLocation();

    //   if (pLoc) 
    //   {
    //     stringstream temp;
    //
    //     temp <<" ;; line ("
    //          << pLoc->first_line
    //          << ", "
    //          <<  pLoc->first_column
    //          << ", "
    //          <<  pLoc->last_line
    //          << ", "
    //          <<  pLoc->last_column
    //          << ")";
    //
    //     mOut << temp.str();
    //   }

    EASYREAD( mOut << endl );
}

XMLPrint::CBlock::~CBlock(void)
{
    EASYREAD( mOut << setw(gLevel) );
        
    mOut << END << mpItem->getType() << CLOSE;

    mOut << endl;
}


// const char * XMLPrint::indentToLevel(size_t level)
// {
//     size_t localLevel = level % maxLevel;

//     return pLevel+(maxLevel-localLevel);
// }

int XMLPrint::Process ( const COMDocument         * pItem )
{
    pItem->Execute( this );

    return 0;
}

//
// Print the object attributes
//
int XMLPrint::Process ( const CAttribute          * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int XMLPrint::Process ( const CAttributeList      * pItem )
{
    mOutVal << pItem->getString();

    return 0;
}

int XMLPrint::Process ( const CAttributeValuePair * pItem )
{
    EASYREAD( mOutVal << indentToLevel(CBlock::gLevel) );

    mNotes.clear();

    pItem->Execute( this );

    stringstream str;

    //str << left << setw(17) << mAttribute << " " << mValue;
    str << OPEN << mAttribute;

    if (mNotes.size() > 0) 
    {
//        str << mNotes;
        str << " notes=\"";
        std::set<size_t>::iterator item = mNotes.begin();
        std::set<size_t>::iterator last = mNotes.end();
        
        while (item != last)
        {
          str << *item;
          ++item;
          if (item != last) str << ",";
        }
        
        str << "\"";
    }
    
    str << CLOSE << escapable(mValue) << END << mAttribute << CLOSE << endl;

    mOutVal << str.str();

    return 0;
}

int XMLPrint::Process ( const CBitfieldComponent   * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int XMLPrint::Process ( const CClass              * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int XMLPrint::Process ( const CComplexComponent   * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int XMLPrint::Process ( const CDimension          * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int XMLPrint::Process ( const CEnumeratedDataType * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int XMLPrint::Process ( const CEnumeration        * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int XMLPrint::Process ( const CInteraction        * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int XMLPrint::Process ( const CNote               * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int XMLPrint::Process ( const CNoteText * pItem )
{
    EASYREAD( mOutVal << indentToLevel(CBlock::gLevel) );

    mNotes.clear();

    pItem->Execute( this );

    stringstream str;

    str << OPEN << mAttribute << CLOSE << escapable(mValue) << END << mAttribute << CLOSE;
    
    EASYREAD( str << endl );

    mOutVal << str.str();
    
    return 0;
}

int XMLPrint::Process ( const CNoteList * pItem )
{
    mNotes = pItem->getNoteSet();

    return 0;
}

int XMLPrint::Process ( const CObjectModel * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int XMLPrint::Process ( const CParameter * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int XMLPrint::Process ( const CRoutingSpace * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int XMLPrint::Process ( const CComplexDataType * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int XMLPrint::Process ( const CBitfieldDataType     * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int XMLPrint::Process ( const OMDBase            * pItem )
{
    return 0;
}

int XMLPrint::Process ( const OMDNode            * pItem )
{
    return 0;
}

int XMLPrint::Process ( const Singleton          * pItem )
{
    mOutVal << pItem->getValue() << " ";
    return 0;
}

int XMLPrint::Process ( const CAttributeName     * pItem )
{
    mAttribute = pItem->getName();

    return 0;
}

int XMLPrint::Process ( const CAttributeValue    * pItem )
{
    mValue = pItem->getValue();

    return 0;
}

ostream & XMLPrint::print( ostream & out ) const
{
    //out << "XMLPrint::print() const" << endl;
    //mOutVal = "";
    //Process( m_pOMDoc );
    
    stringstream str;

    str << OPENPROCESS << "xml version=\"1.0\"" << CLOSEPROCESS << endl;

    str << OPEN << "OMD" << CLOSE 
        << mOutVal.str() 
        << END << "OMD" << CLOSE
        << endl;
    
    out << str.str();

    return out;
}

