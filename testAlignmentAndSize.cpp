
#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>

#include <stack>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

using namespace std;

#include <stdlib.h>

#include "OMDBase.h"
#include "OMDNode.h"

#include <FlexLexer.h>
#include "omdtLex.h"

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
#include "Document.h"

#include "eprint.hh"
#include "expr.hh"

#include "AlignmentAndSize.h"

//#include "OMDBase.h"
//#include "OMDCardinalityParser.h"

extern const char * getSym();
FlexLexer * getInstance(void);

extern int yydebug;

int yyparse(void);

const char * pTestFile = "ntf.omd";

int main(int argc, char ** argv)
{
    const char * pReason = "";

    string testFile ( argv[1] );

    int parseResult = 0;

    try
    {   
        omdtLex::getInstance( testFile.c_str() );

        pReason     = "parsing";

        parseResult = yyparse();

        cerr << "yyparse returned [" << parseResult << "]" << endl;
 
    }
    catch (const char * pMsg)
    {
        cerr << "Exception(Reason: " << pReason << ") : " << pMsg << endl;
    }
    catch (...)
    {
        cerr << "Unpected Exception; Reason = (" << pReason << ")" << endl;
    }
  
    cerr << "Done parsing." << endl;

    if (parseResult > 0)
    {
        cout << endl
             << "Syntax errors encountered in \"" 
             << testFile
             << "\""
             << endl
             << "Further processing abandoned." 
             << endl;

        exit(-1);
    }

    COMDocument * pDoc = COMDocument::getRoot();

    AlignmentAndSize aas( pDoc );

    aas.Process();

    stringstream str;

    aas.print( str );

    cout << str.str() << endl;
    
    return 0;
}
