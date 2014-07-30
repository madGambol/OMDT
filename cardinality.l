%{
    int atoi(const char *nptr);
    
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <stack>
#include <string>
#include <ostream>
#include <sstream>
#include <iomanip>
    
using namespace std;

#include "yystype.h"
#include "cardinality.hpp"

/* Define local functions for lexing */

static unsigned int lineNumber = 1;

static const char * pLastSym = "--none--";

#define DORETURN(x)   pLastSym = #x; cclval = yytext; return (x)

FlexLexer * getCardInstance(void);

#define YY_INPUT(buf,result,max_size) \
      if ((result = getFlexBuffer().Input(buf,max_size))<0)\
        YY_FATAL_ERROR("input in flexBuffer (scanner) failed.");

class CFlexBuffer
{
  string m_Buffer;

public:

  CFlexBuffer(const string & input)
    :
    m_Buffer( input )
  {
  }

  int Input(char * pBuf, size_t bufSize)
  {
    if (!pBuf) return 0;

    if (bufSize == 0) return 0;

    size_t retVal = (bufSize < m_Buffer.size()) ? bufSize : m_Buffer.size();

    strncpy(pBuf, m_Buffer.c_str(), retVal);

    //
    // Update the m_Buffer to clear out the data just sent out
    //
    m_Buffer = m_Buffer.substr( retVal );

    return retVal;
  }

  void setBuffer(const string & newBuffer)
  {
    m_Buffer = newBuffer;
  }
};

CFlexBuffer & getFlexBuffer( const string & buffer = string() )
{
  static CFlexBuffer localFlexBuffer( buffer );

  return localFlexBuffer;
}

void SetNewBuffer(const string & newBuffer)
{
  getFlexBuffer().setBuffer( newBuffer );
}
 

%}

%option noyywrap
%option yy_scan_buffer


NUMBER              [0-9]+

NAME                [a-zA-Z][a-zA-Z0-9_+-]+

EQ                  [=]
GT                  [>]
LT                  [<]
NE                  [!][=]

DASH                [-]
DOTS                \.\.

COLON               [:]
OPAREN              [(]
CPAREN              [)]
PLUS                [+]

COMMA               [,]
MODULO              [%]

SS                  [ ,\t\r\n]

%%

{NUMBER}                  { DORETURN( eeNumber   ); }
{NAME}                    { DORETURN( eeName     ); }
{OPAREN}                  { DORETURN( eeOparen   ); }
{CPAREN}                  { DORETURN( eeCparen   ); }
{COMMA}                   { DORETURN( eeComma    ); }
{COLON}                   { DORETURN( eeColon    ); }
{DASH}                    { DORETURN( eeDash     ); }
{PLUS}                    { DORETURN( eePlus     ); }
{MODULO}                  { DORETURN( eeModulo   ); }

{EQ}                      { DORETURN( eeEqual          ); }
{GT}                      { DORETURN( eeGreaterThan    ); }
{LT}                      { DORETURN( eeLessThan       ); }
{NE}|{GT}{LT}|{LT}{GT}    { DORETURN( eeNotEqual       ); }
{GT}{EQ}|{EQ}{GT}         { DORETURN( eeGreaterOrEqual ); }
{LT}{EQ}|{EQ}{LT}         { DORETURN( eeLessOrEqual    ); }

{DOTS}                    { DORETURN( eeElipses  ); }

[ \t\r]                         /* ignore */
[\n]                      { 
                                   lineNumber++; 
                                   //printf("On Line %d <<<<<<<<<<<<<<<<\n", lineNumber);
                          }

                                /* 
 pick up all other characters
                                */
.                         {
  printf( "Unrecognized character at line %d: \"%s\"\n",
	  lineNumber, 
	  yytext 
	  );
  
  char ch = '?';
  if (yytext) ch = *yytext;
  
  return ch;
                          }
%%


int getCardLine()
{
  return lineNumber;
}

const char * getCardSym(void)
{
  return pLastSym;
}

FlexLexer * getCardInstance(void)
{
  static FlexLexer * pInstance = new ccFlexLexer;

  return pInstance;
}


// Local Variables:
// mode:C++
// End:
