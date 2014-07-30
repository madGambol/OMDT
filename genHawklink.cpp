
#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <iomanip>

#include <stack>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

using namespace std;

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

bool isHawklink( const list<string> & nameList )
{
    bool retVal = false;

    do
    {
        if (nameList.size() == 0) break;

        retVal = nameList.back() == "Hawklink";
        
    } while (0);
    
    
    return retVal;
}

string ConCatHawklink( const list<string> & nameList )
{
    string retVal;

    list<string>::const_iterator item = nameList.begin();
    list<string>::const_iterator last = nameList.end();
    
    while (item != last)
    {
        retVal = *item + retVal;
        ++item;
        if (item != last)
        {
            retVal = string(".")+retVal;
        }
    }
    
    return retVal;
}

string getSecond( const list<string> & nameList )
{
    string retVal;

    list<string>::const_iterator item = nameList.begin();
    list<string>::const_iterator last = nameList.end();
    
    do
    {
        if (item == last) break;

        ++item;

        if (item == last) break;

        retVal = *item;
        
    } while (0);
    

    return retVal;
}

string Lower( const string & str )
{
    string retVal = str;

    size_t i;
    for (i=0; i < retVal.size(); i++)
    {
        retVal[i] = tolower( retVal[i] );
    }

    return retVal;
}

bool dependsOrder( const list<string> & lhs, const list<string> & rhs)
{
    bool bRetVal = false;
    bool bDone   = false;
    
    list<string>::const_reverse_iterator lhsItem = lhs.rbegin();
    list<string>::const_reverse_iterator rhsItem = rhs.rbegin();

    list<string>::const_reverse_iterator lhsLast = lhs.rend();
    list<string>::const_reverse_iterator rhsLast = rhs.rend();

    while ((lhsItem != lhsLast) && (rhsItem != rhsLast))
    {
        if (*lhsItem < *rhsItem)
        {
            bRetVal = true;
            bDone   = true;
            
            break;
        }
        else if (*lhsItem > *rhsItem)
        {
            bDone   = true;
            break;
        }

        ++lhsItem;
        ++rhsItem;
    }

    do
    {
        if (bDone) break;
        
        if ((lhsItem != lhsLast) && (rhsItem == rhsLast))
        {
            // lhs > rhs
            break;
        }
        
        if ((lhsItem == lhsLast) && (rhsItem != rhsLast))
        {
            bRetVal = true;
        }
        
    } while(0);

    return bRetVal;
}

       
    

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

    //AlignmentAndSize aas( pDoc );

    //aas.Process();

// auto_hlsonolaunch.hh auto_hlsonolaunch.cc: $(FULL_SOM) $(toolsbindir)/autosih  ///
//                         $(toolsbindir)/autosih \                               ///
//                 -som $(RDRDEST)/som.omt \                                      ///
//                 -outfile $(basename $@) \                                      ///
//                 -somclass interactionRoot.Hawklink.HeloToShip.SonobuoyLaunch \ ///
//                 -sihclass SIHHawklinkSonoLaunch \                              ///
//                 -include libsihhawklink \                                      ///
//                 -superclass SIHHawklinkHeloToShip                              ///

    {
        ofstream fout("GNUmakefile.in.tmp");

        stringstream str; // will hold auto gen data until the end

        stringstream hdr;
        stringstream bdy;
        stringstream obj;

        Document doc( pDoc );

        set< list<string> > fqiNames = doc.getFQIntNamesLists();

        cout << endl
             << "# of interactions = "
             << fqiNames.size()
             << endl;

        cout << "# of interactions = "
             << doc.getFQIntNames().size()
             << endl;

        list< list<string> > fqiNamesList;

        {
            set< list<string> >::iterator item = fqiNames.begin();
            set< list<string> >::iterator last = fqiNames.end();

            while (item != last)
            {
                if (isHawklink( *item ))
                {
                    fqiNamesList.push_back( *item );
                }
                ++item;
            }
        }

        fqiNamesList.sort( dependsOrder );
        
        list< list<string> >::iterator item = fqiNamesList.begin();
        list< list<string> >::iterator last = fqiNamesList.end();

        list<string> fileNames;

        while (item != last)
        {
            string front       = (*item).front();
            string concat      = ConCatHawklink( *item );
            string second      = getSecond( *item );
            string lowerName   = Lower( front );
            string lowerSecond = Lower( second );
            
            string parentHdr;

            if (lowerSecond.size())
            {
                parentHdr = string("auto_hl")+lowerSecond+string(".hh");
            }

            fileNames.push_back( string("auto_hl")+lowerName );
                
            str << "auto_hl"
                << lowerName
                << ".hh auto_hl"
                << lowerName
                << ".cc: $(FULL_SOM) $(toolsbindir)/autosih " << parentHdr
                << endl
                << "\t\t\t$(toolsbindir)/autosih \\"
                << endl
                << "\t\t-som $(RDRDEST)/som.omt \\"
                << endl
                << "\t\t-outfile $(basename $@) \\"
                << endl
                << "\t\t-somclass interactionRoot."
                << concat
                << " \\"
                << endl
                << "\t\t-sihclass SIHHawklink";
                

            if (front != "Hawklink")
            {
                str << front;
            }

            str << " \\"
                << endl
                << "\t\t-include libsihhawklink \\"
                << endl
                << "\t\t-superclass ";

            if (front == "Hawklink")
            {
                str << "SIHBase";
            }
            else if (second == "Hawklink")
            {
                str << "SIHHawklink";
            }
            else
            {
                str << "SIHHawklink" << second;
            }

            str << endl
                << endl;
            
            ++item;
        }

        //
        fout << "srcdir=@srcdir@"           << endl
             << "VPATH=@srcdir@"            << endl
             << "top_srcdir=@top_srcdir@"   << endl
             << "CC=@CC@"                   << endl
             << "CXX=@CXX@"                 << endl
             << "CPPFLAGS=@CPPFLAGS@"       << endl
             << "CFLAGS=@CFLAGS@"           << endl
             << "CXXFLAGS=@CXXFLAGS@"       << endl
             << "AR=@AR@"                   << endl
             << "ARFLAGS=@ARFLAGS@"         << endl
             << "CXX_AR=@CXX_AR@"           << endl
             << "CXX_ARFLAGS=@CXX_ARFLAGS@" << endl
             << "RANLIB=@RANLIB@"           << endl
             << "RLFLAGS=@RLFLAGS@"         << endl
             << "LDFLAGS=@LDFLAGS@"         << endl
             << "LIBS=@LIBS@"               << endl
             << endl
             << "targetroot=@targetroot@"   << endl
             << "toolsbindir=@toolsbindir@" << endl;
        {
            list<string>::iterator item = fileNames.begin();
            list<string>::iterator last = fileNames.end();

            while (item != last)
            {
                hdr << setw(21) << " " << (*item) << ".hh";
                bdy << setw(21) << " " << (*item) << ".cc";
                obj << setw(21) << " " << (*item) << ".o";
            
                ++item;

                if (item != last)
                {
                    hdr << " \\";
                    bdy << " \\";
                    obj << " \\";
                }

                hdr << endl;
                bdy << endl;
                obj << endl;
            }
        }
        
        
        //       01234567890123456789012
        fout << "CXX_OBJS           = \\" << endl
             << obj.str()
             << endl
             << "EXTRA_CLEAN_FILE_H = \\" << endl
             << hdr.str()
             << endl
             << "EXTRA_CLEAN_FILE_CC= \\" << endl
             << bdy.str()
             << endl;

        fout << "LIBNAME = sihhawklink" << endl
             << "HEADERS = libsihhawklink.hh $(EXTRA_CLEAN_FILE_H)" << endl
             << endl
             << "LOCAL_HEADERS = " << endl
             << "EXTRA_CLEAN_FILES = $(EXTRA_CLEAN_FILE_H) $(EXTRA_CLEAN_FILE_CC)" << endl
             << endl
             << "FUNCTION_PREFIX = SIH" << endl
             << endl
             << "include $(top_srcdir)/makeinclude/make.librules" << endl
             << endl
             << "MY_SOM:=sihhawklink.omt" << endl
             << endl
             << "FULL_SOM:=$(RDRDEST)/som.omt" << endl
             << endl
             << str.str();
    }
    
    return 0;
}
