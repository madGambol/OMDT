#ifndef _AlignmentAndSize_h
#define _AlignmentAndSize_h

#include <string>
#include <sstream>

#include "OMDTask.h"

std::string normalizeExpr(const std::string & size);
    
class AlignmentAndSize : public OMDTask
{
    struct AlignmentSizeRec : public CPrint
    {
        size_t mAlignment;   // offset for CDT elements
        std::string mSize;        // size   of  CDT elements, CDT's (expression)

        void normalize(void)
        {
            mSize = normalizeExpr( mSize );
        }

        AlignmentSizeRec()
            :
            mAlignment( 0 )
        {
        }

        AlignmentSizeRec(const AlignmentSizeRec & rhs)
            :
            mAlignment( rhs.mAlignment ),
            mSize     ( normalizeExpr(rhs.mSize) )
        {
        }

        AlignmentSizeRec(size_t offst, const std::string & size)
            :
            mAlignment( offst ),
            mSize     ( normalizeExpr(size)  )
        {
        }

        AlignmentSizeRec & operator=(const AlignmentSizeRec & rhs)
        {
            if (this == &rhs) return *this;

            mAlignment = rhs.mAlignment;
            mSize      = normalizeExpr(rhs.mSize);

            return *this;
        }       

	AlignmentSizeRec & operator += (const AlignmentSizeRec & item)
	{
	    mSize += std::string("+")+item.mSize;

        mSize = normalizeExpr( mSize );

	    return *this;
	}

        const AlignmentSizeRec operator *( size_t mult ) const
        {
            AlignmentSizeRec retVal( *this );

            std::stringstream multStr;

            multStr << "(" << retVal.mSize << ")*" << mult;

            CExpr * pExpr = CExpr::factory( multStr.str().c_str() );

            if (pExpr == NULL)
            {
                std::cerr << "AlignmentSizeRec operator *: ERROR, couldn't parse \""
                     << multStr.str() << "\"" << std::endl;
            }
            else
            {
                std::stringstream expr;

                pExpr->eprint( expr );
                
                retVal.mSize = expr.str();
            }   
            
            return retVal;
        }

        const AlignmentSizeRec operator *( const std::string & mult ) const
        {
            AlignmentSizeRec retVal( *this );

            std::stringstream multStr;

            if (retVal.mSize != "")
            {
                multStr << "("
                        << retVal.mSize
                        << ")*("
                        << mult
                        << ")";
            }
            else
            {
                multStr << mult;
            }

            retVal.mSize = normalizeExpr( multStr.str() );
            
            return retVal;
        }

        std::ostream & print(std::ostream & out) const;
    };

    AlignmentSizeRec mCurrentCDT;          // for current CDT

    typedef enum { eNONE,
                   eDataType,
                   eCardinality,
                   eComplexDataType        // the logic for this covers enum types too
                 } eComplexDataTypeElement;

    AlignmentSizeRec mCurrentCC;           // for current CC
    AlignmentSizeRec mFirstCC;             // first one for current CC

    eComplexDataTypeElement mElementType;  // kind of element in the CDT we're processing

    std::string      mComplexDataType;     // name of the CDT we're processing
    std::string      mDataType;            // name of the current CC DataType
    std::string      mCardinality;         // cardinality of the DataType
    bool             mbCDTStart;

    std::map<std::string, AlignmentSizeRec> mTypeNameToInfo;

    std::map<std::string, std::list< const CComplexComponent *> > mCdtToCcList; // list of elements making up a CDT

    const CComplexDataType    * mpCurrentCdt; // actual CDT

    typedef std::map<std::string, AlignmentSizeRec>::iterator       AlignmentSizeRecIter;
    typedef std::map<std::string, AlignmentSizeRec>::const_iterator ConstAlignmentSizeRecIter;

    COMDocument * mpOMDoc;

    std::list< const CComplexDataType *> mDeferred;
    bool          mbDeferred;

    bool          mbInCDT;

    void processDeferred(void);

    std::string getEnumerateSize( const string & enumTypeName ) const;

public:

    AlignmentAndSize(COMDocument *);

    virtual ~AlignmentAndSize();

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
    virtual int  Process ( const CNoteList           * pItem );
    virtual int  Process ( const CNoteText           * pItem );
    virtual int  Process ( const COMDocument         * pItem );
    virtual int  Process ( const CObjectModel        * pItem );
    virtual int  Process ( const CParameter          * pItem );
    virtual int  Process ( const CRoutingSpace       * pItem );
    virtual int  Process ( const OMDBase             * pItem );
    virtual int  Process ( const OMDNode             * pItem );
    virtual int  Process ( const Singleton           * pItem );
    virtual int  Process ( const CAttributeName      * pItem );
    virtual int  Process ( const CAttributeValue     * pItem );

    void Process(void);

    bool getAlignmentAndsize(const string & namedType, AlignmentSizeRec & alignmentAndSize) const;

    std::ostream & print( std::ostream & out ) const;
};

#endif // _AlignmentAndSize_h

// Local Variables:
// mode:C++
// End:
