/****************************************************************************
 *   File: omdStructure.h                                                    *
 *                                                                           *
 *   Copyright 2008 by Alion Science and Technology                          *
 *                                                                           *
 *      Alion Science and Technology                                         *
 *      BMH Operation                                                        *
 *      5365 Robin Hood Road                                                 *
 *      Norfolk, VA 23513                                                    *
 *      (757) 857 5670                                                       *
 *                                                                           *
 *   This software was developed by BMH Associates under                     *
 *   U. S. Government contracts and may be reproduced by or for              *
 *   the U. S. Government pursuant to the copyright license under            *
 *   the clause at DFARS 252.227-7013 (OCT 2005).                            *
 *                                                                           *
 *   Contents: This is the interface for omd structure visitor class.        *
 *   Created:  Sat Jan 19 16:08:43 EST 2008                                  *
 *   Author:   ccarena                                                       *
 *   Remarks:                                    .                           *
 *                                                                           *
 ****************************************************************************/

#ifndef _omdStructure_h
#define _omdStructure_h

#include <map>
#include <list>
#include <string>
#include <iostream>
#include <ostream>
#include <sstream>

//#include "OMDTask.h"
//#include "Attribute.h"
//#include "singleton.h"
//#include "CPrint.h"
//#include "omdStructure.h"

/*
 * The purpose of this class is to extract structural information from
 * the omd/omt file.
 */

// class CAttributeItemList;
// class CParameterItemList;

class CAttribute;
class CParameter;

class COMDStructure : public OMDTask
{
    COMDocument        * m_pOMDoc;

    std::map<std::string, const CComplexDataType    *> mNameToCDT;
    std::map<std::string, const CBitfieldDataType   *> mNameToBFDT;
    std::map<std::string, const CComplexComponent   *> mNameToCC;
    std::map<std::string, const CEnumeratedDataType *> mNameToEDT;
    
    std::map<size_t, const CClass        *>       mIdToClass;
    std::map<size_t, const CInteraction  *>       mIdToInteraction;
    
    std::map<size_t, std::list<const CAttribute *> >   mObjectIdToAttributeList;
    std::map<size_t, std::list<const CParameter *> >   mInteractionIdToParameterList;

    std::map<size_t, size_t>  mObjectIdtoParentId;
    std::map<size_t, size_t>  mIntIdtoParentId;
    
    std::map<size_t, std::string>  mObjectIdToName;
    std::map<size_t, std::string>  mIntIdToName;

    std::map<std::string, size_t>  mFullObjectNameToId;
    std::map<std::string, size_t>  mFullInteractionNameToId;

    std::map<size_t, std::string>  mObjectIdToFullName;
    std::map<size_t, std::string>  mInteractionIdToFullName;

    template <typename T>
    std::ostream & dumpItems( const std::map<std::string, size_t> & fullNameToId,
                              std::ostream & out
                            ) const;

    const CClass       * mpCurrentClass;
    const CInteraction * mpCurrentInteraction;
    
    size_t               mCurrentId;
    size_t               mCurrentParentId;
    std::string          mCurrentName;

    std::ostream & printNameAndTypeAndCard(const CAttributeList *, std::ostream & out) const;

    std::map<std::string, size_t>  mBasicTypesToSize;

    bool isBasicType(const std::string & type) const;

    mutable
    std::stringstream   mErrorSummary;

    class CStructBlock : public CPrint
    {
    public:
        static size_t gIndent;
        
        CStructBlock()
        {
            gIndent++;
        }

        virtual ~CStructBlock()
        {
            if (gIndent > 0) --gIndent;
        }

        std::ostream & print(std::ostream & out) const;
    };

    bool testDiscriminantRecord( const OMDNode * pCDT, std::string & discrimVar ) const;

public:

    COMDStructure(COMDocument *);

    virtual ~COMDStructure();
    
    //
    // Define the OMDTask interfaces
    //
    void Process(void);  // do the work

    std::string getDiscrimVar( const OMDNode * pCDT ) const;

    virtual int  Process ( const CAttribute          * pItem );
    virtual int  Process ( const CAttributeList      * pItem );
    virtual int  Process ( const CAttributeName      * pItem );
    virtual int  Process ( const CAttributeValue     * pItem );
    virtual int  Process ( const CAttributeValuePair * pItem );
    virtual int  Process ( const CBitfieldComponent  * pItem );
    virtual int  Process ( const CBitfieldDataType   * pItem );
    virtual int  Process ( const CClass              * pItem );
    virtual int  Process ( const CComplexComponent   * pItem );
    virtual int  Process ( const CComplexDataType    * pItem );
    virtual int  Process ( const CDimension          * pItem );
    virtual int  Process ( const CEnumeratedDataType * pItem );
    virtual int  Process ( const CEnumeration        * pItem );
    virtual int  Process ( const CInteraction        * pItem );
    virtual int  Process ( const CNote               * pItem );
    virtual int  Process ( const CNoteList           * pItem );
    virtual int  Process ( const CNoteText           * pItem );
    virtual int  Process ( const COMDocument         * pItem );
    virtual int  Process ( const CObjectModel        * pItem );
    virtual int  Process ( const CParameter          * pItem );
    virtual int  Process ( const CRoutingSpace       * pItem );
    virtual int  Process ( const OMDBase             * pItem );
    virtual int  Process ( const OMDNode             * pItem );
    virtual int  Process ( const Singleton           * pItem );

    virtual std::ostream & print( std::ostream & out ) const;

    //
    // Define the accessors for the structural information
    //
    std::list< std::pair<std::string, size_t> > getObjectList      (void) const;
    std::list< std::pair<std::string, size_t> > getInteractionList (void) const;
    
    std::list<const CAttribute *> getObjectAttributeList     (size_t id) const;
    std::list<const CParameter *> getInteractionParameterList(size_t id) const;

    std::list<const CAttribute *> getList                    (size_t id, const CAttribute * ) const;
    std::list<const CParameter *> getList                    (size_t id, const CParameter * ) const;

    std::ostream & traverseType( std::ostream & out,
                                 const std::string & type,
                                 size_t  & offset,
                                 size_t  & bytesRemaining,
                                 unsigned char *& pData) const;
};
#endif
// Local Variables:
// mode:C++
// End:
