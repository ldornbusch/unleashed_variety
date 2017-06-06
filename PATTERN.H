#ifndef __PATTERN__
#define __PATTERN__

enum Timing_Effect_Numbers
{
    MoveLines   = 0x80,
    LenzFlare1  = 0x01,
    LenzFlare2  = 0x02,
    LenzFlare3  = 0x03,
    LenzFlare4  = 0x04,
    LenzFlare5  = 0x05,
    LenzFlare6  = 0x06,
    LenzFlare7  = 0x07,
    LenzFlare8  = 0x08,
    LenzFlare9  = 0x09,
    LenzFlare10 = 0x0a

};
/////////////////////////////////////////////////////////////////////////////
// Music Pattern List !!!
/////////////////////////////////////////////////////////////////////////////
int PatternTimingList[][3] =
{
    /* Pattern */

    17,02,MoveLines,
    17,20,LenzFlare1,
    17,40,LenzFlare1,

    20,40,LenzFlare1,

    21,40,LenzFlare1,

    22,40,LenzFlare1,

    0,40,LenzFlare1,

    0,40,LenzFlare1,

    5,40,LenzFlare1,

    5,40,LenzFlare1,

    7,40,LenzFlare1,

    8,40,LenzFlare1,

    1,40,LenzFlare1,

    9,40,LenzFlare1,

    10,40,LenzFlare1,

    11,40,LenzFlare1,

    12,40,LenzFlare1,

    19,40,LenzFlare1,

    16,40,LenzFlare1,

    16,40,LenzFlare1,

    14,40,LenzFlare1,

    15,40,LenzFlare1,

    2,40,LenzFlare1,

    3,40,LenzFlare1,

    4,40,LenzFlare1,

    23,40,LenzFlare1,

    18,40,LenzFlare1,

    19,40,LenzFlare1,

    24,40,LenzFlare1
};



#endif
