// Maintain configuration data
#pragma once
#include "fs_mount.h"

typedef enum sound_state
{
    off = 0,
    start = off + 1,
    brown = start,
    file_1 = brown + 1,
    file_2 = file_1 + 1,
    file_3 = file_2 + 1,
    white = file_3 + 1,
    pink = white + 1,
    end = pink + 1
} sound_state;

#define CONFIG_INITIAL_VOLUME 0.8f
#define CONFIG_INITIAL_SOUND  file_1
#define CONFIG_INITIAL_RGB 0xFFFFFFFF

#define CONFIG_FILENAME "config"

extern void configGetStatus(fs_mount* fs, sound_state* sound, float* volume, uint32_t* rgb);
extern bool configSetSoundState(fs_mount* fs, sound_state sound);
extern bool configSetVolume(fs_mount* fs, float volume);
extern bool configSetRGB(fs_mount* fs, uint32_t rgb);

extern bool getSampleValues(uint sample_rate, uint* shift, uint* wrap, uint* mid_point, float* fraction);