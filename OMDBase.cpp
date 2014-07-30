
#include <stdio.h>
#include <map>
#include <iostream>
#include <sstream>

#include "OMDBase.h"

using namespace std;

// void setOMDBase  ( long omdBase );
// bool checkOMDBase( long omdBase );

string IntToStr(size_t input)
{
  char buf[32];

  snprintf(buf,sizeof(buf),"%ld", input);

  return string(buf);
}

Loc::Loc()
  :
  first_line  ( 1   ),
  first_column( 0   ),
  last_line   ( 1   ),
  last_column ( 0   )
{
}

Loc::Loc( const Loc & rLoc )
  :
  first_line  ( rLoc.first_line   ),
  first_column( rLoc.first_column ),
  last_line   ( rLoc.last_line    ),
  last_column ( rLoc.last_column  )
{
}

const Loc & Loc::operator=(const Loc & rhs)
{
  first_line   = rhs.first_line;
  first_column = rhs.first_column;

  last_line    = rhs.last_line;
  last_column  = rhs.last_column;

  return *this;
}

Loc::~Loc()
{
}

ostream & Loc::print(ostream & out) const
{
//   out << "Start.Col("
//       << first_line
//       << "."
//       << first_column
//       << ")..End.Col("
//       << last_line
//       << "."
//       << last_column
//       << ")";
  
  stringstream outStr;

  outStr.copyfmt( out );
  
  outStr << "line "
	 << first_line; 

  out << outStr.str();
    
  return out;
}

// stringstream & operator<<(stringstream & out, const CPrint & item)
// {
//     stringstream str;

//     item.print( str );

//     out << str.str();

//     return out;
// }

//OMDBase * OMDBase::g_pRoot    = NULL;
size_t    OMDBase::g_Count    = 0;
size_t    OMDBase::g_MaxCount = 0;

//
// Get the current root of the document.
//
OMDBase * OMDBase::getRoot()
{
  return NULL;  // g_pRoot;
}

//
// Get the current root, clear the global pointer, and 
// count variable to prepare for another document.
//
OMDBase * OMDBase::getTree(void)
{
  // OMDBase * pRetVal = g_pRoot;

  // g_pRoot    = NULL;
  g_Count    = 0;
  //g_MaxCount = 0; // keep this as its a max for all docs processed.

  return NULL; // pRetVal;
}

int getLine();

OMDBase::OMDBase(NodeTypeEnum nodeType, const Loc * pLoc)
  :
  m_eNodeType( nodeType ),
  m_pParent  ( NULL     ),
  m_Loc      ()
{
//   if (g_pRoot == NULL) 
//   {
//     g_pRoot = this;
//   }

  g_Count++;

  if (g_Count > g_MaxCount)
  {
    g_MaxCount = g_Count;
  }

  if (pLoc)
  {
    m_Loc = *pLoc;
  }
  else
  {
    m_Loc.first_line = getLine();
    m_Loc.last_line  = getLine();
  }
}

OMDBase::~OMDBase()
{
  //printf("Deleting OMDBase\n");

//   if (g_pRoot == this) 
//   {
//     g_pRoot = NULL;

//     cout << "Total node count: " << g_Count << endl;;
//     cout << "Max   node count: " << g_MaxCount << endl;

//     cout << "Deleting OMDBase root" << endl;
//   }

//   if (checkOMDBase( long(this) ))
//   {
//     cout << "already deleted" << endl;
//   }
//   else
//   {
//     //delete m_pNoteRefs;
//     setOMDBase( long(this) );
//   }
  //m_pNoteRefs = NULL;
}

const Loc * OMDBase::getLocation(void) const
{
  return &m_Loc;
}

OMDBase * OMDBase::getParent(void) const
{
  return m_pParent;
}

void OMDBase::setParent(OMDBase * pParent)
{
  m_pParent = pParent;
}

ostream & OMDBase::print(ostream & out) const
{
    stringstream str;

    str << "OMDBase: ";
    str << m_eNodeType;
    str << ", loc = ";
    //str << m_Loc;

    out << str.str();

    return out;
}

// void OMDBase::addNoteNumber(size_t noteRef)
// {
//   if (!m_pNoteRefs)
//   {
//     m_pNoteRefs = new std::vector<size_t>();
//   }

//   if (m_pNoteRefs)
//   {
//     m_pNoteRefs->push_back( noteRef );
//   }
// }

// void OMDBase::addNoteNumber(const string & noteRefStr)
// {
//   int noteRef = atoi( noteRefStr.c_str() );

//   addNoteNumber( noteRef );
// }

// size_t OMDBase::getNoteCount(void) const
// {
//   size_t retVal = 0;

//   if (m_pNoteRefs)
//   {
//     retVal = m_pNoteRefs->size();
//   }

//   return retVal;
// }

// string OMDBase::getNote(size_t index) const
// {
//   string retVal = "";

//   do
//   {
//     if (!m_pNoteRefs) break;

//     if (index >= m_pNoteRefs->size()) break;

//     retVal = IntToStr( (*m_pNoteRefs)[index] );

//   } while (0);

//   return retVal;
// }

// string OMDBase::getNoteRefString(void) const
// {
//   string retVal = "";

//   do
//   {
//     if (!m_pNoteRefs) break;

//     vector<size_t>::const_iterator item = m_pNoteRefs->begin();
//     vector<size_t>::const_iterator last = m_pNoteRefs->end();

//     if (item != last)
//     {
//       retVal = "[";

//       retVal += IntToStr(*item);

//       ++item;

//       while (item != last)
//       {
// 	retVal += string(", ") + IntToStr(*item);
	
// 	++item;
//       }
      
//       retVal += "]";
//     }

//   } while (0);

//   return retVal;
// }

// map<long, size_t> debugDealloc;

// size_t serialNo = 0;

// void setOMDBase  ( long omdBase )
// {
//   if (checkOMDBase( omdBase )) return;
  
//   debugDealloc[ omdBase ] = serialNo++;
// }

// bool checkOMDBase( long omdBase )
// {
//   map<long, size_t>::const_iterator item = debugDealloc.find( omdBase );

//   if (item == debugDealloc.end() 
//       && 
//       (((*item).second+1) != serialNo)
//      ) 
//   {
//     return false;
//   }
  
//   cout << "Already deallocated at SN " << (*item).second << endl;
  
//   return true;
// }
