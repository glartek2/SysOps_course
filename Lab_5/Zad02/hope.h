
#ifndef _HOPIUM_H
#define _HOPIUM_H

enum Hopium {
    M_PNUMS     = 1,    // Print numbers
    M_PCHANGES  = 2,    // Print number of cap changes
    M_EXIT      = 3     // Exit
};

enum Hopium parse_hope(const char *);

#endif // _HOPIUM_H
