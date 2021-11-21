#pragma once
#include "pico/stdlib.h"   // stdlib 
#include "hardware/pwm.h"  // pwm 

typedef struct pwm_data {
    int pin_slice;
    uint gpio_number;
} pwm_data;

extern void pwmChannelInit(pwm_data* data, uint gpio_num);
extern void pwmChannelReconfigure(pwm_data* data, float clk_div, uint16_t wrap);
extern void pwmChannelSetFirstValue(pwm_data* data, uint16_t value);

/*
 * Inline helper functions
 */
// Return the slice associated with this pwm channel
inline int pwmChannelGetSlice(pwm_data* data){return data->pin_slice;}

// Start the pwm
inline static void pwmChannelStart(pwm_data* data){pwm_set_enabled(data->pin_slice, true);}

// Stop the pwm
inline static void pwmChannelStop(pwm_data* data){pwm_set_enabled(data->pin_slice, false);}

// build a list of pwms to start in one go
inline void pwmChannelAddStartList(pwm_data* data, uint32_t* start_mask){*start_mask |= 0x01 << data->pin_slice;}

// Start a list of PWM channels in one go
inline static void pwmChannelStartList(uint32_t start_mask) {pwm_set_mask_enabled(start_mask);}

