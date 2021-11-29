// Maintain configuration data
#pragma once
#include "fs_mount.h"

typedef enum sound_state    // Describes the supported music and noise states
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

typedef enum led_state      // Describes the supported LED colours
{
    led_black = 0,
    led_start = led_black,
    led_red = led_black + 1,
    led_orange = led_red + 1,
    led_yellow = led_orange + 1,
    led_white = led_yellow + 1,
    led_wrap = led_white +1
} led_state;

#define CONFIG_INITIAL_VOLUME 0.8f
#define CONFIG_INITIAL_SOUND  file_1
#define CONFIG_INITIAL_LED led_black
#define CONFIG_INITIAL_INTENSITY 1.0f

#define CONFIG_FILENAME "config_2"          // Change name when have breaking changes to config

extern void configGetStatus(fs_mount* fs, sound_state* sound, float* volume, led_state* led, float* intensity);
extern bool configSetSoundState(fs_mount* fs, sound_state sound);
extern bool configSetVolume(fs_mount* fs, float volume);
extern bool configSetLed(fs_mount* fs, led_state led);
extern bool configSetIntensity(fs_mount* fs, float intensity);

extern bool getSampleValues(uint sample_rate, uint* shift, uint* wrap, uint* mid_point, float* fraction);