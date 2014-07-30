
#include <sstream>
#include <ostream>

#include "CPrint.h"

using namespace std;

CPrint::CPrint()
{
}

CPrint::~CPrint()
{
}

ostream & operator <<(ostream & out, const CPrint & item)
{
    stringstream str;

    item.print(str);

    out << str.str();

    return out;
}

stringstream & operator <<(stringstream & out, const CPrint & item)
{
    stringstream str;

    item.print(str);

    out << str.str();

    return out;
}
