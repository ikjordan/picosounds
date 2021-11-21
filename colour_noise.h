#pragma once
#include <stdint.h>
#include <stdbool.h>

enum
{
    NumPinkBins = 16,
    NumPinkBins1 = NumPinkBins - 1
};

typedef struct colour_noise
{
    uint32_t  m_seed;
    uint32_t  m_count;
    union
    {
        uint32_t  m_white;
        float     m_fwhite;
    };

    float     m_white_scale;
    float     m_pink;
    float     m_brown;
    float     m_pinkStore[NumPinkBins];
} colour_noise;

extern void colourNoiseCreate(colour_noise* cn, float m_white_scale);
extern void colourNoiseSeed(colour_noise* cn, unsigned long seed);

inline float colourNoiseWhite(colour_noise* cn)
{
    cn->m_seed = (cn->m_seed * 196314165) + 907633515;
    cn->m_white = cn->m_seed >> 9;
    cn->m_white |= 0x40000000;
    return (cn->m_fwhite - 3.0f) * cn->m_white_scale;
};

int inline CTZ(int num)
{
    int i = 0;
    while (((num >> i) & 1) == 0 && i < (signed)sizeof(int)) i++;
    return i;

    
    //if (num == 0) {
    //    return 32U;
    //}
    //return __builtin_clz(num);
    //}
}

// returns pink noise random number in the range -0.5 to 0.5
//
inline float colourNoisePink(colour_noise* cn)
{
    float prevr;
    float r;
    unsigned long k;
    k = CTZ(cn->m_count);
    k = k & NumPinkBins1;

    // get previous value of this octave 
    prevr = cn->m_pinkStore[k];

    while (true)
    {
        r = colourNoiseWhite(cn);

        // store new value 
        cn->m_pinkStore[k] = r;

        r -= prevr;

        // update total 
        cn->m_pink += r;

        if (cn->m_pink < -4.0f || cn->m_pink > 4.0f)
        {
            cn->m_pink -= r;
        }
        else
        {
            break;
        }
    }

    // update counter 
    cn->m_count++;

    return (colourNoiseWhite(cn) + cn->m_pink) * 0.125f;
}

// returns brown noise random number in the range -0.5 to 0.5
//
inline float colourNoiseBrown(colour_noise* cn)
{
    while (true)
    {
        float  r = colourNoiseWhite(cn);
        cn->m_brown += r;
        if (cn->m_brown < -8.0f || cn->m_brown>8.0f)
        {
            cn->m_brown -= r;
        }
        else
        {
            break;
        }
    }
    return cn->m_brown * 0.0625f;
}
