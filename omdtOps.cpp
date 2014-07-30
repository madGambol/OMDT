
#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <list>
#include <map>
#include <set>

using namespace std;

#include <stdlib.h>

#include <FlexLexer.h>
#include "omdtLex.h"

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
#include "Document.h"
#include "check.h"

//#include "OMDBase.h"
//#include "OMDCardinalityParser.h"

//#include "CountComplex.h"
//#include "CountBytes.h"

#include <string>

#include <stdio.h>

const char * pVersion = "omdtOps v0.0";
using namespace std;

extern const char * getSym();
FlexLexer * getInstance(void);

extern int yydebug;

int yyparse(void);

typedef enum
  {
    eCHECK,           // just check the file for errors
    eCOMPARE,         // compare two files
    eEXTRACT,         // extract objects/interactions to form a BOM
    eMERGE,           // merge N files
    eEXPLODE          // each object/interaction instance in a separate file
  } omdtOperations;

omdtOperations getOperation(const char * pArg)
{
  string arg( pArg );

  if ((arg == "-c") || (arg == "--compare"))
  {
    return eCOMPARE;
  }
  else if ((arg == "-e") || (arg == "--extract"))
  {
    return eEXTRACT;
  } 
  else if ((arg == "-m") || (arg == "--merge"))
  {
    return eMERGE;
  }
  else if ((arg == "-x") || (arg == "--explode"))
  {
    return eEXPLODE;
  }
  else if ((arg == "--check"))
  {
    return eCHECK;
  }
  else
  {
  }

  return eCHECK;
}

void Usage(int argc , char ** argv)
{
  stringstream str;

  str << "Usage:" << endl
      << "      " << argv[0]
      << " <options> <filename1> [<filename2>]*"
      << endl
      << " "
      << endl
      << "where: <options> are one of the following:"
      << endl
      << "       -c or --compare : compares two files"
      << endl
      << "       -e\"O,I\" or " 
      << endl 
      << "       --extract=\"O,I\" : extracts objects or interactions and" 
      << endl
      << "                         O/I are a comma delimited list of object/interaction ID's or names"
      << endl
      << "       -m or --merge   : merge two or more files"
      << endl
      << "       -x or --explode : explode an OMD/OMT file into BOM-lettes"
      << endl
      << "       -v or --version : print the version. This version is (" 
      << pVersion 
      << ")";

cerr  << str.str()
      << endl
      << endl
      << endl;
}

bool isOptions(const char * pArg)
{
  return (*pArg == '-');
}

class DocMap : public map<string, COMDocument *>
{
public:
  DocMap()
  {
  }

  ~DocMap()
  {
    map<string, COMDocument*>::iterator item = begin();
    map<string, COMDocument*>::iterator last = end();

    while (item != last)
    {
      map<string, COMDocument*>::iterator thisOne = item++;

      COMDocument * pItem = (*thisOne).second;

      erase( thisOne );

      delete pItem;
    }
  }
};

class MapDoc : public map<COMDocument *, string>
{
public:
  MapDoc()
  {
  }

  ~MapDoc()
  {
  }
};

DocMap  docMap;
MapDoc  mapDoc;

class DocList : public list<COMDocument *>
{
public:

  DocList(){
  }

  ~DocList() 
  {
  }
};

class DocOp
{
protected:

  string mArgs;

public:

  DocOp(const char * pArgs)  
    :
    mArgs( pArgs )
  {
  }

  virtual ~DocOp() 
  {
  }

  virtual const char * getName(void) const = 0;

  virtual bool apply( DocList & docList )
  {
    cerr << "Operation " << getName() << " not implemented." << endl;

    return false;
  }
};

class DocCompare : public DocOp
{
public:

  DocCompare(const char * pArgs)
    :
    DocOp ( pArgs )
  {
  }

  virtual ~DocCompare() {}

  virtual const char * getName(void) const 
  {
    return "Compare";
  }

  virtual bool apply( DocList & docList );
};

class DocExtract : public DocOp
{
public:

  DocExtract(const char * pArgs) 
    :
    DocOp ( pArgs )
  {
  }

  virtual ~DocExtract() {}
  
  virtual const char * getName(void) const 
  {
    return "Extract";
  }

  virtual bool apply( DocList & docList );
};

class DocExplode : public DocOp
{
public:

  DocExplode(const char * pArgs) 
    :
    DocOp ( pArgs )
  {
  }

  virtual ~DocExplode() 
  {
  }
  
  virtual const char * getName(void) const 
  {
    return "Explode";
  }

  virtual bool apply( DocList & docList );
};

class DocMerge : public DocOp
{
public:

  DocMerge(const char * pArgs) 
    :
    DocOp ( pArgs )
  {
  }

  virtual ~DocMerge() 
  {
  }
  
  virtual const char * getName(void) const 
  {
    return "Merge";
  }

  virtual bool apply( DocList & docList );
};

class DocCheck : public DocOp
{
public:

  DocCheck(const char * pArgs) 
    :
    DocOp ( pArgs )
  {
  }

  virtual ~DocCheck() 
  {
  }

  virtual const char * getName(void) const 
  {
    return "Check";
  }

  virtual bool apply( DocList & docList );
};

DocOp * getDocumentOperation(const char * pArgs)
{
  omdtOperations operation = getOperation(pArgs);

  switch (operation)
  {
  default:
  case eCOMPARE:
    return new DocCompare( pArgs );
    break;

  case eCHECK:
    return new DocCheck  ( pArgs );
    break;

  case eEXTRACT:
    return new DocExtract( pArgs );
    break;

  case eMERGE:
    return new DocMerge  ( pArgs );
    break;

  case eEXPLODE:
    return new DocExplode( pArgs );
    break;
  }

  return NULL;
}

int main(int argc , char ** argv)
{
  int index = 1;

  DocList docList;

  if (argc == 1) 
  {
    Usage(argc, argv);
    exit(-1);
  }

#ifdef YYDEBUG
#if YYDEBUG==1
  yydebug = 0;
#endif
#endif
  DocOp * pThisOp = NULL; // new DocCheck("");

  const char * pReason = "";

  int parseResult = 0;

  index = 1;

  while (index < argc)
  {
    try
    {
      if (isOptions(argv[index]))
      {
        if (pThisOp)
        {
          cout << endl << pThisOp->getName();

          if (pThisOp->apply( docList ))
          {
            cout << "Succeeded" << endl;
          }
          else
          {
            cout << "Failed" << endl;
          }

          delete pThisOp;
        }

        pThisOp = getDocumentOperation( argv[index] );

        docList.clear();

        ++index;

        continue;
      }

      omdtLex::getInstance( argv[index] );

      pReason     = "parsing";

      parseResult = yyparse();

      //printf("yyparse returned [%d]\n", parseResult);

      if (parseResult)
      {
        cerr << "Syntax or other error in " << argv[index] << ", exiting." << endl;
        exit(-1);
      } 
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

    //COMDocument * pDoc = COMDocument::getRoot();
    //OMDBase * pBaseDoc = OMDBase::getTree();

    COMDocument * pDoc = COMDocument::getRoot(); // dynamic_cast<COMDocument *>( pBaseDoc );

    COMDocument::clearRoot();

    if (pDoc)
    {
      //Check check( pDoc );

      docMap[ argv[index] ] = pDoc;
      mapDoc[ pDoc        ] = argv[index];

      docList.push_back( pDoc );
    }
    else
    {
      cout << "Error: ....???" << endl;
    }

    ++index;
  }

  if (pThisOp)
  {
    if (pThisOp->apply( docList ))
    {
      cout << endl << pThisOp->getName() << " Succeeded" << endl;
    }
    else
    {
      cout << endl << pThisOp->getName() << " Failed" << endl;
    }

    delete pThisOp;
  }

  cout << "done." << endl;

  /**/
  return 0;
}

bool DocCompare::apply( DocList & docList )
{
  if (docList.size() != 2)
  {
    cerr << "Compare : expected 2 files, not " <<  docList.size() << "." << endl;

    return false;
  }

  bool bEqual = *(docList.front()) == *(docList.back());

  cout << "Files are "
       << (bEqual?"equal":"unequal") << endl;

  return true;
}

bool DocCheck::apply( DocList & docList )
{
  DocList::const_iterator item = docList.begin();
  DocList::const_iterator last = docList.end();

  while (item != last)
  {
    COMDocument * pDoc = *item;

    Check check( pDoc );  /// <<<<<<<<<<<< limited check

    string fileName = mapDoc[ pDoc ];

    cout << "File \"" 
         << fileName
         << "\""
//       <<" @ 0x" 
//       << hex 
//       << long( pDoc ) 
//       << dec 
         << ":" 
         << endl
         << check;
    
    ++item;
  }

  return true;
}

bool DocExtract::apply( DocList & docList )
{
  cerr << endl << "Extract not implemented" << endl;
  return true;
}

bool DocMerge::apply( DocList & docList )
{
  cerr << endl << "Merge not implemented" << endl;
  return true;
}

bool DocExplode::apply( DocList & docList )
{
  cerr << endl << "Explode not implemented" << endl;
  return true;
}
// Local Variables:
// mode:C++
// End:
