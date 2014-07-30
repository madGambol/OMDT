
#include <stdio.h>

#include <string>
#include <iostream>
#include <ostream>
#include <sstream>

#include <stack>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

using namespace std;

#include <stdlib.h>

#include "cardinality.hpp"

#include "OMDBase.h"
#include "OMDNode.h"

#include <FlexLexer.h>
#include "omdtLex.h"

#include "OMDTask.h"
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
#include "PrettyPrint.h"
#include "Document.h"

#include "OMDBase.h"
#include "OMDCardinalityParser.h"

//#include "CountComplex.h"
//#include "CountBytes.h"

extern const char * getSym();
FlexLexer * getInstance(void);

extern int yydebug;

int yyparse(void);

void PrintResult( CCardinalityExpr * pBase)
{
    if (!pBase) 
    {
        printf("Error: pBase is NULL\n");
        return;
    }

    CType1 * pType1 = dynamic_cast<CType1 *>(pBase);
    CType2 * pType2 = dynamic_cast<CType2 *>(pBase);
    CType3 * pType3 = dynamic_cast<CType3 *>(pBase);
    CType4 * pType4 = dynamic_cast<CType4 *>(pBase);
    CType5 * pType5 = dynamic_cast<CType5 *>(pBase);
    CType6 * pType6 = dynamic_cast<CType6 *>(pBase);

    if (pType1)
    {
        printf("Type 1 (nnn) : constant = %d\n", pType1->getConst() );
    }
    else if (pType2)
    {
        printf("Type 2 (nnn+): constant = %d\n", pType2->getConst() );
    }
    else if (pType3)
    {
        printf("Type 3 (mmm-nnn): constant lower = %d, constant upper = %d\n", 
               pType3->getConstLower(), 
               pType3->getConstUpper() 
              );
    }
    else if (pType4)
    {
        printf("Type 4 (var): var = [%s]\n", pType4->getFieldName().c_str() );
    }
    else if (pType5)
    {
        CType5a * pType5a = dynamic_cast<CType5a*>(pType5);
        CType5b * pType5b = dynamic_cast<CType5b*>(pType5);

        if (pType5a)
        {
            printf("Type 5a (var op enumerate): var = [%s], comp = [%s], enumerate = [%s]\n",
                   pType5a->getFieldName().c_str(),
                   pType5a->getComparisonString().c_str(),
                   pType5a->getEnumerateName().c_str()
                  );

        }
        else if (pType5b)
        {
            do
            {
                printf("Type 5b (var : enumerateList): var [%s] : ", pType5b->getFieldName().c_str() );

                const list<CardinalityExprTerm *> & terms = pType5b->getTermList();

                list<CardinalityExprTerm *>::const_iterator item = terms.begin();
                list<CardinalityExprTerm *>::const_iterator last = terms.end();

                if (item == last)
                {
                    printf("---> error, missing first term.\n");
                    break;
                }

                CardinalityExprTerm * pTerm = *item;
                printf("%s", pTerm->getString().c_str() );

                ++item;

                while (item != last)
                {
                    pTerm = *item;
                    printf(", %s", pTerm->getString().c_str() );
                    ++item;
                }
            } while (0);

            printf("\n");
        }
    }
    else if (pType6)
    {
        printf("Type 6 (%%nnn): constant = %d\n", pType6->getConst());
    }
}

void TestCardinalityExpressions(void)
{
    do
    {
        OMDCardinalityParser cardExpr;
        CCardinalityExpr * pBase;

        if ((pBase = cardExpr.IsOK(string("123"))))
        {
            printf("Test 1 pass.\n");
            PrintResult( pBase );
        }
        else
        {
            printf("Test 1 failed.\n");
            break;
        }

        if ((pBase = cardExpr.IsOK(string("234+"))))
        {
            printf("Test 2 pass.\n");
            PrintResult( pBase );
        }
        else
        {
            printf("Test 2 failed.\n");
            break;
        }

        if ((pBase = cardExpr.IsOK(string("234-567"))))
        {
            printf("Test 3 pass.\n");
            PrintResult( pBase );
        }
        else
        {
            printf("Test 3 failed.\n");
            break;
        }

        if ((pBase = cardExpr.IsOK(string("A_Variable"))))
        {
            printf("Test 4 pass.\n");
            PrintResult( pBase );
        }
        else
        {
            printf("Test 4 failed.\n");
            break;
        }

        if ((pBase = cardExpr.IsOK(string("A_Variable_5-1 = Enumerate"))))
        {
            printf("Test 5-1 pass.\n");
            PrintResult( pBase );
        }
        else
        {
            printf("Test 5-1 failed.\n");
            break;
        }

        if ((pBase = cardExpr.IsOK(string("A_Variable_5-2a>Enumerate"))))
        {
            printf("Test 5-2a pass.\n");
            PrintResult( pBase );
        }
        else
        {
            printf("Test 5-2a failed.\n");
            break;
        }

        if ((pBase = cardExpr.IsOK(string("A_Variable_5-2b>=Enumerate"))))
        {
            printf("Test 5-2b pass.\n");
            PrintResult( pBase );
        }
        else
        {
            printf("Test 5-2b failed.\n");
            break;
        }

        if ((pBase = cardExpr.IsOK(string("A_Variable_5-2c <> Enumerate"))))
        {
            printf("Test 5-2c pass.\n");
            PrintResult( pBase );
        }
        else
        {
            printf("Test 5-2c failed.\n");
            break;
        }

        if ((pBase = cardExpr.IsOK(string("A_Variable_5-2d !=  Enumerate   "))))
        {
            printf("Test 5-2d pass.\n");
            PrintResult( pBase );
        }
        else
        {
            printf("Test 5-2d failed.\n");
            break;
        }

        if ((pBase = cardExpr.IsOK(string("A_Variable_5-3a : Enumerate"))))
        {
            printf("Test 5-3a pass.\n");
            PrintResult( pBase );
        }
        else
        {
            printf("Test 5-3a failed.\n");
            break;
        }

        if ((pBase = cardExpr.IsOK(string("A-Variable_5-3b: Enumerate .. Enumerate2"))))
        {
            printf("Test 5-3b pass.\n");
            PrintResult( pBase );
        }
        else
        {
            printf("Test 5-3b failed.\n");
        }

        if ((pBase = cardExpr.IsOK(string("A_Variable_5-3c : > Enumerate"))))
        {
            printf("Test 5-3c pass.\n");
            PrintResult( pBase );
        }
        else
        {
            printf("Test 5-3c failed.\n");
            //break;
        }

        if ((pBase = cardExpr.IsOK(string("A_Variable_5-4a : Enumerate1, < Enumerate2"))))
        {
            printf("Test 5-4a pass.\n");
            PrintResult( pBase );
        }
        else
        {
            printf("Test 5-4a failed.\n");
        }

        if ((pBase = cardExpr.IsOK(string("A_Variable_5-4b : Enumerate1 , < Enumerate2, Enumerate3 .. Enumerate4"))))
        {
            printf("Test 5-4b pass.\n");
            PrintResult( pBase );
        }
        else
        {
            printf("Test 5-4b failed.\n");
        }

        if ((pBase = cardExpr.IsOK(string("A_Variable_5-4c : >= Enumerate1, <= Enumerate2 , Enumerate3..Enumerate4"))))
        {
            printf("Test 5-4c pass.\n");
            PrintResult( pBase );
        }
        else
        {
            printf("Test 5-4c failed.\n");
        }

        if ((pBase = cardExpr.IsOK(string("%4"))))
        {
            printf("Test 6a pass.\n");
            PrintResult( pBase );
        }
        else
        {
            printf("Test 6a failed.\n");
        }

        if (!(pBase = cardExpr.IsOK(string("%+4"))))
        {
            printf("Test 6b pass.\n");
        }
        else
        {
            printf("Test 6b failed.\n");
        }

    } while (0);
}

int main(int argc , char ** argv)
{

    /**/

    TestCardinalityExpressions();

    return 0;
}
