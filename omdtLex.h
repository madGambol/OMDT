#ifndef _omdtLex_h
#define _omdtLex_h

#include <string>

//#include <FlexLexer.h>

class omdtLex : public yyFlexLexer
{
  std::string m_fileName;

  char * m_pBuffer;

  size_t m_filePosition;     // file position
  size_t m_LastPosition;     // eof position

  omdtLex(const char * pFileName);

  bool   m_bIsGood;

  bool isGood(void) const;
  
 public:

  virtual ~omdtLex();

  void comment(void);

  int yylex(void);

  virtual int LexerInput( char * buf, int size);

  virtual void LexerOutput(const char* buf, int size );

  static
  omdtLex * getInstance(const char * pFileName = NULL);
  
};
#endif

