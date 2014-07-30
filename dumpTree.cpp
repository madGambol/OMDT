
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
#include "check.h"

#include "eprint.hh"
#include "expr.hh"

#include "AlignmentAndSize.h"
#include "Document.h"

//#include "OMDBase.h"
//#include "OMDCardinalityParser.h"

extern const char * getSym();
FlexLexer * getInstance(void);

extern int yydebug;

int yyparse(void);

const char * pTestFile = "ntf.omd";

void addPrefix(const char * pPrefix, set<string> & items)
{
    set< string > copy( items );
    
    set< string >::iterator item = copy.begin();
    set< string >::iterator last = copy.end();

    items.clear();
    
    while (item != last)
    {
        string value = *item++;

        items.insert( string(pPrefix)+ value );
    }
}

ostream & operator<<(ostream & out, const set<string> & items)
{
    stringstream str;

    set< string >::const_iterator item = items.begin();
    set< string >::const_iterator last = items.end();

    while (item != last)
    {
        str << *item << endl;
        
        ++item;
    }

    out << str.str();

    return out;
}

ostream & operator<<(ostream & out, const StringToStringSetMap & items)
{
    stringstream str;

    ConstStringToStringSetMapIter item = items.begin();
    ConstStringToStringSetMapIter last = items.end();

    while (item != last)
    {
        string name = (*item).first;

        ConstStringSetIter setItem = (*item).second.begin();
        ConstStringSetIter setLast = (*item).second.end();

        while (setItem != setLast)
        {
            str << name << ":" << *setItem << endl;

            ++setItem;
        }

        ++item;
    }

    out << str.str();

    return out;
}

ostream & operator<<(ostream & out, const NameToAttrTypePairSetMap & items)
{
    stringstream str;
    
    ConstNameToAttrTypePairSetMapIter item = items.begin();
    ConstNameToAttrTypePairSetMapIter last = items.end();
    
    while (item != last)
    {
        String name = (*item).first;
        
        ConstStringPairSetIter setItem = (*item).second.begin();
        ConstStringPairSetIter setLast = (*item).second.end();

        while (setItem != setLast)
        {
            str << name << ":" << (*setItem).first << " : " << (*setItem).second << endl;

            ++setItem;
        }

        ++item;
    }
    
    out << str.str();
    
    return out;
}

string JSON( const StringToStringSetMap & items)
{
    string retVal;
    
    stringstream str;
    
    str << "{" << endl;
    
    ConstStringToStringSetMapIter item = items.begin();
    ConstStringToStringSetMapIter last = items.end();

    while (item != last)
    {
        string name = (*item).first;

        ConstStringSetIter setItem = (*item).second.begin();
        ConstStringSetIter setLast = (*item).second.end();
        
        str << "\"" << name << "\" : [" << endl;

        while (setItem != setLast)
        {
            str << "\"" << *setItem << "\"";

            ++setItem;
            
            if (setItem != setLast) str << ", ";
        }
        
        str << "]";

        ++item;
        
        if (item != last) str << ", ";
        
        str << endl;
    }
    
    str << "}" << endl;
    
    return retVal;
}

int main(int argc, char ** argv)
{
    const char * pReason = "";
    
    if (argc == 1)
    {
        cout 
            << "Usage:" << endl
            << argv[0] << " <omd_file_name> [full,types,card]" << endl
            << endl;
        return ( -1 );
    }

    string testFile ( argv[1] );

    bool bFull       = false;
    bool bJson       = false;
    bool bTypes      = false;
    //bool bCard       = false;
    
    int  parseResult = 0;

    if (argc > 2)
    {
        string arg2 = argv[2];

        bFull  = arg2.find("full")  != string::npos;
            
        bJson  = arg2.find("json")  != string::npos;
            
        bTypes = arg2.find("type") != string::npos;     
    }
    
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
        cerr << "Unexpected Exception; Reason = (" << pReason << ")" << endl;
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

    Document doc( pDoc );

//     if (bFull)
//     {
// 	set< list<string> > fqObjSetAttrs = doc.getFQObjNamesLists();
// 	set< list<string> > fqIntSetParms = doc.getFQIntNamesLists();

// 	//addPrefix("objectRoot.",      fqObjSetAttrs );
// 	//addPrefix("interactionRoot.", fqIntSetParms );

// 	cout << fqObjSetAttrs
// 	     << fqIntSetParms;
//     }
//     else
//     {

    if (bFull)
    {
        if (bTypes)
        {
            NameToAttrTypePairSetMap  nameToAttrTypePairSets  = doc.getClosedObjectToAttrTypePairSets();
            NameToParamTypePairSetMap nameToParamTypePairSets = doc.getClosedInteractToParamTypePairSets();
            
            cout << nameToAttrTypePairSets
                 << nameToParamTypePairSets;
        }
        else
        {
            StringToStringSetMap ObjParmSets = doc.getClosedObjectToAttrSets();
            StringToStringSetMap IaParmSets  = doc.getClosedInteractToParamSets();

            cout << ObjParmSets
                 << IaParmSets;
        }
    }
    else if (bJson)
    {
        StringToStringSetMap ObjParmSets = doc.getClosedObjectToAttrSets();
        StringToStringSetMap IaParmSets  = doc.getClosedInteractToParamSets();

        cout << JSON(ObjParmSets)
             << JSON(IaParmSets);
        
    }
    else
    {
        set< string > fqObjSet = doc.getFQObjNames();
        set< string > fqIntSet = doc.getFQIntNames();

        //addPrefix("objectRoot.", fqObjSet );

        //addPrefix("interactionRoot.", fqIntSet );

        cout << fqObjSet
             << fqIntSet;
    }

    return 0;
}
