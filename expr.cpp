
#include <stdlib.h>
#include <stddef.h>
#include <cstdlib>

#include <boost/spirit/include/classic_core.hpp>
#include <iostream>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <string>
#include <list>

using namespace std;

#include "CPrint.h"
#include "eprint.hh"
#include "expr.hh"

const string mul( "*" );
const string pls(" + ");

//
// This is a product in the sum of products.
//

class CProd : public CExpr
{
    int  mValue;    // coefficient, implicitly 1 if not added

    list< string > mVars;

    void order(void)
    {
        mVars.sort();
    }

public:

    CProd(int val, const char * pFile = __FILE__, size_t line = __LINE__)
      :
      CExpr ( pFile, line ),
      mValue( val )
    {
        //
        //cout << endl << endl << "in CProd ****************************" << endl << endl;
    }

    CProd(const string & var, const char * pFile = __FILE__, size_t line = __LINE__)
      :
      CExpr ( pFile, line ),
      mValue( 1 )
    {
        //
        //cout << endl << endl << "in CProd ****************************" << endl << endl;

        mVars.push_back( var );
    }

    CProd(const CProd & item)
      :
      CExpr ( item        ),
      mValue( item.mValue ),
      mVars ( item.mVars  )
    {
        //
        //cout << endl << endl << "in CProd Cpy Op ****************************" << endl << endl;
    }

    CProd & operator=(const CProd & rhs)
    {
        if (this == &rhs) return *this;

        mValue = rhs.mValue;

        mVars  = rhs.mVars;

        return *this;
    }

    virtual ~CProd()
    {
        //
    }

    eTermType getType(void) const
    {
        return eProd;
    }

    bool   evalMod (int modulus) const
    {
        return (mValue % modulus) == 0;
    }

    CProd * clone(void) const
    {
        return new CProd( *this );
    }

    CProd * simplify(void)
    {
        // TBS
        return this;
    }

    bool isValue(void) const
    {
        return mVars.empty();  // this is a value only if the list is empty (no vars)
    }

    bool isNonZero(void) const
    {
        return mValue != 0;
    }

    //
    // CProd::combine
    //

    bool combine( const CProd & item )
    {
        bool bRetVal = false;

        do
        {
            if (mVars != item.mVars) break;

            mValue += item.mValue;

            bRetVal = true;

        } while (0);

        return bRetVal;
    }

    //
    // CProd::multTerm
    //

    void multTerm( const CProd & item )
    {
        mVars.insert( mVars.begin(), item.mVars.begin(), item.mVars.end() );

        mValue *= item.mValue;

        order();
    }

    //
    // CProd::operator<
    //

    bool operator<( const CProd & rhs ) const
    {
        list< string >::const_iterator itemLhs = mVars.begin();
        list< string >::const_iterator lastLhs = mVars.end();

        list< string >::const_iterator itemRhs = rhs.mVars.begin();
        list< string >::const_iterator lastRhs = rhs.mVars.end();

        while ((itemLhs != lastLhs) && (itemRhs != lastRhs))
        {
            if (*itemLhs < *itemRhs) return true;
            if (*itemLhs > *itemRhs) return false;

            ++itemLhs;
            ++itemRhs;
        }

        return (itemLhs == lastLhs) && (itemRhs != lastRhs);
    }

    //
    // CProd::print
    //

    ostream & print(ostream & out) const
    {
        stringstream str;

        eprint( str );

        out << str.str();

        return out;
    }

    //
    // CProd::eprint
    //

    ostream & eprint(ostream & out) const
    {
        stringstream str;

        do
        {
            if ((mValue != 1) || mVars.empty() )
            {
                str << mValue;
            }

            if (mVars.empty()) break;

            if (mValue != 1)
            {
                str << "*";
            }

            list< string >::const_iterator item = mVars.begin();
            list< string >::const_iterator last = mVars.end();

            while (item != last)
            {
                str << (*item);

                ++item;

                if (item != last) str << mul;
            }

        } while (0);

        out << str.str();

        return out;
    }
} ;

//
// This is the sum in the sum of products.
//

class CSum : public CExpr
{
    list< CProd * > mVars;

    void collectTerms(void)
    {
        mVars.sort();

        list< CProd * > temp;

        list< CProd * >::iterator item = mVars.begin();
        list< CProd * >::iterator last = mVars.end();

        while (item != last)
        {
            CProd * pTempProd = *item++;

            list< CProd *>::iterator innerItem = item;

            mVars.remove( pTempProd );

            while (innerItem != last)
            {
                stringstream str;

                CProd * pInnerProd = *innerItem;

                //                 str << "CollectTerms: Combining : \""
                //                     << *pTempProd
                //                     << "\" with \""
                //                     << *pInnerProd
                //                     << "\"";


                if (!pTempProd->combine( *pInnerProd ))
                {
                    innerItem++;
                    continue;
                }

                if (item == innerItem)
                {
                    ++item;
                }

                ++innerItem;

                mVars.remove( pInnerProd );

                //                 cout << str.str() << endl;
            }

            //             cout << "temp.push_back( \"" << *pTempProd << "\")" << endl;

            temp.push_back( new CProd( *pTempProd ) );
        }

        mVars = temp;
    }

public:

    CSum(const char * pFile = __FILE__, size_t line = __LINE__)
      :
      CExpr ( pFile, line )
    {
        //         cout << endl
        //              << endl
        //              << "in CSum ****************************"
        //              << endl
        //              << endl;
        //
    }

    CSum( const CSum & item )
      :
      CExpr( item ),
      mVars( item.mVars )
    {
        //         cout << endl
        //              << endl
        //              << "in CSum Cpy OP ****************************"
        //              << endl
        //              << endl;
    }

    CSum & operator=(const CSum & rhs)
    {
        if (this == &rhs) return *this;

        mVars = rhs.mVars;

        return *this;
    }

    //
    // CSum::~CSum
    //

    virtual ~CSum()
    {
        //
        list< CProd * >::iterator item = mVars.begin();
        list< CProd * >::iterator last = mVars.end();

        while (item != last)
        {
            CProd * pItem = *item++;

            delete pItem;
        }
    }

    //
    // CSum::getType
    //

    eTermType getType(void) const
    {
        return eSum;
    }

    //
    // CSum::isNonZero
    //

    bool isNonZero(void) const
    {
        bool bRetVal = false;

        list< CProd * >::const_iterator item = mVars.begin();
        list< CProd * >::const_iterator last = mVars.end();

        while (item != last)
        {
            CProd * pItem = *item++;

            if (pItem->isNonZero())
            {
                bRetVal = true;
                break;          // no need to check further
            }
        }
        return bRetVal;
    }

    //
    // CSum::addTerms
    //

    void addTerms( const CSum & items )
    {
        mVars.insert( mVars.end(),
                     items.mVars.begin(),
                     items.mVars.end()
                     );

        collectTerms();
    }

    void addTerm( const CProd & item )
    {
        mVars.push_back( new CProd(item) );

        collectTerms();
    }

    //
    // CSum::multTerms
    //
    void multTerms( CSum const & sum );

    //
    // CSum::multTerm
    //
    void multTerm( CProd const & prod );

    //
    // CSum::evalMod
    //

    bool   evalMod (int modulus) const
    {
        bool bRetVal = true;

        list< CProd * >::const_iterator item = mVars.begin();
        list< CProd * >::const_iterator last = mVars.end();

        while (item != last)
        {
            if (!(*item)->evalMod( modulus ))
            {
                bRetVal = false;
                break;
            }
            ++item;
        }

        return bRetVal;
    }

    //
    // CSum::clone
    //

    CSum * clone(void) const
    {
        return new CSum( *this );
    }

    //
    // CSum::simplify
    //

    CSum * simplify(void)
    {
        // TBS
        return this;
    }

    //
    // CSum::isValue
    //

    bool isValue(void) const
    {
        return mVars.empty();  // this is a value only if the list is empty (no vars)
    }

    //
    // CSum::print
    //

    ostream & print(ostream & out) const
    {
        stringstream str;

        eprint( str );

        out << str.str();

        return out;
    }

    //
    // CSum::eprint
    //

    ostream & eprint(ostream & out) const
    {
        stringstream str;

        bool nonZero = false;

        list< CProd * >::const_iterator item = mVars.begin();
        list< CProd * >::const_iterator last = mVars.end();

        while (item != last)
        {
            if ((*item)->isNonZero())
            {
                if (nonZero) str << pls;

                const CProd * pProd = (const CProd *) (*item);

                pProd->eprint( str );

                nonZero = true;
            }

            ++item;
        }

        out << str.str();

        return out;
    }
} ;

CSum  operator + (CSum  const & rLHS, CSum  const & rRHS);
CSum  operator + (CSum  const & rLHS, CProd const & rRHS);
CSum  operator + (CProd const & rLHS, CSum  const & rRHS);
CSum  operator + (CProd const & rLHS, CProd const & rRHS);

CSum  operator * (CSum  const & rLHS, CSum  const & rRHS);
CSum  operator * (CSum  const & rLHS, CProd const & rRHS);
CSum  operator * (CProd const & rLHS, CSum  const & rRHS);
CProd operator * (CProd const & rLHS, CProd const & rRHS);

////////////////////////////////////////////////////////////////////////////
//
//  Semantic actions
//
////////////////////////////////////////////////////////////////////////////
namespace
{
    list< CExpr * > exprStack;

    map<string, CExpr *> nameToExpr;

    struct do_int
    {

        void operator()(const int value) const
        {
            exprStack.push_back( new CProd( value, __FILE__, __LINE__ ) );

            //             cout << "PUSH(VAL: " << value << ')' << endl;
        }
    } ;

    void doInt(char const *str, char const * end)
    {
        string s(str, end);

        long value = atoi( s.c_str() );

        exprStack.push_back( new CProd( value, __FILE__, __LINE__ ) );
    }

    void do_var(char const *str, char const * end)
    {
        string s(str, end);

        exprStack.push_back( new CProd( s, __FILE__, __LINE__ ) );

        //         cout << "PUSH(VAR: \"" << s << "\")" << endl;
    }

    void do_add(char const*, char const*)
    {
        CExpr * pRhs = exprStack.back();
        exprStack.pop_back();

        CExpr * pLhs = exprStack.back();
        exprStack.pop_back();

        CExpr * pResult = NULL;

        //         stringstream str;
        //         str << "Combining : \"";
        //         pLhs->eprint( str );
        //         str << "\" with \"";
        //         pRhs->eprint( str );
        //         str << "\" with ADD ";

        //cout << str.str() << endl;

        // do Add
        switch (pLhs->getType())
        {
        case eProd:
            switch (pRhs->getType())
            {
            case eProd:
                {
                    CProd & rLhs = *dynamic_cast<CProd *> (pLhs);
                    CProd & rRhs = *dynamic_cast<CProd *> (pRhs);

                    pResult = new CSum(rLhs + rRhs);
                }
                break;

            case eSum:
                {
                    CProd & rLhs = *dynamic_cast<CProd *> (pLhs);
                    CSum & rRhs = *dynamic_cast<CSum *> (pRhs);

                    pResult = new CSum(rLhs + rRhs);
                }
                break;
            default:
            case eExpr:
                break;
            }
            break;

        case eSum:
            switch (pRhs->getType())
            {
            case eProd:
                {
                    CSum & rLhs = *dynamic_cast<CSum *> (pLhs);
                    CProd & rRhs = *dynamic_cast<CProd *> (pRhs);

                    pResult = new CSum(rLhs + rRhs);
                }
                break;

            case eSum:
                {
                    CSum & rLhs = *dynamic_cast<CSum *> (pLhs);
                    CSum & rRhs = *dynamic_cast<CSum *> (pRhs);

                    pResult = new CSum(rLhs + rRhs);
                }
                break;
            default:
            case eExpr:
                break;
            }
            break;
        default:
        case eExpr:
            break;
        }

        exprStack.push_back(pResult);

        //str << " with result --> \"" << *pResult << "\"";

        //cout << str.str() << endl;
    }

    void do_mult(char const*, char const*)
    {
        CExpr * pRhs = exprStack.back();
        exprStack.pop_back();

        CExpr * pLhs = exprStack.back();
        exprStack.pop_back();

        CExpr * pResult = NULL;

        //         stringstream str;
        //         str << "Combining : \"";
        //         pLhs->eprint( str );
        //         str << "\" with \"";
        //         pRhs->eprint( str );
        //         str << "\" with MULT ";

        // do mult
        switch (pLhs->getType())
        {
        case eProd:
            switch (pRhs->getType())
            {
            case eProd:
                {
                    CProd & rLhs = *dynamic_cast<CProd *> ( pLhs );
                    CProd & rRhs = *dynamic_cast<CProd *> ( pRhs );

                    pResult = new CProd( rLhs * rRhs );
                }
                break;

            case eSum:
                {
                    CProd & rLhs = *dynamic_cast<CProd *> ( pLhs );
                    CSum  & rRhs = *dynamic_cast<CSum  *> ( pRhs );

                    pResult = new CSum( rLhs * rRhs );
                }
                break;
            default:
            case eExpr:
                break;
            }
            break;

        case eSum:
            switch (pRhs->getType())
            {
            case eProd:
                {
                    CSum  & rLhs = *dynamic_cast<CSum  *> ( pLhs );
                    CProd & rRhs = *dynamic_cast<CProd *> ( pRhs );

                    pResult = new CSum( rLhs * rRhs );
                }
                break;

            case eSum:
                {
                    CSum  & rLhs = *dynamic_cast<CSum  *> ( pLhs );
                    CSum  & rRhs = *dynamic_cast<CSum  *> ( pRhs );

                    pResult = new CSum( rLhs * rRhs );
                }
                break;
            default:
            case eExpr:
                break;
            }
            break;
        default:
        case eExpr:
            break;
        }

        exprStack.push_back( pResult );

        //         str << "with result --> \"" << *pResult << "\"";

        //         cout << str.str() << endl;

    }
}

////////////////////////////////////////////////////////////////////////////
//
//  Our equation grammar
//
////////////////////////////////////////////////////////////////////////////
using namespace std;

using namespace BOOST_SPIRIT_CLASSIC_NS;

struct XYZ {};

struct equation : public grammar< equation >
{
    template <typename ScannerT>
    struct definition
    {
        definition(equation const& /*self*/)
        {
            first = 
            (
                expression
                    =
                    term
                    >> *(       ('+' >> term )[ &do_add  ]
                        |       ('&' >> term )[ &do_add  ]
                        )
                    ,

                term
                    =   factor
                        >> *(   ('*' >> factor)[ &do_mult ] )
                    ,

                factor
                    =   var     [ &do_var  ]
                    |   intval  [ &doInt   ]
                    |   '(' >> expression >> ')'
                    ,

                var = lexeme_d[ ( alpha_p | ch_p('_') )
                                >>
                                *( alnum_p | ch_p('_') )
                              ],

                intval = lexeme_d[ digit_p >> *digit_p  ]
            );
            
            BOOST_SPIRIT_DEBUG_NODE( expression );
            BOOST_SPIRIT_DEBUG_NODE( term       );
            BOOST_SPIRIT_DEBUG_NODE( factor     );
            BOOST_SPIRIT_DEBUG_NODE( var        );
            BOOST_SPIRIT_DEBUG_NODE( intval     );
        }

        subrule<0> expression;
        subrule<1> term;
        subrule<2> factor;
        subrule<3> var;
        subrule<4> intval;

        rule<ScannerT> first;

        rule<ScannerT> const& start() const { return first; }
    } ;
} ;

eTermType CExpr::getType(void) const
{
    return eExpr;
}

CExpr * CExpr::factory(const char * pExpr)
{
    CExpr * pRetVal = NULL;

    //
    // parse the expression, the result is left in a global variable if successful
    //
    equation equa; //  parser

    string str( pExpr );

    parse_info<> info = parse(pExpr, equa, space_p);

    if (info.full)
    {
        cout << "-------------------------" << endl;
        cout << "Parsing succeeded"         << endl;
        cout << "-------------------------" << endl;

        if (!exprStack.empty())
        {
            pRetVal = exprStack.back();

            exprStack.pop_back();

            //             stringstream str;

            //             str << "Print expression:" << endl;

            //             pRetVal->eprint( str );

            //             str << ", Value of expression: "
            //                 << pRetVal->getValue()
            //                 << endl;

            while ( !exprStack.empty())
            {
                CExpr * pDummy = exprStack.back();

                exprStack.pop_back();

                delete pDummy;
            }

            //             cout << str.str() << endl;

        }
        //         else
        //         {
        //             cout << "Unexpected empty expression stack ??? " << endl;
        //         }
    }
    else
    {
        stringstream str;

        str << "-------------------------"
            << endl
            << "Parsing failed"
            << endl
            << "stopped at: : \""
            << info.stop
            << "\""
            << endl
            << "-------------------------";

        cout << str.str() << endl;

    }

    //cout << "End of run count value = " << CExpr::getCount() << endl;

    return pRetVal;
}

int CExpr::gCount = 0;

CExpr::CExpr(const char * pFile, int line)
  :
  mFile    ( pFile ),
  mLine    ( line  )
{
    //
    gCount++;
}

CExpr::~CExpr()
{
    if (gCount) --gCount;
}

int CExpr::getCount(void)
{
    return gCount;
}

CExpr * CExpr::convertToSOP( const CExpr * pExpr)
{
    CSum * pRetVal = NULL; // new CSum( __FILE__, __LINE__ );

    return pRetVal;
}

CSum  operator + (CSum  const & rLHS, CSum  const & rRHS)
{
    CSum retVal(rLHS);

    // add the rhs to the sum

    retVal.addTerms( rRHS );

    return retVal;
}

CSum  operator + (CSum  const & rLHS, CProd const & rRHS)
{
    CSum retVal( rLHS );

    retVal.addTerm( rRHS );

    return retVal;
}

CSum  operator + (CProd const & rLHS, CSum  const & rRHS)
{
    CSum retVal( rRHS );

    retVal.addTerm( rLHS );

    return retVal;
}

CSum  operator + (CProd const & rLHS, CProd const & rRHS)
{
    CSum retVal;

    retVal.addTerm( rLHS );
    retVal.addTerm( rRHS );

    return retVal;
}

CSum  operator * (CSum  const & rLHS, CSum  const & rRHS)
{
    CSum retVal( rLHS );

    retVal.multTerms( rRHS );

    return retVal;
}

CSum  operator * (CSum  const & rLHS, CProd const & rRHS)
{
    CSum retVal( rLHS );

    retVal.multTerm( rRHS );

    return retVal;
}

CSum  operator * (CProd const & rLHS, CSum  const & rRHS)
{
    CSum retVal( rRHS );

    retVal.multTerm( rLHS );

    return retVal;
}

CProd operator * (CProd const & rLHS, CProd const & rRHS)
{
    CProd retVal( rLHS );

    retVal.multTerm( rRHS );

    return retVal;
}

void CSum::multTerms( CSum const & sum )
{
    list< CProd * > temp;

    list< CProd * >::const_iterator item = mVars.begin();
    list< CProd * >::const_iterator last = mVars.end();

    while (item != last)
    {
        list< CProd * >::const_iterator innerItem = sum.mVars.begin();
        list< CProd * >::const_iterator innerLast = sum.mVars.end();

        CProd tempProd( *(*item) );

        while (innerItem != innerLast)
        {
            CProd innerProd( *(*innerItem) );

            temp.push_back( new CProd( innerProd * tempProd ) );

            ++innerItem;
        }

        ++item;
    }

    mVars = temp;

    collectTerms();
}

void CSum::multTerm( CProd const & prod )
{
    list< CProd * > temp;

    list< CProd * >::const_iterator item = mVars.begin();
    list< CProd * >::const_iterator last = mVars.end();

    while (item != last)
    {
        CProd tempProd( *(*item) );

        temp.push_back( new CProd( prod * tempProd ) );

        ++item;
    }

    mVars = temp;

    collectTerms();
}
