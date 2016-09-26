#ifndef FBCONTROL_H
#define FBCONTROL_H

#include "fbcontrol_global.h"

class FBCONTROLSHARED_EXPORT Fbcontrol
{

public:
    Fbcontrol();

    static void Alpha(char* chFbPath,bool bEnableAlpha,unsigned char bAlphaValue);
    static void ColorKey(char* chFbPath,bool bEnableColorKey,unsigned int iColorKey);
};

#endif // FBCONTROL_H
