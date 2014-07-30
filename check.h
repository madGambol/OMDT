
#ifndef _check_h
#define _check_h

#include "OMDTask.h"
#include <list>
#include <map>
#include <string>
#include <vector>
#include "Attribute.h"
#include "report.h"
#include "baseMap.h"
#include "Document.h"

//
// Checks performed:
//
// Error Checks:
//
// 1. Complex type closure (all those used in classes/interaction are defined), 
//    no duplicates, no loops
// 2. Enumeration definition (if a named type is not a simple or complex type, 
//    it is an enumerated type), no duplicates.
// 3. Class/Interaction hierarchy correct/complete (no loops, all defined),
//    no duplicates.
// 4. Cardinality checks: syntax (Rpr 1, 2, legacy or NTF), indefinite
// 5. Declared notes exist, no duplicates.
// 6. Routing space errors
// 7. 
//
// Warnings:
// 
// 1. Complex types incomplete (but not used).
// 2. Enumeration check (enumerate value).
// 3. Loose Cardinality (cardinality variable separate from data).
// 4. Alignment rules (N byte primitive type must align to N boundary).
// 5. Signed quantities used for variable cardinality.
// 6. Unused complex data types
// 7. Notes with the same text
// 8. Non-enumerated types without a "Units" clause.
// 9. Unnecessary padding fields.
//  
class Check : public Document
{
    //   bool dependsOnlyOn(const CComplexDataType *, const dependsMap & noLoops) const;

    void summarizeFrontMatter        (void);
    void summarizeRoutingSpace       (void);
    void summarizeEnumeratedDataTypes(void);
    void summarizeComplexDataTypes   (void);
    void summarizeClasses            (void);
    void summarizeInteractions       (void);
    void summarizeCardinality        (void); // check expressions, indefinite use
    void summarizeVariantRecords     (void);
    void summarizeNotes              (void);
    void summarizeAll                (void);

    void issueDupMsg( const std::string typeName, // Name field for this
                      const char    * pTypeStr,   // Name of the type
                      const OMDBase * pItem,      // the original item
                      const OMDBase * pDupItem,   // the duplicate item
                      size_t        & count
                    );

    void checkRequiredEnum        ();        // check required fields in the EnumeratedType decl
    void checkDupEnum             ();        // look for duplicated EnumeratedType decl
    void issueDupEnumMsg          ( const CEnumeratedDataType * pItem, 
                                    const OMDBase * pDupItem, 
                                    size_t & count
                                  );

    void checkRequiredCDT         ();
    void checkDupCDT              ();
    void checkMissingCDT          ();
    void checkLoopCDT             ();
    void checkOrphanedCDT         ();
    void issueDupCDTMsg           ( const CComplexDataType * pItem, 
                                    const OMDBase          * pDupItem, 
                                    size_t & count
                                  );
    void issueMissingCDTMsg       ( const CComplexComponent * pItem,
                                    const std::string  & dataType
                                  );

    void issueMissingIDMsg        ( classID id, 
                                    const OMDBase * pItem
                                  );

    void checkLoopObjects         ();
    void checkRequiredObjectProperties();
    void checkMissingAttrType     ();
    void checkMissingObjectIDs    ();
    void checkDupAttr             ();
    void issueMissingAttrTypeMsg  (const OMDBase      * pItem,
                                   const std::string  & dataType,
                                   const OMDBase      * pDataType
                                  );
    void issueDupAttrMsg          ( const OMDBase * pItem, 
                                    const OMDBase * pDupItem, 
                                    size_t        & count
                                  );

    void checkLoopInteractions    ();
    void checkRequiredIaProperties();
    void checkMissingParmType     ();
    void checkMissingIaIDs        ();
    void checkDupParm             ();


    void checkCardinalitySyntax   ();   // syntax checking
    void checkCardinalitySemantics();   // proper definition, indefinite use
   

    void issueMissingParmTypeMsg  ( const OMDBase * pItem,
                                    const std::string  & dataType,
                                    const OMDBase * pAttr
                                  );
    void issueDupParmMsg          ( const OMDBase * pItem, 
                                    const OMDBase * pDupItem, 
                                    size_t  & count
                                  );
    
    size_t                        mSummaryLineNumber;

    CReport                       mErrorReport;
    CReport                       mWarningReport;
    CReport                       mSummary;

    void addSummaryLine(const std::string & line);

public:
    
    Check( COMDocument *);

    virtual ~Check(void);
    
    size_t getErrorCount  (void) const;
    size_t getWarningCount(void) const;

    virtual std::ostream & print(std::ostream & out) const;
};

#endif
// Local Variables:
// mode:C++
// End:
