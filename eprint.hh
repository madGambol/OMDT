/* ---------------------------------------------------------------------
 *                      Hallowell EMC
 *                      Pittsfield, MA
 *----------------------------------------------------------------------
 */
#ifndef _eprint_hh
#define _eprint_hh

#include <ostream>

class EPrint
{
public:
    virtual ~EPrint() 
    {
    }

    virtual std::ostream & eprint(std::ostream & out) const = 0;
};

std::ostream & operator<<( std::ostream & out, const EPrint &);

#endif // _eprint_hh

