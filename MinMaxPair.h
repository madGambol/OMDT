
#ifndef _MinMaxPair
#define _MinMaxPair

#include <stdlib.h>

class MinMaxPair
{
  size_t m_MinSize;
  size_t m_MaxSize;

  bool  m_bIsValid;

 public:

  MinMaxPair();

  MinMaxPair(size_t theSize);

  MinMaxPair(const MinMaxPair & item);

  const MinMaxPair & operator =(const MinMaxPair & rhs);

  MinMaxPair(size_t minSize, size_t maxSize);

  void setSize(size_t newSize);

  size_t getMaxSize(void) const;

  size_t getMinSize(void) const;

  MinMaxPair & operator +=(const MinMaxPair & rhs);

  MinMaxPair & operator *=(float rhs);

  const MinMaxPair operator *(const MinMaxPair & rhs) const;

  const MinMaxPair operator +(const MinMaxPair & rhs) const;

  void minMax( const MinMaxPair & item);

  bool isValid(void) const;

  void roundUpToNearest(size_t val);
};

#endif
