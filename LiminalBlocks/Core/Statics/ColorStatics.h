//
// Created by minec on 3/12/2026.
//

#ifndef OOP_COLORSTATICS_H
#define OOP_COLORSTATICS_H
#include "SFML/System/Vector3.hpp"

/*
hsv rgb2hsv(sf::Vector3f in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}
*/


sf::Vector3f hsv2rgb(sf::Vector3f in)
{
    double      hh, p, q, t, ff;
    long        i;
    sf::Vector3f         out;

    if(in.y <= 0.0) {       // < is bogus, just shuts up warnings
        out.x = in.z;
        out.y = in.z;
        out.z = in.z;
        return out;
    }
    hh = in.x;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.z * (1.0 - in.y);
    q = in.z * (1.0 - (in.y * ff));
    t = in.z * (1.0 - (in.y * (1.0 - ff)));

    switch(i) {
    case 0:
        out.x = in.z;
        out.y = t;
        out.z = p;
        break;
    case 1:
        out.x = q;
        out.y = in.z;
        out.z = p;
        break;
    case 2:
        out.x = p;
        out.y = in.z;
        out.z = t;
        break;

    case 3:
        out.x = p;
        out.y = q;
        out.z = in.z;
        break;
    case 4:
        out.x = t;
        out.y = p;
        out.z = in.z;
        break;
    case 5:
    default:
        out.x = in.z;
        out.y = p;
        out.z = q;
        break;
    }
    return out;
}

#endif //OOP_COLORSTATICS_H