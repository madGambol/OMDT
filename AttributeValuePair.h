
#ifndef _AttributeValuePair_h
#define _AttributeValuePair_h

#include <string>

#ifndef  _OMDBase_h
#include "OMDBase.h"
#endif


class AttributeValuePair : public OMDBase
{
  std::string m_Value;

 public:

  AttributeValuePair(OMDBase::NodeTypeEnum nodeType, const std::string & val);

  virtual ~AttributeValuePair();

  const std::string getString   (void) const;

  const std::string getAttribute(void) const;

  const std::string getValue    (void) const;

  void setValue( const std::string & val);

  const char * getType(void) const;

  //
  // For the visitor pattern...
  //
  void Execute( OMDTask * );

};

#endif
