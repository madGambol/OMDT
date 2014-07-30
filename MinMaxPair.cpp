
#include "MinMaxPair.h"

MinMaxPair::MinMaxPair()
  :
  m_MinSize(0),
  m_MaxSize(0),
  m_bIsValid(false)
{
}

MinMaxPair::MinMaxPair(size_t theSize)
  :
  m_MinSize(theSize),
  m_MaxSize(theSize),
  m_bIsValid(true)
  
{
}


MinMaxPair::MinMaxPair(const MinMaxPair & item)
  :
  m_MinSize ( item.m_MinSize ),
  m_MaxSize ( item.m_MaxSize ),
  m_bIsValid( item.m_MinSize <= item.m_MaxSize)
{
}

const MinMaxPair & MinMaxPair::operator =(const MinMaxPair & rhs)
{
  m_MinSize  = rhs.m_MinSize;
  m_MaxSize  = rhs.m_MaxSize;

  m_bIsValid = rhs.m_bIsValid;

  return *this;
}

MinMaxPair::MinMaxPair(size_t minSize, size_t maxSize)
  :
  m_MinSize ( minSize ),
  m_MaxSize ( maxSize ),
  m_bIsValid( minSize <= maxSize )
{
}

void MinMaxPair::setSize(size_t newSize)
{
  if (newSize < m_MinSize) m_MinSize = newSize;
  if (newSize > m_MaxSize) m_MaxSize = newSize;

  m_bIsValid = m_MinSize <= m_MaxSize;
}

size_t MinMaxPair::getMaxSize(void) const
{
  return m_MaxSize;
}

size_t MinMaxPair::getMinSize(void) const
{
  return m_MinSize;
}

MinMaxPair & MinMaxPair::operator +=(const MinMaxPair & rhs)
{
  m_MinSize += rhs.m_MinSize;
  m_MaxSize += rhs.m_MaxSize;

  m_bIsValid = (m_MinSize <= m_MaxSize) && rhs.m_bIsValid;

  return *this;
}

MinMaxPair & MinMaxPair::operator *=(float rhs)
{
  m_MinSize *= (size_t)rhs;
  m_MaxSize *= (size_t)rhs;
  
  return *this;
}

const MinMaxPair MinMaxPair::operator+(const MinMaxPair & rhs) const
{
  MinMaxPair retVal = *this;
    
  retVal += rhs;

  return retVal;
}

const MinMaxPair MinMaxPair::operator *(const MinMaxPair & rhs) const
{
  MinMaxPair retVal = *this;
    
  retVal.m_MinSize *= rhs.m_MinSize;
  retVal.m_MaxSize *= rhs.m_MaxSize;

  return retVal;
}


void MinMaxPair::minMax( const MinMaxPair & item)
{
  m_MinSize = (item.m_MinSize < m_MinSize)?item.m_MinSize:m_MinSize;
  m_MaxSize = (item.m_MaxSize > m_MaxSize)?item.m_MaxSize:m_MaxSize;
}

bool MinMaxPair::isValid(void) const
{
  return m_bIsValid;
}

void MinMaxPair::roundUpToNearest(size_t val)
{
  if (val < 1) return;

  m_MinSize = ((m_MinSize+val-1)/val)*val;
  m_MaxSize = ((m_MaxSize+val-1)/val)*val;
}  
