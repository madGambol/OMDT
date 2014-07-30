####### Compiler, tools and options

#CC       = gcc
#CXX      = g++
LEX      = flex
YACC     = bison
OPT      =  #-O9
DEBUG    = -g
STLFILT  = -fmessage-length=999
CFLAGS   = $(OPT) $(DEBUG) -pipe -Wall #-DQT_NO_DEBUG -DQT_SHARED # -W -O2
CXXFLAGS = $(OPT) $(DEBUG) -pipe -Wall $(STLFILT) #-DQT_NO_DEBUG -DQT_SHARED # -W -02
LEXFLAGS = -L -v -+
YACCFLAGS= -d -l 
LINK     = $(CXX)
LFLAGS   = 
AR       = ar
LDFLAGS  = rcs
RANLIB   = ranlib
TAR      = tar -cf
GZIP     = gzip -9f
COPY     = cp -f
COPY_FILE= $(COPY) -p
COPY_DIR = $(COPY) -pR
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rmdir
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p

GCCVER34:=$(shell $(CC) -v 2>&1 | grep 'gcc version 3\.4' )
GCCVER40:=$(shell $(CC) -v 2>&1 | grep 'gcc version 4\.0' )
GCCVER41:=$(shell $(CC) -v 2>&1 | grep 'gcc version 4\.1' )
GCCVER42:=$(shell $(CC) -v 2>&1 | grep 'gcc version 4\.2' )
GCCVER43:=$(shell $(CC) -v 2>&1 | grep 'gcc version 4\.3' )
GCCVER44:=$(shell $(CC) -v 2>&1 | grep 'gcc version 4\.4' )

#ifeq "$(BOOST)" ""
#$(error BOOST environment variable needs to be defined, it should point to the include dir)
#endif

#ifeq "$(BOOSTLIB)" ""
#$(error BOOSTLIB must be set to the boost program options library: -lboost_program_options-gcc42-mt)
#endif

#ifeq "$(RTI_HOME)" ""
#$(error RTI_HOME environment variable needs to be defined)
#endif


ifneq "$(GCCVER34)" ""
GCCVER:=3.4
endif
ifneq "$(GCCVER40)" ""
GCCVER:=4.0
endif
ifneq "$(GCCVER41)" ""
GCCVER:=4.1
endif
ifneq "$(GCCVER42)" ""
GCCVER:=4.2
endif
ifneq "$(GCCVER43)" ""
GCCVER:=4.3
endif
ifneq "$(GCCVER44)" ""
GCCVER:=4.4
endif

RTISUB = $(GCCVER)

# USE_RTIS:=$(shell if test -d /usr/stow/external/rtis-13; then echo "RTIS";  fi)
#USE_RTING:=$(shell if test -d /usr/stow/external/rti13ng; then echo "RTING"; fi)

#ifeq "$(USE_RTING)" "RTING"
#
###
### RTI NG Pro 3.0/4.0
###

#$(info <<<<<<<< GCCVER   = $(GCCVER) >>>>>>>>>>>>>>>>)
#RTILINUX:= $(shell ls -1 $(RTI_HOME) | grep $(GCCVER))
#
#$(info >>>>>>>> RTI_HOME = $(RTI_HOME) <<<<<<<<<<<<<< )
#$(info >>>>>>>> RTILINUX = $(RTILINUX) <<<<<<<<<<<<<< )

#RTIINC   = $(RTI_HOME)/$(RTILINUX)/include/1_3DlcApi/
#RTILIB   = $(RTI_HOME)/$(RTILINUX)/lib

#$(info >>>>>>>> RTILIB = $(RTILIB) <<<<<<<<<<<<<< )

#$(info "using RTING")

#else

#ifeq "$(USE_RTIS)" "RTIS"
###
### RTIS D18
###
#RTIHOME  = $(JSAFHOME)/external/rtis-13
#RTIINC   = $(RTI_HOME)/include/1.3
#RTILIB   = $(RTI_HOME)/lib/linux_g++-4.2

#$(info "using RTIS")

#else

#$(error "no RTI is selected via the external directory")

#endif
#endif

# JSAFHOME:=$(shell pwd | sed -e 's"/src/randomtools""g' | sed -e 's"/OMDT""g')
#JSAFBUILD:=$(shell pwd | sed -e 's"/src/randomtools""g' | sed -e 's"/OMDT""g' | sed -e 's"Projects/OMDT"Projects"g' -e 's/source/build/g')

#$(info ===============  JSAFHOME = $(JSAFHOME) ============== )

TOOLSUPPORT=$(JSAFBUILD)/toolSupport
#ICPRINT=-I$(TOOLSUPPORT)/libcprint

##RTIPATH  = $(shell ls -1 /usr/stow/external/ | grep $(GCCVER)  )
INCPATH  = -I$(RTIINC) $(ICPRINT) -I$(BOOST)

#$(info ------------------------------RTILIB = $(RTILIB) --------------------------- )
#$(info ------------------------------RTIINC = $(RTIINC) --------------------------- )

#LPRINT=-L$(TOOLSUPPORT)/libcprint

#LIBPATH  = -L$(RTILIB) $(LCPRINT) -L/usr/local/lib -L.
LIBPATH  = $(LCPRINT) -L/usr/local/lib -L.
LIBS     =  $(LIBPATH) -lomdt
#RTILIBS  = -lrti13 -lfedtime13
#BOOSTLIB = -lboost_program_options-gcc43-mt-1_37

####### Files

SOURCES  = main.cpp

   APPSRC=omdt_test.cpp omdtCheck.cpp omdtPretty.cpp omdtOps.cpp omdtXml.cpp
   APPOBJ=omdt_test.o   omdtCheck.o   omdtPretty.o   omdtOps.o   omdtXml.o

OMDT_TEST=omdt_test 

testExecutables=omdt_test testCirc testCardinality testAlignmentAndSize \
testOmdStructure testConvert
applExecutables=omdtCheck omdtOps omdtPretty genHawklink dumpTree omdtXmlPrint

generated=cardFlex.c++ cardFlex.cpp cardFlex.h++ lex.yy.cc cardinality.cpp \
cardinality.hpp omdt.h omdt.c *.vcg

executables      =  $(testExecutables) $(applExecutables)

libraries=libomdt.a #libdecode.a

OMDTSRC0 :=Attribute.cpp   OMDNode.cpp       OMDBase.cpp           
OMDTSRC1 :=PrettyPrint.cpp Document.cpp      check.cpp dcTypes.cpp omdStructure.cpp
OMDTSRC2 :=OMDTask.cpp     PrettyPrint.cpp   singleton.cpp
OMDTSRC3 :=report.cpp      circAnalysis.cpp  AlignmentAndSize.cpp
OMDTSRC4 :=OMDCardinalityParser.cpp cardinality.cpp cardFlex.cpp
OMDTSRC5 :=CPrint.cpp eprint.cpp XMLPrint.cpp
OMDTSRC6 :=CBitfieldComponent.cpp CBitfieldDataType.cpp CClass.cpp
OMDTSRC7 :=CComplexComponent.cpp  CComplexDataType.cpp
OMDTSRC8 :=CDimension.cpp CObjectModel.cpp COMDocument.cpp CRoutingSpace.cpp
OMDTSRC9 :=CEnumeratedDataType.cpp CEnumeration.cpp expr.cpp

OMDTSRC  := $(OMDTSRC0) $(OMDTSRC1) $(OMDTSRC2) $(OMDTSRC3) $(OMDTSRC4) $(OMDTSRC5) $(OMDTSRC6) $(OMDTSRC7) $(OMDTSRC8) $(OMDTSRC9)

CARD0    := cardFlex.o cardinality.o 

OMDT_OBJECTS   := $(OMDTSRC:%.cpp=%.o) omdt.o omdtLex.o lex.yy.o singleton.o $(CARD0)
	
DECODESRC0     :=

DECODESRC      :=$(DECODESRC0) 

DECODE_OBJECTD :=$(DECODESRC:%.cpp=%.o)

####### Implicit rules

.SUFFIXES: .c .cpp .cc .cxx .C .l .y .c++ .h++

.cpp.o: 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c++.h++.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules

.PHONY:	clean
depends=libomdt.dep # libdecode.dep

all: $(depends) $(libraries) $(executables)

clean:
	rm -f core.* *.o *~ *.ii *.s *.dep
	rm -f $(executables) 
	rm -f $(libraries) 
	rm -f $(depends) 
	rm -f $(generated)

lang: omdt.o omdtLex.o lex.yy.o

# ---------------------- dependency:

libomdt.dep:	$(OMDTSRC) Makefile
	$(CXX) -c $(CFLAGS) $(INCPATH) -MM -MG $(OMDTSRC) > libomdt.dep
	
#libdecode.dep: $(DECODESRC) Makefile
#	$(CXX) -c $(CXXFLAGS) $(INCPATH) -MM -MG $(DECODESRC) > libdecode.dep

app.dep:	$(APPSRC) Makefile
	$(CXX) -c $(CFLAGS) $(INCPATH) -MM -MG $(APPSRC) > app.dep

cardinality.cpp: cardinality.y++ Makefile
	$(YACC) $(YACCFLAGS) -o $@ -p cc $<

cardinality.o: cardinality.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

AlignmentAndSize.o: AlignmentAndSize.cpp AlignmentAndSize.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

cardFlex.cpp: cardinality.l Makefile
	$(LEX) $(LEXFLAGS) -o$@ -Pcc $<

cardFlex.o: cardFlex.cpp Makefile cardinality.l
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

expr.o: expr.cpp expr.hh
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

CPrint.o: CPrint.cpp CPrint.h
#	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

Attribute.o: omdt.c omdt.h Attribute.cpp Attribute.h

# ---------------------- language:

omdt.c omdt.h: omdt.y++ Makefile Attribute.h OMDBase.h OMDNode.h
	$(YACC) $(YACCFLAGS) -o $@ $<

omdt.o: omdt.c omdt.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

lex.yy.cc: omdt.l omdt.h
	$(LEX) $(LEXFLAGS) omdt.l

lex.yy.o: lex.yy.cc omdt.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

# ---------------------- Libraries:

libomdt.a: $(OMDT_OBJECTS) 
	$(AR) $(LDFLAGS) libomdt.a $(OMDT_OBJECTS)
	$(RANLIB) libomdt.a
	
#libdecode.a: $(DECODE_OBJECTS)
#	$(AR) $(LDFLAGS) libdeocde.a $(DECODE_OBJECTS)
#	$(RANLIB) libdecode.a

# ---------------------- Actual applications:


omdtCheck: libomdt.a Makefile omdtCheck.o
	$(LINK) -Wall -W -O2 omdtCheck.o $(LIBS) -o omdtCheck  

omdtPretty: libomdt.a Makefile omdtPretty.o
	$(LINK) -Wall -W -O2 omdtPretty.o $(LIBS)  -o omdtPretty 

omdtXmlPrint: libomdt.a Makefile omdtXml.o
	$(LINK) -Wall -W -O2 omdtXml.o $(LIBS)  -o omdtXmlPrint 

omdtOps: libomdt.a Makefile omdtOps.o
	$(LINK) -Wall -W -O2 omdtOps.o $(LIBS) -o omdtOps

genHawklink: genHawklink.o libomdt.a Makefile
	$(LINK) -Wall -W -O2 genHawklink.o $(LIBS) -o genHawklink

dumpTree: dumpTree.o libomdt.a Makefile
	$(LINK) -Wall -W -O2 dumpTree.o $(LIBS) $(LIBS) -o dumpTree

# ---------------------- Test drivers

testCirc: report.o circAnalysis.o circAnalysis.h testCirc.o
	$(CXX) -c -g testCirc.cpp -o testCirc.o
	$(LINK) -Wall -W -O2 testCirc.o report.o circAnalysis.o $(LIBS) -o testCirc 

omdt_test:libomdt.a Makefile omdt_test.o
	$(LINK) -Wall -W omdt_test.o $(LIBS) -o $(OMDT_TEST) 

testCardinality:libomdt.a Makefile testCardinality.o
	$(LINK) -Wall -W testCardinality.o $(LIBS) -o testCardinality 

testAlignmentAndSize:libomdt.a Makefile testAlignmentAndSize.o
	$(LINK) -Wall -W testAlignmentAndSize.o expr.o $(LIBS) -o testAlignmentAndSize 

testOmdStructure:libomdt.a Makefile testOmdStructure.o
	$(LINK) -Wall -W testOmdStructure.o $(LIBS) -o testOmdStructure  

testConvert: Makefile testConvert.o
	$(LINK) -Wall -W testConvert.o $(LIBPATH) -o testConvert 

# ---------------------- Test code

functionTest: Makefile functionTest.o
	$(LINK) -Wall -W functionTest.o $(LIBPATH) $(LIBS) -o functionTest
	
-include app.dep
-include libraries.dep
