#pragma once

#include "Math/qangle.h"
#include "Math/vector.h"

class CUserCMD
{
public:
    int             command_number;
    int             tick_count;
    QAngle          viewangles;
    float           forwardmove;
    float           sidemove;
    float           upmove;
    int             buttons;
    unsigned char   impulse;
    int             weaponselect;
    int             weaponsubtype;
    int             random_seed;
    short           mousedx;
    short           mousedy;
    bool            hasbeenpredicted;
};