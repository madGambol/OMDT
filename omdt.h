/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_OMDT_H_INCLUDED
# define YY_YY_OMDT_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    eAttributeName = 258,
    eAttributeValue = 259,
    eVersionString = 260,
    OPAREN = 261,
    CPAREN = 262,
    OSQRBRACKET = 263,
    CSQRBRACKET = 264,
    COMMA = 265,
    QUOTEDSTR = 266,
    BADQUOTEDSTR = 267,
    UNQUOTEDSTR = 268,
    DATESTR = 269,
    DOCTYP = 270,
    NUMBER = 271,
    ELIPSES = 272,
    OPEN_eAccuracy = 273,
    OPEN_eAccuracyCondition = 274,
    OPEN_eApplicationDomain = 275,
    OPEN_eAttribute = 276,
    OPEN_eAutoSequence = 277,
    OPEN_eBitfieldDataType = 278,
    OPEN_eBitfieldComponent = 279,
    OPEN_eCardinality = 280,
    OPEN_eClass = 281,
    OPEN_eComplexComponent = 282,
    OPEN_eComplexDataType = 283,
    OPEN_eDIF = 284,
    OPEN_eDataBits = 285,
    OPEN_eDataPos = 286,
    OPEN_eDataWidth = 287,
    OPEN_eDataType = 288,
    OPEN_eDeliveryCategory = 289,
    OPEN_eDescription = 290,
    OPEN_eDimension = 291,
    OPEN_eDimensionMaximum = 292,
    OPEN_eDimensionMinimum = 293,
    OPEN_eDimensionType = 294,
    OPEN_eDocument = 295,
    OPEN_eEnumeratedDataType = 296,
    OPEN_eEnumeration = 297,
    OPEN_eEnumerator = 298,
    OPEN_eFEDname = 299,
    OPEN_eFieldName = 300,
    OPEN_eID = 301,
    OPEN_eISRType = 302,
    OPEN_eInteraction = 303,
    OPEN_eIntervalType = 304,
    OPEN_eMOMClass = 305,
    OPEN_eMOMComplexDataType = 306,
    OPEN_eMOMEnumeratedDataType = 307,
    OPEN_eMOMInteraction = 308,
    OPEN_eMOMVersion = 309,
    OPEN_eMessageOrdering = 310,
    OPEN_eModificationDate = 311,
    OPEN_eName = 312,
    OPEN_eNormalizationFunction = 313,
    OPEN_eNote = 314,
    OPEN_eNoteNumber = 315,
    OPEN_eNoteText = 316,
    OPEN_eOMDT = 317,
    OPEN_eObjectModel = 318,
    OPEN_ePOCEmail = 319,
    OPEN_ePOCFirstName = 320,
    OPEN_ePOCHonorificName = 321,
    OPEN_ePOCLastName = 322,
    OPEN_ePOCOrgName = 323,
    OPEN_ePOCPhone = 324,
    OPEN_ePSCapabilities = 325,
    OPEN_eParameter = 326,
    OPEN_ePurpose = 327,
    OPEN_eRange = 328,
    OPEN_eRangeSetUnits = 329,
    OPEN_eRepresentation = 330,
    OPEN_eResolution = 331,
    OPEN_eRoutingSpace = 332,
    OPEN_eSponsorOrgName = 333,
    OPEN_eStartValue = 334,
    OPEN_eSuperClass = 335,
    OPEN_eSuperInteraction = 336,
    OPEN_eTYPE = 337,
    OPEN_eTransferAccept = 338,
    OPEN_eType = 339,
    OPEN_eUnits = 340,
    OPEN_eUpdateCondition = 341,
    OPEN_eUpdateReflect = 342,
    OPEN_eUpdateType = 343,
    OPEN_eVersionNumber = 344,
    CLOSE_eAccuracy = 345,
    CLOSE_eAccuracyCondition = 346,
    CLOSE_eApplicationDomain = 347,
    CLOSE_eAttribute = 348,
    CLOSE_eAutoSequence = 349,
    CLOSE_eBitfieldDataType = 350,
    CLOSE_eBitfieldComponent = 351,
    CLOSE_eCardinality = 352,
    CLOSE_eClass = 353,
    CLOSE_eComplexComponent = 354,
    CLOSE_eComplexDataType = 355,
    CLOSE_eDIF = 356,
    CLOSE_eDataBits = 357,
    CLOSE_eDataPos = 358,
    CLOSE_eDataWidth = 359,
    CLOSE_eDataType = 360,
    CLOSE_eDeliveryCategory = 361,
    CLOSE_eDescription = 362,
    CLOSE_eDimension = 363,
    CLOSE_eDimensionMaximum = 364,
    CLOSE_eDimensionMinimum = 365,
    CLOSE_eDimensionType = 366,
    CLOSE_eDocument = 367,
    CLOSE_eEnumeratedDataType = 368,
    CLOSE_eEnumeration = 369,
    CLOSE_eEnumerator = 370,
    CLOSE_eFEDname = 371,
    CLOSE_eFieldName = 372,
    CLOSE_eID = 373,
    CLOSE_eISRType = 374,
    CLOSE_eInteraction = 375,
    CLOSE_eIntervalType = 376,
    CLOSE_eMOMClass = 377,
    CLOSE_eMOMComplexDataType = 378,
    CLOSE_eMOMEnumeratedDataType = 379,
    CLOSE_eMOMInteraction = 380,
    CLOSE_eMOMVersion = 381,
    CLOSE_eMessageOrdering = 382,
    CLOSE_eModificationDate = 383,
    CLOSE_eName = 384,
    CLOSE_eNormalizationFunction = 385,
    CLOSE_eNote = 386,
    CLOSE_eNoteNumber = 387,
    CLOSE_eNoteText = 388,
    CLOSE_eOMDT = 389,
    CLOSE_eObjectModel = 390,
    CLOSE_ePOCEmail = 391,
    CLOSE_ePOCFirstName = 392,
    CLOSE_ePOCHonorificName = 393,
    CLOSE_ePOCLastName = 394,
    CLOSE_ePOCOrgName = 395,
    CLOSE_ePOCPhone = 396,
    CLOSE_ePSCapabilities = 397,
    CLOSE_eParameter = 398,
    CLOSE_ePurpose = 399,
    CLOSE_eRange = 400,
    CLOSE_eRangeSetUnits = 401,
    CLOSE_eRepresentation = 402,
    CLOSE_eResolution = 403,
    CLOSE_eRoutingSpace = 404,
    CLOSE_eSponsorOrgName = 405,
    CLOSE_eStartValue = 406,
    CLOSE_eSuperClass = 407,
    CLOSE_eSuperInteraction = 408,
    CLOSE_eTYPE = 409,
    CLOSE_eTransferAccept = 410,
    CLOSE_eType = 411,
    CLOSE_eUnits = 412,
    CLOSE_eUpdateCondition = 413,
    CLOSE_eUpdateReflect = 414,
    CLOSE_eUpdateType = 415,
    CLOSE_eVersionNumber = 416
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{


  COMDocument         * pOMDoc;
  CAttributeList      * pAL;
  CObjectModel        * pOM;
  CRoutingSpace       * pRS;
  CDimension          * pDim;
  CBitfieldDataType   * pBFDT;
  CBitfieldComponent  * pBF;
  CComplexDataType    * pCDT;
  CComplexComponent   * pCC;
  CEnumeratedDataType * pEDT;
  CEnumeration        * pEnum;
  CClass              * pClass;
  CAttribute          * pAttr;
  CInteraction        * pInt;
  CParameter          * pParm;
  CNote               * pNote;
  CNoteText           * pNoteText;
  CNoteList           * pNL;
  CAttributeValuePair * pAVP;
  OMDNode             * pNode;
  const char          * pStr;


};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_OMDT_H_INCLUDED  */
