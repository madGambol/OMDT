#ifndef _convert_h
#define _convert_h
/****************************************************************************
 *   File: convert.h                                                         *
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
 *   Created:  Sun Jan 20 21:32:36 EST 2008                                  *
 *   Author:   ccarena                                                       *
 *   Remarks:                                    .                           *
 *                                                                           *
 ****************************************************************************/

#include <typeinfo>

template <int length> struct DataType;

template <> 
struct DataType<1>
{
    typedef char _Base;

    DataType(void)
        :
        data( 0 )
    {
    }

    DataType(const void * pData)
        :
        data( *(_Base *)pData )
    {
    }

    char data;
  
    char operator()(void * pData)
    {
        return *(_Base *)(pData);
    }

    template <typename T>
        T operator()(void)
    {
        return *(T*)(&data);
    }
  
    bool Set(void * pData)
    {
        bool bRetVal = data != *(_Base*)(pData);

        *(_Base *)pData = data;

        return bRetVal;
    }
};

template <> 
struct DataType<2>
{
    typedef short _Base;

    DataType(void)
        :
        data( 0 )
    {
    }

    DataType(const void * pData)
        :
        data( *(_Base *)pData )
    {
    }

    short data;
  
    short operator()(void * pData)
    {
        return *(_Base *)(pData);
    }

    template <typename T>
        T operator()(void)
    {
        return *(T*)(&data);
    }
  

    bool Set(void * pData)
    {
        bool bRetVal = data != *(_Base*)(pData);

        *(_Base *)pData = data;

        return bRetVal;
    }
};

template <> 
struct DataType<4>
{
    typedef long _Base;

    DataType(void)
        :
        data( 0 )
    {
    }

    DataType(const void * pData)
        :
        data( *(_Base*)pData )
    {
    }

    long data;
    long operator()(void * pData)
    {
        return *(_Base *)(pData);
    }

    template <typename T>
        T operator()(void)
    {
        return *(T*)(&data);
    }
  
    bool Set(void * pData)
    {
        bool bRetVal = data != *(_Base*)(pData);

        *(_Base *)pData = data;

        return bRetVal;
    }
};

template <> 
struct DataType<8>
{
    typedef long long _Base;

    DataType(void)
        :
        data( 0 )
    {
    }

    DataType(const void * pData)
        :
        data( *(_Base *)pData )
    {
    }

    _Base data;

    _Base operator()(const void * pData)
    {
        return *(_Base *)(pData);
    }

    template <typename T>
        T operator()(void)
    {
        return *(T*)(&data);
    }
  
    bool Set(void * pData)
    {
        bool bRetVal = data != *(_Base*)(pData);

        *(_Base *)pData = data;
    
        return bRetVal;
    }
};

template <int length, typename T> 
bool toNativeOrderBasic(T & rData, const unsigned char * & pBuffer, size_t & bytes_remaining)
{
    DataType<length> temp;

    switch (length)
    {
    case 8:
        temp.data |= *(unsigned char*)pBuffer++;
        temp.data <<= 8;
        temp.data |= *(unsigned char*)pBuffer++;
        temp.data <<= 8;
        temp.data |= *(unsigned char*)pBuffer++;
        temp.data <<= 8;
        temp.data |= *(unsigned char*)pBuffer++;
        temp.data <<= 8;
    
    case 4:
        temp.data |= *(unsigned char*)pBuffer++;
        temp.data <<= 8;
        temp.data |= *(unsigned char*)pBuffer++;
        temp.data <<= 8;
    
    case 2:
        temp.data |= *(unsigned char*)pBuffer++;
        temp.data <<= 8;
    
    case 1:
        temp.data |= *(unsigned char*)pBuffer++;
        break;
    
    default:
        throw "toNativeOrderBasic conversion error";
    }

    bool bRetVal = temp.Set((void *)&rData);

    bytes_remaining -= length;

    return bRetVal;
};

template <int length> 
void toNetworkOrderBasic(unsigned char *& pBuffer, const void * pData, size_t & bytes_remaining)
{
    unsigned char * pTemp = pBuffer + length - 1;

    DataType<length> temp( pData );

    switch (length)
    {
    case 8:
        *pTemp--    = temp.data;
        temp.data >>= 8;
        *pTemp--    = temp.data;
        temp.data >>= 8;
        *pTemp--    = temp.data;
        temp.data >>= 8;
        *pTemp--    = temp.data;
        temp.data >>= 8;
    case 4:
        *pTemp--    = temp.data;
        temp.data >>= 8;
        *pTemp--    = temp.data;
        temp.data >>= 8;
    case 2:
        *pTemp--    = temp.data;
        temp.data >>= 8;
    case 1:
        *pTemp--    = temp.data;
        break;
    default:
        throw "toNetworkOrderBasic conversion error";
        break;
    }

    bytes_remaining -= length;
    pBuffer         += length;
}

#endif

// Local Variables:
// mode:C++
// End:
