
#include <string.h>

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
#include "PrettyPrint.h"

using namespace std;

#define indentToLevel(x) setw(x)

string escapable(const string & str)
{
    // test for blanks, line breaks, etc

    static const string quote("\"");

    const char * pFound = strpbrk( str.c_str(), " \r\t\n/-&+=([{|.");

    if (pFound)
    {
        return quote+str+quote;
    }

    return str;
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

PrettyPrint::PrettyPrint(COMDocument * pOMDoc)
:
m_pOMDoc( pOMDoc ),
m_Level ( 0 )
{
}

PrettyPrint::~PrettyPrint()
{
}

void PrettyPrint::Process(void)
{
    Process( m_pOMDoc );
}

//                              1111111111222222222233333333334444444444555555555566666666667
//                     1234567890123456789012345678901234567890123456789012345678901234567890
const char * pLevel = "                                                                      ";
const size_t maxLevel = strlen(pLevel);
size_t   PrettyPrint::CBlock::gLevel = 0;

void PrettyPrint::blockStart( const OMDBase * pItem )
{
    mOutVal << indentToLevel(CBlock::gLevel)
            << "("
            << pItem->getType();

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

    mOutVal << endl;
}

void PrettyPrint::blockEnd  ( const OMDBase * pItem, bool bTrailing )
{
    mOutVal << indentToLevel(CBlock::gLevel);

    if (bTrailing)
    { 
        mOutVal << ") ;; " << pItem->getType() << endl;
    }
    else
    {
        mOutVal  << ")" << endl;
    }
}

PrettyPrint::CBlock::CBlock( ostream & out, const OMDBase * pItem, bool bTrailing  )
    :
mOut      ( out       ),
mpItem    ( pItem     ),
mbTrailing( bTrailing )
{
    //size_t localLevel = gLevel % maxLevel;

    mOut << setw( gLevel )
         << "("
         << mpItem->getType();

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

    mOut << endl;
}

PrettyPrint::CBlock::~CBlock(void)
{
    mOut << setw(gLevel) << ")";
    
    if (mbTrailing)
    { 
        mOut << " ;; " << mpItem->getType();
    }

    mOut << endl;
}


// const char * PrettyPrint::indentToLevel(size_t level)
// {
//     size_t localLevel = level % maxLevel;

//     return pLevel+(maxLevel-localLevel);
// }

int PrettyPrint::Process ( const COMDocument         * pItem )
{
    pItem->Execute( this );

    return 0;
}

//
// Print the object attributes
//
int PrettyPrint::Process ( const CAttribute          * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int PrettyPrint::Process ( const CAttributeList      * pItem )
{
    mOutVal << pItem->getString();

    return 0;
}

int PrettyPrint::Process ( const CAttributeValuePair * pItem )
{
    mOutVal << indentToLevel(CBlock::gLevel) << "(";

    mNotes = "";

    pItem->Execute( this );

    stringstream str;

    //str << left << setw(17) << mAttribute << " " << mValue;
    str << mAttribute << " " << escapable(mValue);

    if (mNotes.size() > 0) str << " " << mNotes;

    str << ")" << endl;

    mOutVal << str.str();

    return 0;
}

int PrettyPrint::Process ( const CBitfieldComponent   * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int PrettyPrint::Process ( const CClass              * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int PrettyPrint::Process ( const CComplexComponent   * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int PrettyPrint::Process ( const CDimension          * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int PrettyPrint::Process ( const CEnumeratedDataType * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int PrettyPrint::Process ( const CEnumeration        * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int PrettyPrint::Process ( const CInteraction        * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int PrettyPrint::Process ( const CNote               * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int PrettyPrint::Process ( const CNoteText * pItem )
{
    mOutVal << indentToLevel(CBlock::gLevel) << "(";

    mNotes = "";

    pItem->Execute( this );

    stringstream str;

    str << mAttribute << " \"" << mValue << "\")" << endl;

    mOutVal << str.str();
    
    return 0;
}

int PrettyPrint::Process ( const CNoteList * pItem )
{
    mNotes = pItem->getString();

    return 0;
}

int PrettyPrint::Process ( const CObjectModel * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int PrettyPrint::Process ( const CParameter * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int PrettyPrint::Process ( const CRoutingSpace * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int PrettyPrint::Process ( const CComplexDataType * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int PrettyPrint::Process ( const CBitfieldDataType     * pItem )
{
    CBlock blockOpen( mOutVal, pItem );

    {
        CScopeLevel indent( CBlock::gLevel );

        pItem->Execute( this );
    }

    return 0;
}

int PrettyPrint::Process ( const OMDBase            * pItem )
{
    return 0;
}

int PrettyPrint::Process ( const OMDNode            * pItem )
{
    return 0;
}

int PrettyPrint::Process ( const Singleton          * pItem )
{
    mOutVal << pItem->getValue() << " ";
    return 0;
}

int PrettyPrint::Process ( const CAttributeName     * pItem )
{
    mAttribute = pItem->getName();

    return 0;
}

int PrettyPrint::Process ( const CAttributeValue    * pItem )
{
    mValue = pItem->getValue();

    return 0;
}

ostream & PrettyPrint::print( ostream & out ) const
{
    //out << "PrettyPrint::print() const" << endl;
    //mOutVal = "";
    //Process( m_pOMDoc );
    
    stringstream str;

    str << mOutVal.str() << endl;
    
    out << str.str();

    return out;
}

