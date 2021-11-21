#include "colour_noise.h"

void colourNoiseCreate(colour_noise* cn, float m_white_scale)
{
    cn->m_seed = 0;
    cn->m_white = 0;
    cn->m_count = 1;
    cn->m_white_scale = m_white_scale;
    cn->m_brown = 0.0f;
    cn->m_pink = 0.0f;

    for (int i = 0; i < NumPinkBins; i++)
    {
        cn->m_pinkStore[i] = 0.0f;
    }
}

extern void colourNoiseSeed(colour_noise* cn, unsigned long seed)
{
    cn->m_seed = seed;
}
