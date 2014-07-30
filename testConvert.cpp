
#include <iostream>
#include <ostream>
#include <sstream>
#include <iomanip>

#include <stack>
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

#include "convert.h"

ostream & PrintBuffer(ostream & out, const unsigned char * pBuffer, size_t bufferSize)
{
    stringstream str;
    
    const size_t charsPerLine = 16;
    size_t index;

    for (index = 0; index < bufferSize; index += charsPerLine)
    {
        str << "[" << setw(3) << index << "]: ";

        size_t col;
    
        for (col = index; col < min(index+charsPerLine,bufferSize); col++)
        {
            str << setw(2) << hex << (unsigned int)(pBuffer[col]) << " ";
        }
    
        for (; col < index+charsPerLine; col++)
        {
            str << "   ";
        }

        str << " >>";
    
        for (col = index; col < min(index+charsPerLine,bufferSize); col++)
        {
            int ch = pBuffer[col];

            str << char( isprint(ch)?ch:'.');
        }
    
        for (; col < index+charsPerLine; col++)
        {
            str << " ";
        }

        str << "<<" << endl;
    }

    out << str.str();

    return out;
}

template <typename T>
bool tryConversion( const T & val )
{
    T aVal = val;
    T cVal = 0;

    //     cout << "Input Value bytes:" << endl;
    //     PrintBuffer( cout, (const unsigned char *)&aVal, sizeof(T) );
    
    const size_t size = sizeof(T);

    unsigned char buffer[ 2*sizeof(T) ];
    unsigned char * pBuffer = buffer;

    size_t bytesRemaining = size;

    toNetworkOrderBasic<sizeof(T)>( pBuffer, (const void *)&aVal, bytesRemaining );

    bytesRemaining = size;

    cout << "Data Buffer :" << endl;
    
    PrintBuffer( cout, buffer, sizeof(T) );

    pBuffer = buffer;

    toNativeOrderBasic<sizeof(T),T>( cVal, (const unsigned char * &)pBuffer, bytesRemaining);

    cout << "input value = "
         << val
         << ", output value = "
         << cVal
         << ", ---> values " << ((val == cVal)?"":"don't ") << "match"
         << endl;

    //     cout << "Output Value bytes:" << endl;
    //     PrintBuffer( cout, (const unsigned char *)&cVal, sizeof(T) );

    return val == cVal;
};


int main(int argc, char ** argv)
{
    cout << "Testing Conversion" << endl;

    double    aDblVal      = 1.23456789;
    long long aLongLongVal = 1234567890123456LL;
    long      aLongVal     = 123456789;
    short     aShortVal    = -12345;
    unsigned short aUShort = 54321;

    tryConversion( aDblVal      );
    tryConversion( aLongLongVal );
    tryConversion( aLongVal     );
    tryConversion( aShortVal    );
    tryConversion( aUShort      );
    
    return 0;
}


        
