
#include "AttributeValuePair.h"
#include "OMDTask.h"


using namespace std;

const char * tokenLookup(int token);

AttributeValuePair::AttributeValuePair(OMDBase::NodeTypeEnum nodeType, const string & val)
  :
  OMDBase( nodeType ),
  m_Value( val      )
{
}

AttributeValuePair::~AttributeValuePair()
{
}

const string AttributeValuePair::getString(void) const
{
  string retVal = string("(")
                  +
                  getAttribute()
                  +
                  string(" ")
                  + 
                  m_Value;

  string noteRefStr = getNoteRefString();

  if (noteRefStr.size() > 0)
  {
    retVal += string(" ") + noteRefStr;
  }

  retVal += string(")\n");

  return retVal;
}

const string AttributeValuePair::getAttribute(void) const
{
  return string( 
                tokenLookup( getNodeType() )
               );
}

const string AttributeValuePair::getValue    (void) const
{
  return m_Value;
}

void AttributeValuePair::setValue( const string & val)
{
  m_Value = val;
}

const char * AttributeValuePair::getType(void) const
{
  return "AttributeValuePair";
}

void AttributeValuePair::Execute( OMDTask * pTask )
{
  if (!pTask) return;

  pTask->Process( this );

}
