/*
 * Abstract interface for simple expression parser, defined for integer
 * 
 */
#ifndef _expr_hh
#define _expr_hh

#include "CPrint.h"
#include "eprint.hh"
#include <string>
#include <cstdlib>

typedef enum { eExpr,
               eSum,
               eProd
             } eTermType;

    
class CExpr : public EPrint    // print symbolic expression
{

protected:
    
    std::string mFile;
    size_t mLine;
    
    CExpr(const char * pFile = __FILE__ , int line = __LINE__);

    // total expression count
    
    static int gCount;
    
public:

    virtual ~CExpr();

    virtual eTermType getType(void) const = 0;

//     virtual
//     int getValue(void) const = 0;

    //
    // Expr is a simple value (rather than a variable or expression)
    //
    virtual
    bool isValue(void) const = 0;
    
    //
    // Evaluate the Expr, and if its not zero, return true.
    //
    virtual
    bool   isNonZero(void) const = 0;

    //
    // Evaluate as a modulo expression: use the relation
    //
    // mod( a*b ) = mod(a) * mod(b)
    // mod( a+b ) = mod(a) + mod(b)
    //
    // The objective is to see if the value of the expression is
    // 0 for a modulus value of 2, 4, 8, ... for the purposes of
    // verifying alignment to a 2, 4, 8, ... byte boundary.
    //
    // Reducing the expression to a sum-of-products with a single
    // numeric coefficient (the others being variables) for each
    // product will allow the evaluation of the expression for this
    // alignment check.
    //
    // returns TRUE if getValue() % modulus == 0
    //
    virtual
    bool   evalMod (int modulus) const = 0;

    virtual
    CExpr * simplify(void) = 0;

    virtual
    CExpr * clone(void) const = 0;
    
    static
    CExpr * factory(const char * pExpr);

    static
    CExpr * convertToSOP( const CExpr * pExpr);

    static int getCount(void);
};

#endif //  _expr_hh
