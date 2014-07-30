

#ifndef _XMLPrint_h
#define _XMLPrint_h

#include <sstream>

#include "OMDTask.h"
#include "Attribute.h"
#include "singleton.h"

class XMLPrint : public OMDTask
{
    std::stringstream   mOutVal;
    COMDocument  * m_pOMDoc;

    size_t m_Level;

    std::string      mAttribute;
    std::string      mValue;
    std::set<size_t> mNotes;

    //static
    //const char * indentToLevel(size_t level);

    void blockStart( const OMDBase * pItem );

    //
    // do closing tag
    //
    void blockEnd  ( const OMDBase * pItem );

    struct CBlock
    {
        std::ostream  & mOut;
        const OMDBase * mpItem;
        bool            mbTrailing;

        static size_t   gLevel;
        
    public:
        
        CBlock( std::ostream & out, const OMDBase * pItem, bool bTrailing = true );

        ~CBlock(void);
        
    };

public:

    XMLPrint(COMDocument *);

    virtual ~XMLPrint();

    void Process(void);  // do the work

    virtual int  Process ( const CAttribute          * pItem );
    virtual int  Process ( const CAttributeList      * pItem );
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
    virtual int  Process ( const CNoteText           * pItem );
    virtual int  Process ( const CNoteList           * pItem );
    virtual int  Process ( const COMDocument         * pItem );
    virtual int  Process ( const CObjectModel        * pItem );
    virtual int  Process ( const CParameter          * pItem );
    virtual int  Process ( const CRoutingSpace       * pItem );
    virtual int  Process ( const OMDBase             * pItem );
    virtual int  Process ( const OMDNode             * pItem );
    virtual int  Process ( const Singleton           * pItem );
    virtual int  Process ( const CAttributeName      * pItem );
    virtual int  Process ( const CAttributeValue     * pItem );

    virtual std::ostream & print( std::ostream & out ) const;
};

#endif

// Local Variables:
// mode:C++
// End:
