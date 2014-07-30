
#include "COMDocument.h"

//#ifdef YYTOKENTYPE
//#undef YYTOKENTYPE
//#endif

#include "omdt.h"
#include <FlexLexer.h>
#include "omdtLex.h"

using namespace std;

int yyparse(void);
const char * tokenLookup(int token);

COMDocument * COMDocument::gpRoot = NULL; 

COMDocument * COMDocument::getRoot(void)
{
    if (!gpRoot) gpRoot = new COMDocument();

    return gpRoot;
}

COMDocument * COMDocument::getNewRoot(void)
{
    gpRoot = new COMDocument();

    return gpRoot;
}

void COMDocument::clearRoot(void)
{
    gpRoot = NULL;
}

COMDocument * COMDocument::factory( const char * pOMDFileName )
{
    COMDocument * pRetVal = NULL;

    omdtLex::getInstance( pOMDFileName );

    int parseResult = yyparse();

    if (parseResult > 0)
    {
        cerr << endl
             << "Syntax errors encountered in \"" 
             << pOMDFileName 
             << endl
             << "Further processing abandoned." 
             << endl;

    }
    else
    {
        pRetVal = getRoot();
    }

    return pRetVal;
}

const string COMDocument::getString(void) const
{
    string nodeName = tokenLookup( getNodeType() );

    stringstream retVal;
 
    size_t index;

    for (index = 0; index < m_Children.size(); index++)
    {
        retVal << m_Children[ index ]->getString(); // + string("\n");
        //cout << m_Children[ index ]->getString() << endl;
    }

    return retVal.str();
}

const char * COMDocument::getType(void) const
{
    return "Document";
}

void COMDocument::showChildrenTypes(void) const
{
    stringstream str;
    
    size_t index;
    str << "Start of COMDocument::showChildrenTypes " << endl;

    for (index = 0; index < m_Children.size(); index++)
    {
        str << "Index = " << index << ": " << m_Children[ index ]->getType() << endl;
    }
  
    str << "End   of COMDocument::showChildrenTypes " << endl << endl;

    cout << str.str();
}

COMDocument::COMDocument()
    :
    CAttributeList( OPEN_eDocument )
  //CAttributeList( OMDBase::NodeTypeEnum( OPEN_eDocument ) )
{
}

COMDocument::~COMDocument()
{
}

void COMDocument::Indirect( OMDTask * pVisitor) const
{
    pVisitor->Process( this );
}

void COMDocument::Execute( OMDTask * pVisitor ) const
{
    OMDNode::Execute( pVisitor );
}

const list<string> & COMDocument::getInfo(const char * pInfoType)
{
    return mInfo[ pInfoType ];
}

