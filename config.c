#include <stdio.h>
#include "config.h"
#include <ff.h>

static void configCreateInitialFile(void);

FIL fp;

void configGetStatus(fs_mount* fs, sound_state* sound, float* volume, uint32_t* rgb)
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
            *rgb = CONFIG_INITIAL_RGB;
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

            if (failed || (f_read(&fp, rgb, sizeof(uint32_t), &read) != FR_OK) || (read != sizeof(uint32_t)))
            {
                printf("cannot read rgb\n");
                *rgb = CONFIG_INITIAL_RGB;
            }
            f_close(&fp);
        }
    }
    else
    {
        *sound = CONFIG_INITIAL_SOUND;
        *volume = CONFIG_INITIAL_VOLUME;
        *rgb = CONFIG_INITIAL_RGB;
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

bool configSetRGB(fs_mount* fs, uint32_t rgb)
{
    bool ret = false;

    if (fsMounted(fs))
    {
        UINT write;

        if (f_open(&fp, CONFIG_FILENAME, FA_OPEN_ALWAYS | FA_WRITE) == FR_OK)
        {
            if (f_lseek(&fp, sizeof(sound_state) + sizeof(float)) == FR_OK)
            {
                if ((f_write(&fp, &rgb, sizeof(rgb), &write) != FR_OK) || write != sizeof(rgb))
                {
                    printf("cannot write rgb\n");
                }
                else
                {
                    ret = true;
                }
            }
            else
            {
                printf("Cannot seek to rgb\n");
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
        uint32_t c = CONFIG_INITIAL_RGB;
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

        if (failed || (f_write(&fp, &c, sizeof(c), &write) != FR_OK) || (write != sizeof(c)))
        {
            printf("Cannot write colour config\n");
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

