# picosounds
Loops mp3 and wav files, also generates coloured noise.

## For use at night

The aim of this project is to use the Cytron Maker Pico board https://github.com/CytronTechnologies/MAKER-PI-PICO to create a noise generator that can be used at night, to aid sleep.

It uses ping pong DMA to set the PWM levels in the GPs (`GP18`, `GP19`)connected to the audio jack on the board. This allows:
1. Generation White, Pink and Brown (Red) noise
2. Playing up to 3 separate mp3 and wav files from the SD card

All sounds are played through the audio jack on the board. Sounds loop continually until user intervention, either by chaning the selection, or removing the power!

Cycle through the play options by pressing the on board button button connected to `GP20`

Sounds can be stopped by pressing the button connected to `GP22`

## Selecting the mp3 or wav file to play

The player will look for and play files named `1`, `2` and `3` in the root directory of the SD Card. Files can be either `mp3` or `wav` format. Mono or stereo files are supported.  
**Note:** there should be no extension in the filename, the filename should consist of only a single digit in the range 1 to 3.


### Supported sampling rates
The following sampling rates are supported:  
8000 kHz  
11000 kHz  
11025 kHz  
12000 kHz  
16000 kHz  
22000 kHz  
22050 kHz  
24000 kHz  
32000 kHz  
44000 kHz  
44100 kHz  
48000 kHz  

MP3 ABR and CBR is supported, with bit rates up to 320kBit/s.

### PWM Generation
Sound is played with 12 bit accuracy. To support this at up to 48kHz sampling rates, the pico is overclocked to 180MHz

### Sub module usage
The code uses two submodules:  
1. FatFS:   https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico  
2. Mp3:     https://github.com/ikjordan/picomp3lib

## Volume Control
By default it is assumed that the speakers attached to the audio jack will have volume control, so `GP18` and `GP19` are driven at maximum volume. If the attached speakers do not have volume control, then the buttons attached to `GP21` (up) and `GP22` (down) can be configured to vary the volume. Define `VOLUME` in picosounds.c.

### Debug
PWM is not disabled when a break point is reached. With the code disabled the interrupt routine to reconfigure the DMA will not execute, resulting in random sound being generated.  
If volume control is not enabled, the button attached to `GP22` can be used to disable the PWM to avoid the noise generation. If volume control is enabled then a button can be attached to `GP14` for debug purposes.

## State storage
The volume and play state is stored on the sd card, and restored when the device is restarted.
### Next steps
1. Configure the RGB LED on the Maker board, so that it can be used as a night light. When not in volume mode, configure control using the button connected to `GP21`



