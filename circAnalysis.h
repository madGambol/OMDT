

#ifndef _circAnalysis_h
#define _circAnalysis_h

//>#include <string>
//>#include <map>
//>#include <list>
//>#include "report.h"

class CCircularityAnalysis
{
 public:

  typedef std::string Symbol;

  CCircularityAnalysis(void) {}

  virtual
  ~CCircularityAnalysis(void) {}

  virtual
  void addPair(const Symbol & symbol, const Symbol & ref) = 0;

  virtual
  void clear(void) = 0;

  virtual
  void reportResults(CReport & report) = 0;

  static CCircularityAnalysis * factory(void);
};
#endif
// Local Variables:
// mode:C++
// End:
