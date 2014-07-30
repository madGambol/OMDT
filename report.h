

#ifndef _report_h
#define _report_h

#include <iostream>
#include <string>
#include <map>

#include "CPrint.h"

class CReport : public std::multimap<size_t, std::string>, 
                public CPrint
{
  std::string mTitle;

  size_t mCurrentLine;

public:

  typedef std::pair<size_t, std::string> ReportLine;
  typedef std::string string;

  CReport(const char * pTitle)
    :
    mTitle( pTitle ),
    mCurrentLine( 1 )
  {
  }

  virtual
  ~CReport()
  {
  }

  void addLine   (                 const char   * pLine);

  void addAtLine ( size_t lineNum, const char   * pLine);
  void addAtLine ( size_t lineNum, const string & line );
  void addError  ( size_t lineNum, const string & line );

  void addTBS    ( size_t lineNum, const char   * pFeature);
  void addApplied( size_t lineNum, const char   * pFeature);

  string getLinePrefix( size_t line, const char * pLabel) const;

  size_t getCount(void) const;

  virtual std::ostream & print(std::ostream & out) const;
};

#endif
// Local Variables:
// mode:C++
// End:
