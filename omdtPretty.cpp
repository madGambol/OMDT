
#include <stdlib.h>
#include <stdio.h>

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

#include "CPrint.h"

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

//#include "OMDBase.h"
//#include "OMDCardinalityParser.h"

//#include "CountComplex.h"
//#include "CountBytes.h"

extern const char * getSym();
FlexLexer * getInstance(void);

extern int yydebug;

int yyparse(void);

void Usage(int argc , char ** argv)
{
  cerr << "Usage:" << endl
       << "      " << argv[0]
       << " <options> <filename1> [[<options>] <filename2>]*"
       << endl
       << endl
       << "where: <options> are one of the following:"
       << endl
       << "       TBS"
       << endl;
}

bool isOptions(const char * pArg)
{
  return (*pArg == '-');
}

int main(int argc , char ** argv)
{
  int index = 1;

  do
  {
    if (argc == 1) 
    {
      Usage(argc, argv);
      exit(-1);
    }

    index = 1;

    while (index < argc)
    {
      string arg_n = argv[index];

      cerr << "arg # " << index << " --> " << arg_n << endl;
 
      ++index;
    }

  } while (0);

  /**/

  //TestCardinalityExpressions();

#ifdef YYDEBUG
#if YYDEBUG==1
  yydebug = 0;
#endif
#endif

  const char * pReason = "";

  int parseResult = 0;

  index = 1;

  while (index < argc)
  {
    try
    {
      if ( isOptions(argv[index]) )
      {
	// set options
	
	cerr << "Fake option : \"" << argv[index] << "\"" << endl;

	++index;

	continue;
      }

      omdtLex::getInstance( argv[index] );

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
      cerr << endl
	   << "Syntax errors encountered in \"" 
	   << argv[index] 
	   << endl
	   << "Further processing abandoned." 
	   << endl;

      exit(-1);
    }

    COMDocument * pDoc = COMDocument::getRoot();

    cerr << "COMDocument.size() == " << pDoc->getChildCount() << endl;

    PrettyPrint prettyPrint( pDoc );

    prettyPrint.Process();

    cout << prettyPrint << endl;

    //DocumentCheck check( pDoc );

    //cerr << check;

    delete pDoc;

    ++index;
  }

  cerr << "done." << endl;

  /**/
  return 0;
}
// Local Variables:
// mode:C++
// End:
