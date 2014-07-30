
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <list>
#include <iterator>

#include "report.h"
#include "circAnalysis.h"

using namespace std;

typedef string Symbol;

// typedef list< Symbol > SymbolList;

class SymbolList : public list<Symbol>
{
public:
  SymbolList() {}

  ~SymbolList() {}

  void addUniq( const Symbol & sym)
  {
    push_back( sym );

    sort();
    unique();
  }

  void addUniq( const SymbolList & symList )
  {
    SymbolList temp = symList;

    merge( temp );

    sort();
    unique();
  }
};

class Edge 
{
  Symbol mOne;
  Symbol mTwo;

public:

  Edge(const Symbol & one, const Symbol & two)
    :
    mOne( one ),
    mTwo( two )
  {
  }

  bool operator==(const Edge & rhs) const
  {
    //bool bRetVal = false;

    if ((mOne == rhs.mOne) && (mTwo == rhs.mTwo)) return true;

    if ((mOne == rhs.mTwo) && (mTwo == rhs.mOne)) return true;

    return false;
  }

  bool isConnected(const Edge & rhs) const
  {
    if ((mOne == rhs.mOne) || (mOne == rhs.mTwo)) return true;

    if ((mTwo == rhs.mOne) || (mTwo == rhs.mTwo)) return true;

    return false;
  }
};      

typedef multimap< Symbol,Symbol > XRef;

class SymbolMap 
{
  XRef refs;
  XRef sfer;

  typedef XRef::value_type value_type;
  typedef XRef::const_iterator const_iterator;
  typedef XRef::iterator       iterator;

  void removeSymbol( const Symbol & sym, XRef & it)
  {
    iterator item = it.begin();
    iterator last = it.end();

    while (item != last)
    {
      iterator thisOne = item++;

      if ((*thisOne).second == sym)
      {
	it.erase( thisOne );
      }
    }
  }

  void removeSymbol( const Symbol & sym )
  {
    removeSymbol( sym, refs );
    removeSymbol( sym, sfer );
  }

public:

  SymbolMap() 
  {
  }

  ~SymbolMap() 
  {
  }

  void addPair( const Symbol & sym, const Symbol & ref )
  {
    refs.insert( value_type( sym, ref ) );
    sfer.insert( value_type( ref, sym ) );
  }

  bool refersTo    ( const Symbol & sym ) const
  {
    const_iterator item = refs.lower_bound( sym );
    const_iterator last = refs.upper_bound( sym );

    return item != last;
  }

  bool isReferredTo( const Symbol & sym ) const
  {
    const_iterator item = sfer.lower_bound( sym );
    const_iterator last = sfer.upper_bound( sym );

    return item != last;
  }

  void remove( const Symbol & sym )
  {
    refs.erase( sym );
    sfer.erase( sym );

    removeSymbol( sym );
  }

  void Clear(void)
  {
    refs.clear();
    sfer.clear();
  }
};

ostream & operator<<(ostream & out, const SymbolList & symList)
{
  stringstream str;

  SymbolList::const_iterator refItem = symList.begin();
  SymbolList::const_iterator refLast = symList.end();

  while (refItem != refLast)
  {
    str << (*refItem) << ", ";
    ++refItem;
  }

  out << str.str();

  return out;
}

ostream & operator<<(ostream & out, const XRef & xref)
{
  stringstream str;

  XRef::const_iterator item = xref.begin();
  XRef::const_iterator last = xref.end();

  while (item != last)
  {
    str << (*item).first << " with ";

//     const SymbolList & symList = (*item).second;

//     str << symList << endl;

    ++item;
  }
  
  out << str.str();

  return out;
}

class CA : public CCircularityAnalysis
{
  SymbolMap  symbolMap;

  SymbolList testSet;
  SymbolList notCirc;

  void trial( SymbolList & testSet,
	      SymbolList & notCirc,
	      SymbolMap  & symbolMap
	    )
  {
    bool bDone = false;

    while (!bDone)
    {
      bDone = true;

      SymbolList::iterator item = testSet.begin();
      SymbolList::iterator last = testSet.end();
      
      while (item != last)
      {
        SymbolList::iterator thisItem = item++;
        const Symbol sym = *thisItem;

        bool bIsReferedTo = symbolMap.isReferredTo( sym );
        bool bRefersTo    = symbolMap.refersTo    ( sym );

        if (!( bIsReferedTo && bRefersTo) )
        {
          bDone = false;

          symbolMap.remove( sym );

          testSet.remove( sym );

          notCirc.push_back( sym );
        }
      }
    }
  }

public:

  typedef CCircularityAnalysis::Symbol Symbol;

  CA() 
  {
  }

  virtual ~CA() 
  {
  }

  virtual
  void addPair(const Symbol & symbol, const Symbol & ref)
  {
    testSet.addUniq( symbol );

    symbolMap.addPair( symbol, ref );
  }

  virtual
  void clear()
  {
    symbolMap.Clear();
  }

  virtual
  void reportResults2(CReport & report)
  {
  }

  virtual
  void reportResults (CReport & report)
  {
    trial( testSet, notCirc, symbolMap);

    if (testSet.size() == 0) return;

    report.addAtLine(1, "Circularities found");

    stringstream str;

    str << testSet << ends;

    report.addAtLine(2, str.str() );
  }
};

CCircularityAnalysis * CCircularityAnalysis::factory(void)
{
  return new CA();
}
