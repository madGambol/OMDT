
#ifndef _OMDTask_h
#define _OMDTask_h

#include <iostream>

#include "CPrint.h"

class CBitfieldComponent;
class CAttribute;
class CAttributeList;
class CAttributeValuePair;
class CBitfieldComponent;
class CBitfieldDataType;
class CClass;
class CComplexComponent;
class CComplexDataType;
class CDimension;
class CEnumeratedDataType;
class CEnumeration;
class CInteraction;
class CNote;
class CNoteList;
class CNoteText;
class COMDocument;
class CObjectModel;
class CParameter;
class CRoutingSpace;
class OMDBase;
class OMDNode;
class Singleton;
class CAttributeName;
class CAttributeValue;

//
// This is the visitor design pattern that is the base class
// for "work" that is done.
//
// The visitor pattern needs to have knowledge of each of the classes
// it can be invoked from. For each class that will be processed, there
// must be a "Process" method for that class.
//
class OMDTask : public CPrint
{
public:

    OMDTask(void)
    {
    }

    virtual ~OMDTask(void)
    {
    }

    //
    // The visitor pattern requires one of these methods for each of
    // the classes it will process.
    //
    // These are the ones defined so far.
    //
    virtual int  Process ( const CAttribute          * pItem ) = 0;
    virtual int  Process ( const CAttributeList      * pItem ) = 0;
    virtual int  Process ( const CAttributeName      * pItem ) = 0;
    virtual int  Process ( const CAttributeValue     * pItem ) = 0;
    virtual int  Process ( const CAttributeValuePair * pItem ) = 0;
    virtual int  Process ( const CBitfieldComponent  * pItem ) = 0;
    virtual int  Process ( const CBitfieldDataType   * pItem ) = 0;
    virtual int  Process ( const CClass              * pItem ) = 0;
    virtual int  Process ( const CComplexComponent   * pItem ) = 0;
    virtual int  Process ( const CComplexDataType    * pItem ) = 0;
    virtual int  Process ( const CDimension          * pItem ) = 0;
    virtual int  Process ( const CEnumeratedDataType * pItem ) = 0;
    virtual int  Process ( const CEnumeration        * pItem ) = 0;
    virtual int  Process ( const CInteraction        * pItem ) = 0;
    virtual int  Process ( const CNote               * pItem ) = 0;
    virtual int  Process ( const CNoteList           * pItem ) = 0;
    virtual int  Process ( const CNoteText           * pItem ) = 0;
    virtual int  Process ( const COMDocument         * pItem ) = 0;
    virtual int  Process ( const CObjectModel        * pItem ) = 0;
    virtual int  Process ( const CParameter          * pItem ) = 0;
    virtual int  Process ( const CRoutingSpace       * pItem ) = 0;
    virtual int  Process ( const OMDBase             * pItem ) = 0;
    virtual int  Process ( const OMDNode             * pItem ) = 0;
    virtual int  Process ( const Singleton           * pItem ) = 0;

};

#endif
// Local Variables:
// mode:C++
// End:
