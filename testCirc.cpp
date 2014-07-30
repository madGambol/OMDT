

#include <iostream>

#include <string>
#include <map>
#include <list>
#include "report.h"
#include "circAnalysis.h"

using namespace std;

/*
  A ( a b A ) -- period 1

  A ( a B c ) -- period 2
  B ( c A d ) 

  A ( a b B ) -- period 3
  B ( b C d )
  C ( A b c )

  A ( a B c ) -- period 4
  B ( d C f )
  C ( a D f )
  D ( f g A )

*/

size_t noCirc()
{
  size_t errorCount = 0;

  CReport report("noCirc results");

  CCircularityAnalysis * pCA = CCircularityAnalysis::factory();

  pCA->addPair( "A", "a" );
  pCA->addPair( "A", "b" );
  pCA->addPair( "A", "c" );
 
  pCA->addPair( "B", "a" );
  pCA->addPair( "B", "b" );
  pCA->addPair( "B", "c" );

  pCA->addPair( "C", "d" );
  pCA->addPair( "C", "e" );
  pCA->addPair( "C", "g" );

  pCA->reportResults( report );

  delete pCA;

  errorCount = report.getCount();

  cout << report;

  return errorCount;
}

size_t period1()
{
  CReport report("period1 results");

  CCircularityAnalysis * pCA = CCircularityAnalysis::factory();

  pCA->addPair( "A", "a" );
  pCA->addPair( "A", "b" );
  pCA->addPair( "A", "A" );

  pCA->reportResults( report );

  size_t errorCount = report.getCount();

  cout << report;

  delete pCA;

  return errorCount;
}

size_t period2()
{
  CReport report("period2 results");

  CCircularityAnalysis * pCA = CCircularityAnalysis::factory();

  pCA->addPair( "A", "0" );
  pCA->addPair( "A", "1" );
  pCA->addPair( "A", "B" );

  pCA->addPair( "B", "3" );
  pCA->addPair( "B", "4" );
  pCA->addPair( "B", "A" );

  pCA->addPair( "C", "5" );
  pCA->addPair( "C", "A" );
  pCA->addPair( "C", "6" );

  pCA->addPair( "D", "7" );
  pCA->addPair( "D", "8" );
  pCA->addPair( "D", "C" );

  pCA->reportResults( report );

  size_t errorCount = report.getCount();

  cout << report;

  delete pCA;

  return errorCount;
}

size_t period3()
{
  CReport report("period3 results");

  CCircularityAnalysis * pCA = CCircularityAnalysis::factory();

  pCA->addPair( "A", "0" );
  pCA->addPair( "A", "1" );
  pCA->addPair( "A", "B" );

  pCA->addPair( "B", "0" );
  pCA->addPair( "B", "1" );
  pCA->addPair( "B", "C" );

  pCA->addPair( "C", "0" );
  pCA->addPair( "C", "1" );
  pCA->addPair( "C", "A" );

  pCA->addPair( "D", "0" );
  pCA->addPair( "D", "1" );
  pCA->addPair( "D", "2" );
  
  pCA->addPair( "E", "7" );
  pCA->addPair( "E", "8" );
  pCA->addPair( "E", "D" );
  
  pCA->addPair( "F", "0" );
  pCA->addPair( "F", "1" );
  pCA->addPair( "F", "A" );
  
  pCA->reportResults( report );

  size_t errorCount = report.getCount();

  cout << report;

  delete pCA;

  return errorCount;
}

size_t misc()
{
  CReport report("miscellaneous results");

  CCircularityAnalysis * pCA = CCircularityAnalysis::factory();

  pCA->addPair( "A", "0" );
  pCA->addPair( "A", "1" );
  pCA->addPair( "A", "B" );

  pCA->addPair( "B", "0" );
  pCA->addPair( "B", "1" );
  pCA->addPair( "B", "A" );

  pCA->addPair( "C", "0" );
  pCA->addPair( "C", "1" );
  pCA->addPair( "C", "B" );

  pCA->addPair( "D", "0" );
  pCA->addPair( "D", "1" );
  pCA->addPair( "D", "C" );
  
  pCA->addPair( "E", "7" );
  pCA->addPair( "E", "8" );
  pCA->addPair( "E", "D" );
  
  pCA->addPair( "F", "0" );
  pCA->addPair( "F", "1" );
  pCA->addPair( "F", "A" );
  
  pCA->reportResults( report );

  size_t errorCount = report.getCount();

  cout << report;

  delete pCA;

  return errorCount;
}

int main(int argc, char ** argv)
{
  size_t errors = 0;
  size_t failures = 0;

  cout << endl << endl;

  errors = noCirc();

  if (errors)
  {
    cout << "FAIL: circularity detected in \"noCirc\" test" << endl;
    failures++;
  }
  else
  {
    cout << "PASS: no circularity detected in \"noCirc\" test" << endl;
  }

  cout << endl << endl;

  errors = period1();

  if (errors)
  {
    cout << "PASS: circularity detected in period1 test";
  }
  else
  {
    cout << "FAIL: no circularity detected in period1 test";
    failures++;
  }

  cout << endl << endl;

  errors = period2();

  if (errors)
  {
    cout << "PASS: circularity detected in period2 test";
  }
  else
  {
    cout << "FAIL: no circularity detected in period2 test";
    failures++;
  }

  cout << endl << endl;

  errors = period3();

  if (errors)
  {
    cout << "PASS: circularity detected in period3 test";
  }
  else
  {
    cout << "FAIL: no circularity detected in period3 test";
    failures++;
  }

  cout << endl << endl;

  errors = misc();

  if (errors)
  {
    cout << "PASS: circularity detected in miscellaneous test";
  }
  else
  {
    cout << "FAIL: no circularity detected in miscellaneous test";
    failures++;
  }

  cout << endl << endl;

  if (failures)
  {
    cout << endl << "Overall test FAIL with " << failures << endl;
  }
  else
  {
    cout << endl << "Overall test PASS" << endl;
  }

  return 0;
}
