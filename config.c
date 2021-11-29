#include <stdio.h>
#include "config.h"
#include <ff.h>

static void configCreateInitialFile(void);

FIL fp;

void configGetStatus(fs_mount* fs, sound_state* sound, float* volume, led_state* led, float* intensity)
{
    UINT read;
    bool failed = false;

    if (fsMounted(fs))
    {
        if (f_open(&fp, CONFIG_FILENAME, FA_OPEN_EXISTING | FA_READ) != FR_OK)
        {
            printf("Cannot open config file\n");
            configCreateInitialFile();

            *sound = CONFIG_INITIAL_SOUND;
            *volume = CONFIG_INITIAL_VOLUME;
            *led = CONFIG_INITIAL_LED;
            *intensity = CONFIG_INITIAL_INTENSITY;
        }
        else
        {
            if ((f_read(&fp, sound, sizeof(sound_state), &read) != FR_OK) || (read != sizeof(sound_state)))
            {
                printf("cannot read sound\n");
                failed = true;
                *sound = CONFIG_INITIAL_SOUND;
            }

            if (failed || (f_read(&fp, volume, sizeof(float), &read) != FR_OK) || (read != sizeof(float)))
            {
                printf("cannot read volume\n");
                failed = true;
                *volume = CONFIG_INITIAL_VOLUME;
            }

            if (failed || (f_read(&fp, led, sizeof(led_state), &read) != FR_OK) || (read != sizeof(led_state)))
            {
                printf("cannot read LED\n");
                failed = true;
                *led = CONFIG_INITIAL_LED;
            }

            if (failed || (f_read(&fp, intensity, sizeof(float), &read) != FR_OK) || (read != sizeof(intensity)))
            {
                printf("cannot read intensity\n");
                *intensity = CONFIG_INITIAL_INTENSITY;
            }
            f_close(&fp);
        }
    }
    else
    {
        *sound = CONFIG_INITIAL_SOUND;
        *volume = CONFIG_INITIAL_VOLUME;
        *led = CONFIG_INITIAL_LED;
        *intensity = CONFIG_INITIAL_INTENSITY;
    }
}

bool configSetSoundState(fs_mount* fs, sound_state sound)
{
    bool ret = false;

    if (fsMounted(fs))
    {
        UINT write;

        if (f_open(&fp, CONFIG_FILENAME, FA_OPEN_ALWAYS | FA_WRITE) == FR_OK)
        {
            if ((f_write(&fp, &sound, sizeof(sound), &write) != FR_OK) || write != sizeof(sound))
            {
                printf("cannot write sound\n");
            }
            else
            {
                ret = true;
            }

            if (f_close(&fp) != FR_OK)
            {
                printf("Failed to close file\n");
            }
        }
        else
        {
            printf("fopen failed\n");
        }
    }
    return ret;
}

bool configSetVolume(fs_mount* fs, float volume)
{
    bool ret = false;

    if (fsMounted(fs))
    {
        UINT write;

        if (f_open(&fp, CONFIG_FILENAME, FA_OPEN_ALWAYS | FA_WRITE) == FR_OK)
        {
            if (f_lseek(&fp, sizeof(sound_state)) == FR_OK)
            {
                if ((f_write(&fp, &volume, sizeof(volume), &write) != FR_OK) || write != sizeof(volume))
                {
                    printf("cannot write volume\n");
                }
                else
                {
                    ret = true;
                }
            }
            else
            {
                printf("Cannot seek to volume\n");
            }

            if (f_close(&fp) != FR_OK)
            {
                printf("Failed to close file\n");
            }
        }
        else
        {
            printf("fopen failed\n");
        }
    }
    return ret;
}

bool configSetLed(fs_mount* fs, led_state led)
{
    bool ret = false;

    if (fsMounted(fs))
    {
        UINT write;

        if (f_open(&fp, CONFIG_FILENAME, FA_OPEN_ALWAYS | FA_WRITE) == FR_OK)
        {
            if (f_lseek(&fp, sizeof(sound_state) + sizeof(float)) == FR_OK)
            {
                if ((f_write(&fp, &led, sizeof(led), &write) != FR_OK) || write != sizeof(led))
                {
                    printf("cannot write led\n");
                }
                else
                {
                    ret = true;
                }
            }
            else
            {
                printf("Cannot seek to led\n");
            }
            
            if (f_close(&fp) != FR_OK)
            {
                printf("Failed to close file\n");
            }
        }
        else
        {
            printf("fopen failed\n");
        }
    }
    return ret;
}

bool configSetIntensity(fs_mount* fs, float intensity)
{
    bool ret = false;

    if (fsMounted(fs))
    {
        UINT write;

        if (f_open(&fp, CONFIG_FILENAME, FA_OPEN_ALWAYS | FA_WRITE) == FR_OK)
        {
            if (f_lseek(&fp, sizeof(sound_state) + sizeof(float) + sizeof(led_state)) == FR_OK)
            {
                if ((f_write(&fp, &intensity, sizeof(intensity), &write) != FR_OK) || write != sizeof(intensity))
                {
                    printf("cannot write intensity\n");
                }
                else
                {
                    ret = true;
                }
            }
            else
            {
                printf("Cannot seek to intensity\n");
            }
            
            if (f_close(&fp) != FR_OK)
            {
                printf("Failed to close file\n");
            }
        }
        else
        {
            printf("fopen failed\n");
        }
    }
    return ret;
}

/* 
 * configCreateInitialFile
 *
 * Create the initial config file
 * 
 */
static void configCreateInitialFile(void)
{
    if (f_open(&fp, CONFIG_FILENAME, FA_CREATE_NEW | FA_WRITE) == FR_OK)
    {
        sound_state s = CONFIG_INITIAL_SOUND;
        float f = CONFIG_INITIAL_VOLUME;
        uint32_t l = CONFIG_INITIAL_LED;
        float i = CONFIG_INITIAL_INTENSITY;
        UINT write;
        bool failed = false;

        if ((f_write(&fp, &s, sizeof(s), &write) != FR_OK) || (write != sizeof(s)))
        {
            printf("Cannot write sound config\n");
            failed = true;
        }

        if (failed || (f_write(&fp, &f, sizeof(f), &write) != FR_OK) || (write != sizeof(f)))
        {
            printf("Cannot write volume config\n");
            failed = true;
        }

        if (failed || (f_write(&fp, &l, sizeof(l), &write) != FR_OK) || (write != sizeof(l)))
        {
            printf("Cannot write LED config\n");
            failed = true;
        }

        if (failed || (f_write(&fp, &i, sizeof(i), &write) != FR_OK) || (write != sizeof(i)))
        {
            printf("Cannot write intensity config\n");
            failed = true;
        }

        // Close the file
        if (f_close(&fp) != FR_OK)
        {
            printf("Failed to close file\n");
        }

        // Delete the file if failed to write successfully
        if (failed)
        {
            f_unlink("CONFIG_FILENAME");
            printf("Deleting corrupt config file\n");
        }
        else
        {
            printf("Successfully created config file\n");
        }
    }
    else
    {
        printf("Cannot create config file\n");
    }
}

// Determine configuration data, based on sample rate
bool getSampleValues(uint sample_rate, uint* shift, uint* wrap, uint* mid_point, float* fraction)
{
    bool ret = true;

    switch (sample_rate)
    {
        case 11000:
            *shift = 2;
            *wrap = 4091;
            *fraction = 1.0f;
        break;

        case 22000:
            *shift = 1;
            *wrap = 4091;
            *fraction = 1.0f;
        break;

        case 11025:
            *shift = 2;
            *wrap = 4082;
            *fraction = 1.0f;
        break;

        case 22050:
            *shift = 1;
            *wrap = 4082;
            *fraction = 1.0f;
        break;

        case 44000:
            *shift = 0;
            *wrap = 4091;
            *fraction = 1.0f;
        break;

        case 44100:
            *shift = 0;
            *wrap = 4082;
            *fraction = 1.0f;
        break;

        case 8000:
            *shift = 2;
            *wrap = 4091;
            *fraction = 1.375f;
        break;

        case 16000:
            *shift = 1;
            *wrap = 4091;
            *fraction = 1.375f;
        break;

        case 32000:
            *shift = 0;
            *wrap = 4091;
            *fraction = 1.375f;
        break;

        case 12000:
            *shift = 2;
            *wrap = 3750;
            *fraction = 1.0f;
        break;

        case 24000:
            *shift = 1;
            *wrap = 3750;
            *fraction = 1.0f;
        break;

        case 48000:
            *shift = 0;
            *wrap = 3750;
            *fraction = 1.0f;
        break;

        default:
            // Not a supported rate
            *wrap = 0;
            ret = false;
        break;
    }

    // mid point is half of wrap value
    *mid_point = *wrap >> 1;

    return ret;
}

