/* 
 * File:   COMDocument.h
 * Author: arena
 *
 * Created on May 22, 2013, 5:42 PM
 */

#ifndef COMDOCUMENT_H
#define	COMDOCUMENT_H

#include <string>
#include <list>
#include <map>

#include "Attribute.h"

class COMDocument : public CAttributeList
{
    static 
    COMDocument * gpRoot;

    std::map<std::string, std::list<std::string> > mInfo;

public:

    COMDocument();

    virtual ~COMDocument();

    virtual void Execute ( OMDTask * pVisitor ) const;
    virtual void Indirect( OMDTask * pTask    ) const;

    const std::string getString(void) const;

    const char * getType(void) const;

    void showChildrenTypes(void) const;

    //
    // internally check the COMDocument
    //
    bool PerformChecks(void);

    //
    // MissingTypes
    // MissingClassIDs
    // DuplicateEnumeratedTypes
    // DuplicateComplexTypes
    // DuplicateClassIDs
    // DuplicateClassNames
    //
    const std::list<std::string> & getInfo(const char * pInfoType);

    static
    COMDocument * getRoot(void);

    static
    void clearRoot(void);

    static
    COMDocument * getNewRoot(void);

    //
    // This method accepts either OMD or OMT files, and returns the root document
    //
    static
    COMDocument * factory( const char * pOMDFileName );
};



#endif	/* COMDOCUMENT_H */

