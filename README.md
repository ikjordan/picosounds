# picosounds
Loops mp3 and wav files, also generates coloured noise. A WS2812 LED can be configured as a night light.

## For use at night

The aim of this project is to use the Cytron Maker Pico board https://github.com/CytronTechnologies/MAKER-PI-PICO to create a noise generator that can be used at night, to aid sleep.

It is also an opportunity to demonstrate some of the features on the board. These include:
#### SD Card Reader  
Used to hold the wav and mp3 files. Also used to persist the music and night light configuration data
#### PWM Output
A ping pong DMA is used to set the PWM levels in the GPs (`GP18`, `GP19`) connected to the audio jack on the board. This allows:
1. Generation White, Pink and Brown (Red) noise
2. Playing up to 3 separate mp3 and wav files from the SD card

All sounds are played through the audio jack on the board. Sounds loop continually until user intervention, either by changing the selection, or removing the power!

Cycle through the play options by pressing the on board button connected to `GP20`

#### WS2812 LED
The board is fitted with a single NeoPixel RGB LED (`GP28`). This can be enabled to generate either Red, Orange, Yellow or White light. To cycle through the colours (plus off) press the button connected to `GP20`, whilst holding down the PICO boot select button.  
The intensity of the LED can be varied by pressing the buttons connected to `GP21` (brighter) and`GP22` (dimmer) whilst holding down the PICO boot select button.
## Selecting the mp3 or wav file to play
The player will look for and play files named `1`, `2` and `3` in the root directory of the SD Card. Files can be either `mp3` or `wav` format. Mono or stereo files are supported.  
**Note:** there should be no extension in the filename, the filename should consist of only a single digit in the range 1 to 3.

### Useful files to aid sleep
A selection of files to aid sleep can be found at:
https://archive.org/details/relaxingsounds/
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
By default the volume level at the jack can be varied by use of the buttons connected to `GP21` and `GP22`. If the attached speakers have volume control then removing the definition of `VOLUME` in `picosounds.c` will lock the volume at 100%.  
If this is done the intensity of the LED can be varied using the buttons connected to `GP21` and `GP22` without pressing the PICO boot select button.

### Debug
PWM is not disabled when a break point is reached. With the code stopped in the debugger, the interrupt routine to reconfigure the DMA will not execute, resulting in random sound being generated.  
The code is configured so that an off-board button connected to `GP7` can be used to disable the PWM to avoid the noise generation. 

### State storage
The volume and play state is stored on the sd card, and restored when the device is restarted.
