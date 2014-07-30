

#ifndef _dcTypes_h
#define _dcTypes_h

class CTypeDescr
{
  string    mName;
  long      mSize;  // size in bytes

  const  
  OMDBase * mpOMDBase;

public:

  CTypeDescr ( const char * pName, long size );

  virtual 
    ~CTypeDescr() {}

  void setNode( const OMDBase * pOMDBase );

  const OMDBase * getNode(void) const;

  virtual
    bool isSimple(void) const;

  virtual
    bool isEnumeration(void) const;

  virtual
    bool isComplex(void) const;
};

class CSimpleType : public CTypeDescr
{
public:

  CSimpleType(const char * pName, long size);

  virtual
  ~CSimpleType() {}

  virtual
    bool isSimple(void) const;
};

class CComplexType : public CTypeDescr
{
public:
  CComplexType(const char * pName, long size);

  virtual
  ~CComplexType() {}

  virtual
    bool isComplex(void) const;
};

class CEnumerationType : public CTypeDescr
{
public:
  CEnumerationType(const char * pName, long size);

  virtual
    ~CEnumerationType() {}

  virtual
    bool isEnumeration(void) const;
};

#endif
// Local Variables:
// mode:C++
// End:
