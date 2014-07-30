
// #include <sys/types.h>
// #include <regex.h>

#include <string>
#include <sstream>
#include <ostream>
#include <list>
#include <vector>

using namespace std;

#include <stdlib.h>

#include "OMDCardinalityParser.h"

#undef yyFlexLexer
#define yyFlexLexer ccFlexLexer
#include <FlexLexer.h>

string CType5a::getComparisonString(void) const
{
  switch (m_eComparisonType)
  {
  case e_Equal:
    return string("=");
  case e_GreaterThan:
    return string(">");
  case e_LessThan:
    return string("<");
  case e_GreaterOrEqual:
    return string(">=");
  case e_LessOrEqual:
    return string("<=");
  case e_NotEqual:
    return string("!=");
  default:
    break;
  }

  return string("??");
}


OMDCardinalityParser::OMDCardinalityParser()
{
}

OMDCardinalityParser::~OMDCardinalityParser()
{
}

string stripQuotes(const string & input);

extern CCardinalityExpr * pResult;

FlexLexer * getCardInstance();

void SetNewBuffer(const string & newBuffer);

int ccparse();

CCardinalityExpr * OMDCardinalityParser::IsOK(const string & expression) const  // input string to be tested
{

  CCardinalityExpr * pRetValue = NULL;

  string input = stripQuotes( expression );

  //
  // Reinitialize the flex parser
  //

  //
  // set up buffer for parsing
  // 
  SetNewBuffer( input );

  //
  // invoke the parser
  //
  ccparse();

  if (pResult)
  {
    pRetValue = pResult;
  }

  pResult = NULL;

  return pRetValue;
}

string CType1::getExpr(void) const
{
    stringstream retVal;

    retVal << m_Constant;

    return retVal.str();
}

size_t CCardinalityExpr::gIndefCounter = 1;

string CType2::getExpr(void) const
{
    stringstream retVal;   // holder for indef variable

    retVal << "__2_" << gIndefCounter++ << "__";

    return retVal.str();
}

string CType3::getExpr(void) const
{
    stringstream retVal;   // holder for indef variable

    retVal << "__3_" << gIndefCounter++ << "__";

    return retVal.str();
}

string CType4::getExpr(void) const
{
    return m_FieldName;
}

string CType5::getExpr(void) const
{
    stringstream retVal;   // holder for indef variable

    retVal << "__5_" << gIndefCounter++ << "__";

    return retVal.str();
}

string CType6::getExpr(void) const
{
    stringstream retVal;   // holder for indef variable

    retVal << "__6_" << gIndefCounter++ << "__";

    return retVal.str();
}

