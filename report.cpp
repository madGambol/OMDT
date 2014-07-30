

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

#include <stdlib.h>

#include "report.h"

using namespace std;

const size_t lineFieldWidth = 6;
const size_t lineWrapWidth  = 80; // wrap to next line

size_t CReport::getCount(void) const
{
  return this->size();
}

void CReport::addError( size_t lineNum, const std::string & line)
{
  stringstream errorMsg; 

  errorMsg << getLinePrefix(lineNum, "Error")
	   << line
	   << ends;

  addAtLine( lineNum, errorMsg.str() );
}

void CReport::addAtLine(size_t lineNum, const char * pLine)
{
  std::string line( pLine );
  insert( ReportLine(lineNum, line ) );
}

void CReport::addAtLine(size_t lineNum, const std::string & line )
{
  insert( ReportLine(lineNum, line) );
}

void CReport::addTBS( size_t line, const char * pLine)
{
  stringstream msg;

  msg << "Feature : " << left << setw(50) << pLine << " --> TBS";

  insert( ReportLine(line, msg.str() ) );
}

void CReport::addApplied( size_t lineNum, const char   * pFeature)
{
  stringstream msg;

  msg << "Feature : " << left << setw(50) << pFeature << " --> Applied";

  insert( ReportLine(lineNum, msg.str() ) );
}

std::string CReport::getLinePrefix( size_t lineNum, const char * pLabel) const
{
  stringstream retVal;

  retVal << "Line" 
	 << setw(lineFieldWidth) 
	 << lineNum 
	 << " : "
	 << pLabel
	 << " : ";

  return retVal.str();
}

ostream & CReport::print(ostream & out) const
{
  const_iterator item = this->begin();
  const_iterator last = this->end();

  stringstream strOut;

  strOut << mTitle << " :" << endl;

  if (item == last)
  {
    size_t column = 20;

    strOut << setw(column) << " " << "None" << ends;
  }
  else
  {
    while (item != last)
    {
      strOut << (*item).second << endl;

      ++item;
    }
  }

  out << strOut.str() << endl;

  return out;
}


