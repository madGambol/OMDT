
#ifndef _OMDCardinalityParser_h
#define _OMDCardinalityParser_h

//#include <string>
//#include <list>
//#include <vector>

#include <stdlib.h>
#include <string>

//int atoi(const char *nptr);

#include "cardinality.hpp"

typedef enum
{
    eNONE,
    eTYPE1,    // 123
    eTYPE2,    // 123+
    eTYPE3,    // 123-234
    eTYPE4,    // SymbolicCount
    eTYPE5a,   // 0-1,symbol == value
    eTYPE5b,   // SymbolicEnum : (Enumerate | Enumerate .. Enumerate | comp  Enumerate)
    eTYPE6     //  % (modulo)
    
} CardinalityEnumType;

class CCardinalityExpr
{
    CardinalityEnumType m_eCardinalityType;

protected:

    static size_t gIndefCounter;

public:
    CCardinalityExpr(CardinalityEnumType cardinalityType)
    :
    m_eCardinalityType( cardinalityType )
    {
    }

    virtual ~CCardinalityExpr()
    {
    }

    CardinalityEnumType getCardinalityType(void) const
    {
        return m_eCardinalityType;
    }

    bool IsIndefinite(void) const
    {
        switch ( m_eCardinalityType )
        {
            case eTYPE1:// 123
                break;
            case eTYPE2:// 123+
            case eTYPE3:// 123-234
            case eTYPE4:// SymbolicCount
                return true;
                break;
            case eTYPE5a: // 0-1,symbol=val  or 0-1,(symbol = val) ...
            case eTYPE5b: // SymbolicEnum : (Enumerate | Enumerate .. Enumerate | comp  Enumerate)
            case eTYPE6:  // %
                break;
            default:
                break;
        }
        return false;
    }

    virtual
    std::string getExpr(void) const = 0;
};

//
// 123
//
class CType1 : public CCardinalityExpr
{
    size_t m_Constant;

public:

    CType1(const size_t aConst)
    :
    CCardinalityExpr( eTYPE1 ),
    m_Constant( aConst )
    {
    }

    CType1(const std::string & aConst)
    :
    CCardinalityExpr( eTYPE1 ),
    m_Constant( atoi( aConst.c_str() ) )
    {
    }

    size_t getConst() const
    {
        return m_Constant;
    }

    std::string getExpr(void) const;
};

//
// 123+
//
class CType2 : public CCardinalityExpr
{
    size_t m_Constant;

public:

    CType2(const size_t aConst)
    :
    CCardinalityExpr( eTYPE2 ),
    m_Constant( aConst )
    {
    }

    CType2(const std::string & aConst)
    :
    CCardinalityExpr( eTYPE2 ),
    m_Constant( atoi( aConst.c_str() ) )
    {
    }

    size_t getConst() const
    {
        return m_Constant;
    }

    std::string getExpr(void) const;
};

//
// 123-234
//
class CType3 : public CCardinalityExpr
{
    size_t m_ConstantLower;
    size_t m_ConstantUpper;

public:

    CType3(const size_t aConst1, const size_t aConst2)
    :
    CCardinalityExpr( eTYPE3 ),
    m_ConstantLower(aConst1),
    m_ConstantUpper(aConst2)
    {
    }

    CType3(const std::string & aConst1, const string & aConst2)
    :
    CCardinalityExpr( eTYPE3 ),
    m_ConstantLower( atoi( aConst1.c_str() ) ),
    m_ConstantUpper( atoi( aConst2.c_str() ) )
    {
    }

    size_t getConstLower() const
    {
        return m_ConstantLower;
    }

    size_t getConstUpper() const
    {
        return m_ConstantUpper;
    }

    std::string getExpr(void) const;
};

//
// SymbolicCount
//
class CType4 : public CCardinalityExpr
{
    std::string m_FieldName;

public:

    CType4(const std::string & name)
    :
    CCardinalityExpr( eTYPE4 ),
    m_FieldName(name)
    {
    }

    virtual ~CType4(void)
    {
    }

    std::string getFieldName(void) const
    {
        return m_FieldName;
    }

    std::string getExpr(void) const;
};

//
// SymbolicCount with more...
//
class CType5 : public CCardinalityExpr
{
    std::string m_FieldName;

public:

    CType5(const std::string & name, CardinalityEnumType cardinalityType)
    :
    CCardinalityExpr( cardinalityType ),
    m_FieldName(name)
    {
    }

    virtual ~CType5(void)
    {
    }

    std::string getFieldName(void) const
    {
        return m_FieldName;
    }

    std::string getExpr(void) const;
};

typedef enum
{
    e_Equal           = eeEqual,            
    e_GreaterThan     = eeGreaterThan,       
    e_LessThan        = eeLessThan,        
    e_GreaterOrEqual  = eeGreaterOrEqual,  
    e_LessOrEqual     = eeLessOrEqual,     
    e_NotEqual        = eeNotEqual    

} ComparisonType;

//
// SymbolicEnum (<|>|=|<=|=<|>=|=>|!=) Enumerate
//
class CType5a : public CType5
{
    std::string         m_EnumerateName;
    ComparisonType m_eComparisonType;

public:

    CType5a(const std::string & name)
    :
    CType5(name, eTYPE5a)
    {
    }

    virtual ~CType5a(void)
    {
    }

    void setComparisonType(ComparisonType comp)
    {
        m_eComparisonType = comp;
    }

    ComparisonType getComparisonType(void) const
    {
        return m_eComparisonType;
    }
  
    std::string getComparisonString(void) const;

    void setEnumerateName(const std::string & enumerate)
    {
        m_EnumerateName = enumerate;
    }

    std::string getEnumerateName(void) const
    {
        return m_EnumerateName;
    }
};

class CardinalityExprTerm
{
public:

    virtual ~CardinalityExprTerm()
    {
    }

    virtual std::string getString(void) const = 0;
};

class SingleEnumerate : public CardinalityExprTerm
{
    std::string m_EnumerateName;

public:

    SingleEnumerate(const std::string & enumerate)
    {
        m_EnumerateName = enumerate;
    }

    std::string getName(void) const
    {
        return m_EnumerateName;
    }

    std::string getString(void) const
    {
        return getName();
    }
};

class EnumerateRange : public CardinalityExprTerm
{
    std::string m_EnumerateRangeBegin;
    std::string m_EnumerateRangeEnd;

public:

    EnumerateRange(const std::string & rangeBegin, const std::string & rangeEnd )
    :
    m_EnumerateRangeBegin( rangeBegin ),
    m_EnumerateRangeEnd  ( rangeEnd   )
    {
    }

    std::string getRangeBegin(void) const
    {
        return m_EnumerateRangeBegin;
    }

    std::string getRageEnd   (void) const
    {
        return m_EnumerateRangeEnd;
    }

    std::string getString(void) const
    {
        return getRangeBegin()+std::string(" .. ")+getRageEnd();
    }
};

class EnumerateComparison : public CardinalityExprTerm
{
    std::string m_Comparison;
    std::string m_EnumerateName;

public:
    
    EnumerateComparison( const std::string & comparison, 
                         const std::string & enumerateName
                       )
    :
    m_Comparison( comparison ),
    m_EnumerateName( enumerateName )
    {
    }

    std::string getComparison(void) const
    {
        return m_Comparison;
    }

    std::string getName(void) const
    {
        return m_EnumerateName;
    }

    std::string getString(void) const
    {
        return getComparison()+std::string(" ")+getName();
    }
};

//
// SymbolicEnum : (Enumerate | Enumerate .. Enumerate | comp  Enumerate)
//
class CType5b : public CType5
{
    std::list<CardinalityExprTerm *> m_Terms;

public:

    CType5b(const std::string & name)
    :
    CType5(name, eTYPE5b)
    {
    }

    virtual ~CType5b()
    {
    }

    void addTerm(CardinalityExprTerm * pTerm)
    {
        m_Terms.push_back( pTerm );
    }

    const std::list<CardinalityExprTerm *> & getTermList(void) const
    {
        return m_Terms;
    }
};


//
// %123
//
class CType6 : public CCardinalityExpr
{
    size_t m_Constant;

public:

    CType6(const size_t aConst)
    :
    CCardinalityExpr( eTYPE6 ),
    m_Constant( aConst )
    {
    }

    CType6(const std::string & aConst)
    :
    CCardinalityExpr( eTYPE6 ),
    m_Constant( atoi( aConst.c_str() ) )
    {
    }

    virtual ~CType6()
    {
    }

    size_t getConst() const
    {
        return m_Constant;
    }

    std::string getExpr(void) const;
};

class OMDCardinalityParser
{
public:

    OMDCardinalityParser();
    ~OMDCardinalityParser();

    CCardinalityExpr * IsOK(const std::string & input) const;

};

// Local Variables:
// mode:C++
// End:

#endif
