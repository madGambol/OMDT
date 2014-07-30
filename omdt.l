%{

#include <stdlib.h>
#include <stdio.h>

#include <ostream>
#include <sstream>
#include <iomanip>
#include <stack>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

using namespace std;

class CNoteText;
#include "omdtLex.h"

#include "OMDBase.h"
#include "OMDNode.h"

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

/* Define local functions for lexing */ 

static unsigned int lineNumber = 1;

static const char * pLastSym = "--none--";

list<string> allSymbols;

std::string lyytext;

typedef struct
{
  int open;
  int close;
} OpenCloseStruct;

//const
OpenCloseStruct openClose[] =
{
  {OPEN_eAccuracy,              CLOSE_eAccuracy             },
  {OPEN_eAccuracyCondition,     CLOSE_eAccuracyCondition    },
  {OPEN_eApplicationDomain,     CLOSE_eApplicationDomain    },
  {OPEN_eAttribute,             CLOSE_eAttribute            },
  {OPEN_eAutoSequence,          CLOSE_eAutoSequence         },
  {OPEN_eBitfieldDataType,      CLOSE_eBitfieldDataType     },
  {OPEN_eBitfieldComponent,     CLOSE_eBitfieldComponent    },
  {OPEN_eCardinality,           CLOSE_eCardinality          },
  {OPEN_eClass,                 CLOSE_eClass                },
  {OPEN_eComplexComponent,      CLOSE_eComplexComponent     },
  {OPEN_eComplexDataType,       CLOSE_eComplexDataType      },
  {OPEN_eDIF,                   CLOSE_eDIF                  },
  {OPEN_eDataPos,               CLOSE_eDataPos              },
  {OPEN_eDataType,              CLOSE_eDataType             },
  {OPEN_eDataWidth,             CLOSE_eDataWidth            },
  {OPEN_eDeliveryCategory,      CLOSE_eDeliveryCategory     },
  {OPEN_eDescription,           CLOSE_eDescription          },
  {OPEN_eDimension,             CLOSE_eDimension            },
  {OPEN_eDimensionMaximum,      CLOSE_eDimensionMaximum     },
  {OPEN_eDimensionMinimum,      CLOSE_eDimensionMinimum     },
  {OPEN_eDimensionType,         CLOSE_eDimensionType        },
  {OPEN_eDocument,              CLOSE_eDocument             },
  {OPEN_eEnumeratedDataType,    CLOSE_eEnumeratedDataType   },
  {OPEN_eEnumeration,           CLOSE_eEnumeration          },
  {OPEN_eEnumerator,            CLOSE_eEnumerator           },
  {OPEN_eFEDname,               CLOSE_eFEDname              },
  {OPEN_eFieldName,             CLOSE_eFieldName            },
  {OPEN_eID,                    CLOSE_eID                   },
  {OPEN_eISRType,               CLOSE_eISRType              },
  {OPEN_eInteraction,           CLOSE_eInteraction          },
  {OPEN_eIntervalType,          CLOSE_eIntervalType         },
  {OPEN_eMOMClass,              CLOSE_eMOMClass             },
  {OPEN_eMOMComplexDataType,    CLOSE_eMOMComplexDataType   },
  {OPEN_eMOMEnumeratedDataType, CLOSE_eMOMEnumeratedDataType},
  {OPEN_eMOMInteraction,        CLOSE_eMOMInteraction       },
  {OPEN_eMOMVersion,            CLOSE_eMOMVersion           },
  {OPEN_eMessageOrdering,       CLOSE_eMessageOrdering      },
  {OPEN_eModificationDate,      CLOSE_eModificationDate     },
  {OPEN_eName,                  CLOSE_eName                 },
  {OPEN_eNormalizationFunction, CLOSE_eNormalizationFunction},
  {OPEN_eNote,                  CLOSE_eNote                 },
  {OPEN_eNoteNumber,            CLOSE_eNoteNumber           },
  {OPEN_eNoteText,              CLOSE_eNoteText             },
  {OPEN_eOMDT,                  CLOSE_eOMDT                 },
  {OPEN_eObjectModel,           CLOSE_eObjectModel          },
  {OPEN_ePOCEmail,              CLOSE_ePOCEmail             },
  {OPEN_ePOCFirstName,          CLOSE_ePOCFirstName         },
  {OPEN_ePOCHonorificName,      CLOSE_ePOCHonorificName     },
  {OPEN_ePOCLastName,           CLOSE_ePOCLastName          },
  {OPEN_ePOCOrgName,            CLOSE_ePOCOrgName           },
  {OPEN_ePOCPhone,              CLOSE_ePOCPhone             },
  {OPEN_ePSCapabilities,        CLOSE_ePSCapabilities       },
  {OPEN_eParameter,             CLOSE_eParameter            },
  {OPEN_ePurpose,               CLOSE_ePurpose              },
  {OPEN_ePurpose,               CLOSE_ePurpose              },
  {OPEN_eRange,                 CLOSE_eRange                },
  {OPEN_eRangeSetUnits,         CLOSE_eRangeSetUnits        },
  {OPEN_eRepresentation,        CLOSE_eRepresentation       },
  {OPEN_eResolution,            CLOSE_eResolution           },
  {OPEN_eRoutingSpace,          CLOSE_eRoutingSpace         },
  {OPEN_eSponsorOrgName,        CLOSE_eSponsorOrgName       },
  {OPEN_eStartValue,            CLOSE_eStartValue           },
  {OPEN_eSuperClass,            CLOSE_eSuperClass           },
  {OPEN_eSuperInteraction,      CLOSE_eSuperInteraction     },
  {OPEN_eTYPE,                  CLOSE_eTYPE                 },
  {OPEN_eTransferAccept,        CLOSE_eTransferAccept       },
  {OPEN_eType,                  CLOSE_eType                 },
  {OPEN_eUnits,                 CLOSE_eUnits                },
  {OPEN_eUpdateCondition,       CLOSE_eUpdateCondition      },
  {OPEN_eUpdateReflect,         CLOSE_eUpdateReflect        },
  {OPEN_eUpdateType,            CLOSE_eUpdateType           },
  {OPEN_eVersionNumber,         CLOSE_eVersionNumber        },
};

int _stack[ 4096*16 ];

const char * strStack[4096*16];

int stkIndex = 0;

 void PushTok(int token, const char * pToken)
 {
   strStack[ stkIndex   ] = pToken;
   _stack  [ stkIndex++ ] = token;
 }

 int PopTok(void)
 {
   size_t number = sizeof(openClose)/sizeof(OpenCloseStruct);

   int retVal = CPAREN;
   const char * pToken = "not found";

   do
   {
     if (stkIndex == 0) 
     {
       fprintf(stderr, "Stack index is 0 before stack pop.\n");
       break;
     }

     int tos = _stack[ --stkIndex ];

     const char * pTemp = strStack[ stkIndex ];

     size_t i;

     for (i = 0; i < number; i++)
     {
       if (tos == openClose[i].open)
       {
	 retVal = openClose[i].close;
	 pToken = pTemp;
	 break;
       }
     }

     if (i == number)
     {
       fprintf(stderr, "matching paren not found [%s]\n", pToken);
     }

   } while (0);

   //printf("{%4d/%2d} Popping [%s]\n", lineNumber, stkIndex+1, pToken);

   return retVal;
 }

#define DORETURN(x)   allSymbols.push_back( pLastSym ); pLastSym = #x; yylval.pStr = strdup(yytext); return (x)
#define DOTOKRTN(x)   allSymbols.push_back( pLastSym ); pLastSym = #x; yylval.tokn = yytokentype(x); return (x)

#define STACKOP(x)    allSymbols.push_back( pLastSym ); pLastSym = #x; lyytext = yytext; PushTok( x, pLastSym ); return x
#define POPSTACK      allSymbols.push_back( pLastSym ); pLastSym = "CLOSE"; lyytext = yytext; return PopTok()

// QUOTED_STRING       ["]([^"]|[\\]["])*["]
//QUOTED_STRING       ["]([^"\n\r]|[\\]["])*["]
%}

%option noyywrap
%option debug
%option yyclass="omdtLex"
%option c++
%option yylineno
%option stack
%option nounput
%option noreject

OPEN                [(]([ \t\r\n]|[;][^\n]*\n)*
OPEN2               [(][;][^\n]*\n

DIGIT               [0-9]
MM                  01|02|03|04|05|06|07|08|09|10|11|12
DD                  [0-3][0-9]
YY                  [1-9][0-9][0-9][0-9]

DGTS                [0-9]+

SS                  [ \t\r\n]
RR                  [ \t\r\n)]

SLASH               [/]

OJUNK               [&][=][&]
OQUOTE              ["]
CQUOTE              ["]
NONQUOTE            ([^"\n\r]|[\\]["])*
BROKEN              ([^"]|[\\]["]|[\n\r])*

QUOTED_STRING       ["]([^"\n\r]|[\\]["])*["]
UNQUOTED_STRING     [a-zA-Z_][\-a-zA-Z0-9_]*

COMMENT             ;.*$

FOM                 "FOM"
SOM                 "SOM"
FED                 "FED"
OMT                 "OMT"
OMD                 "OMD"
YES                 [Yy][Ee][Ss]
NO                  [Nn][Oo]
Y                   [Yy]
N                   [Nn]

PSN                 [Nn]|[Pp]|[Ss]|[Pp][Ss]|[Ss][Pp]
NIR                 [Nn]|[Rr]|[Ii][Rr]

%s OPN
%%
 
"["                                   { DORETURN(OSQRBRACKET);            }
"]"                                   { DORETURN(CSQRBRACKET);            }
","                                   { DORETURN(COMMA);                  }

{OJUNK}                               {

  // gather all chars until an unescaped double quote



                                      }
{OQUOTE}{NONQUOTE}{CQUOTE}            { DORETURN(QUOTEDSTR);              }
{OQUOTE}{BROKEN}{CQUOTE}              { DORETURN(BADQUOTEDSTR);           }
({FOM}|{SOM}|{FED}|{OMT})/{RR}        { DORETURN(DOCTYP);                 }

"v"{DGTS}("."{DGTS})*                 { DORETURN(eVersionString);         }

{UNQUOTED_STRING}                     { //printf("UNQUOTEDSTR =[%s]\n", yytext);
                                        DORETURN(UNQUOTEDSTR);            }
{MM}{SLASH}{DD}{SLASH}{YY}            { DORETURN(DATESTR);                }

<OPN>"AccuracyCondition"/{SS}         { STACKOP(OPEN_eAccuracyCondition); }
<OPN>"Accuracy"/{SS}                  { STACKOP(OPEN_eAccuracy);          }
<OPN>"ApplicationDomain"/{SS}         { STACKOP(OPEN_eApplicationDomain); }
<OPN>"Attribute"/{SS}                 { STACKOP(OPEN_eAttribute);         }
<OPN>"AutoSequence"/{SS}              { STACKOP(OPEN_eAutoSequence);      }
<OPN>"BitfieldComponent"/{SS}         { STACKOP(OPEN_eBitfieldComponent); }
<OPN>"BitfieldDataType"/{SS}          { STACKOP(OPEN_eBitfieldDataType);  }
<OPN>"Cardinality"/{SS}               { STACKOP(OPEN_eCardinality);       }
<OPN>"Class"/{SS}                     { STACKOP(OPEN_eClass);             }
<OPN>"ComplexComponent"/{SS}          { STACKOP(OPEN_eComplexComponent);  }
<OPN>"ComplexDataType"/{SS}           { STACKOP(OPEN_eComplexDataType);   }
<OPN>"DataType"/{SS}                  { STACKOP(OPEN_eDataType);          }
<OPN>"DeliveryCategory"/{SS}          { STACKOP(OPEN_eDeliveryCategory);  }
<OPN>"Description"/{SS}               { STACKOP(OPEN_eDescription);       }
<OPN>"DIF"/{SS}                       { STACKOP(OPEN_eDIF);               }
<OPB>"DataPos"/{SS}                   { STACKOP(OPEN_eDataPos);           }
<OPB>"DataWidth"/{SS}                 { STACKOP(OPEN_eDataWidth);         }
<OPN>"DimensionMaximum"/{SS}          { STACKOP(OPEN_eDimensionMaximum);  }
<OPN>"DimensionMinimum"/{SS}          { STACKOP(OPEN_eDimensionMinimum);  }
<OPN>"DimensionType"/{SS}             { STACKOP(OPEN_eDimensionType);     }
<OPN>"Dimension"/{SS}                 { STACKOP(OPEN_eDimension);         }
<OPN>"EnumeratedDataType"/{SS}        { STACKOP(OPEN_eEnumeratedDataType);}
<OPN>"Enumeration"/{SS}               { STACKOP(OPEN_eEnumeration);       }
<OPN>"Enumerator"/{SS}                { STACKOP(OPEN_eEnumerator);        }
<OPN>"FEDname"/{SS}                   { STACKOP(OPEN_eFEDname);           }
<OPN>"FieldName"/{SS}                 { STACKOP(OPEN_eFieldName);         }
<OPN>"ID"/{SS}                        { STACKOP(OPEN_eID);                }
<OPN>"Interaction"/{SS}               { STACKOP(OPEN_eInteraction);       }
<OPN>"IntervalType"/{SS}              { STACKOP(OPEN_eIntervalType);      }
<OPN>"ISRType"/{SS}                   { STACKOP(OPEN_eISRType);           }
<OPN>"MessageOrdering"/{SS}           { STACKOP(OPEN_eMessageOrdering);   }
<OPN>"ModificationDate"/{SS}          { STACKOP(OPEN_eModificationDate);  }
<OPN>"MOMVersion"/{SS}                { STACKOP(OPEN_eMOMVersion);        }
<OPN>"MOMClass"/{SS}                  { STACKOP(OPEN_eMOMClass);          }
<OPN>"MOMInteraction"/{SS}            { STACKOP(OPEN_eMOMInteraction);        }
<OPN>"MOMEnumeratedDataType"/{SS}     { STACKOP(OPEN_eMOMEnumeratedDataType); }
<OPN>"MOMComplexDataType"/{SS}        { STACKOP(OPEN_eMOMComplexDataType);    }
<OPN>"Name"/{SS}                      { STACKOP(OPEN_eName);                  }
<OPN>"NormalizationFunction"/{SS}     { STACKOP(OPEN_eNormalizationFunction); }
<OPN>"NoteText"/{SS}                  { STACKOP(OPEN_eNoteText);          }
<OPN>"NoteNumber"/{SS}                { STACKOP(OPEN_eNoteNumber);        }
<OPN>"Note"/{SS}                      { STACKOP(OPEN_eNote);              }
<OPN>"ObjectModel"/{SS}               { STACKOP(OPEN_eObjectModel);       }
<OPN>"OMDT"/{SS}                      { STACKOP(OPEN_eOMDT);              }
<OPN>"Parameter"/{SS}                 { STACKOP(OPEN_eParameter);         }
<OPN>"POCEmail"/{SS}                  { STACKOP(OPEN_ePOCEmail);          }
<OPN>"POCFirstName"/{SS}              { STACKOP(OPEN_ePOCFirstName);      }
<OPN>"POCHonorificName"/{SS}          { STACKOP(OPEN_ePOCHonorificName);  }
<OPN>"POCLastName"/{SS}               { STACKOP(OPEN_ePOCLastName);       }
<OPN>"POCOrgName"/{SS}                { STACKOP(OPEN_ePOCOrgName);        }
<OPN>"POCPhone"/{SS}                  { STACKOP(OPEN_ePOCPhone);          }
<OPN>"PSCapabilities"/{SS}            { STACKOP(OPEN_ePSCapabilities);    }
<OPN>"Purpose"/{SS}                   { STACKOP(OPEN_ePurpose);           } 
<OPN>"Range"/{SS}                     { STACKOP(OPEN_eRange);             }
<OPN>"RangeSetUnits"/{SS}             { STACKOP(OPEN_eRangeSetUnits);     }
<OPN>"Representation"/{SS}            { STACKOP(OPEN_eRepresentation);    }
<OPN>"Resolution"/{SS}                { STACKOP(OPEN_eResolution);        }
<OPN>"RoutingSpace"/{SS}              { STACKOP(OPEN_eRoutingSpace);      }
<OPN>"SponsorOrgName"/{SS}            { STACKOP(OPEN_eSponsorOrgName);    }
<OPN>"StartValue"/{SS}                { STACKOP(OPEN_eStartValue);        }
<OPN>"SuperClass"/{SS}                { STACKOP(OPEN_eSuperClass);        }
<OPN>"SuperInteraction"/{SS}          { STACKOP(OPEN_eSuperInteraction);  }
<OPN>"TransferAccept"/{SS}            { STACKOP(OPEN_eTransferAccept);    }
<OPN>"Type"/{SS}                      { STACKOP(OPEN_eType);              }
<OPN>"TYPE"/{SS}                      { STACKOP(OPEN_eTYPE);              }
<OPN>"Units"/{SS}                     { STACKOP(OPEN_eUnits);             }
<OPN>"UpdateCondition"/{SS}           { STACKOP(OPEN_eUpdateCondition);   }
<OPN>"UpdateReflect"/{SS}             { STACKOP(OPEN_eUpdateReflect);     }
<OPN>"UpdateType"/{SS}                { STACKOP(OPEN_eUpdateType);        }
<OPN>"VersionNumber"/{SS}             { STACKOP(OPEN_eVersionNumber);     }

"("                                   { //STACKOP(OPAREN);
                                        BEGIN(OPN);
                                      }
")"                                   { BEGIN(INITIAL); POPSTACK;         }

{DGTS}                                { DORETURN(NUMBER);                 }

{COMMENT}                              /* ignore */

[ \t\r]                                {
                                         // ignore
                                       }

[\n]                                   { 
                                         lineNumber++; 
                                         //printf("On Line %d <<<<<<<<<<<<<<<<\n", lineNumber);
                                       }
                                /* 
 pick up all other characters
                                */
.                               {
                                   printf( "Unrecognized character at line %d: \"%s\"\n", lineNumber, yytext );

                                   char ch = '?';
                                   if (yytext) ch = *yytext;

                                   return ch;
                                }

%%

/* implementation of routines goes here */

/*
"v"{DGTS}("."{DGTS})*            {
                                   //printf("eVersionString = %s\n", yytext);
                                   DORETURN(eVersionString);    
                                 }
*/

const char * getSym(void)
{
  return pLastSym;
}

const char * getPrevSym(void)
{
  static string retVal;

  list<string>::reverse_iterator item = allSymbols.rbegin();
  list<string>::reverse_iterator last = allSymbols.rend();

  size_t count = 6;

  while (item != last)
  {
    retVal += (*item)+string("/");

    ++item;

    --count;

    if (count == 0) break;
  }

  return retVal.c_str();
}

// FlexLexer * getInstance(void)
// {
//   static FlexLexer * pInstance = new yyFlexLexer;

//   return pInstance;
// }

/*
int yylex()
{
  return getInstance()->yylex();
}
*/


int yylex()
{
  return omdtLex::getInstance()->yylex();
}

int getLine()
{
  return lineNumber;
}

/*
{PSN}/[)]                        { DORETURN(PSALT);             }
{PSN}/{SS}                       { DORETURN(PSALT);             }
{YES}|{NO}                       { DORETURN(YESNO);             }
{Y}|{N}                          { DORETURN(Y_OR_N);            }
{NIR}                            { DORETURN(ISR_ALT);           }

RR                  [ \t\n)]

*/

// Local Variables:
// mode:C++
// End:

