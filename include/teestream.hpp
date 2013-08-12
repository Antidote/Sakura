#ifndef TEESTREAM_H
#define TEESTREAM_H
#include "teebuf.hpp"
#include <iostream>
class teestream : public std::ostream
{
public:
    // Construct an ostream which tees output to the supplied
    // ostreams.
    teestream(std::ostream & o1, std::ostream & o2)
      : std::ostream(&tbuf)
      , tbuf(o1.rdbuf(), o2.rdbuf())
    {
    }
private:
    teebuf tbuf;
};



#endif // TEESTREAM_H
