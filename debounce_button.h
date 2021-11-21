#pragma once
#include "pico/stdlib.h"    
#include "hardware/gpio.h" 

enum debounce_event
{
    single_press = 1
};

typedef void(* event_callback_t) (uint gpio_number, enum debounce_event event);

typedef struct debounce_button_data{
    int delay_ms;                       // Debounce delay in ms
    uint pin;                           // Associated gpio pin number
    event_callback_t event_callback;    // callback into application to report events
    bool up;                            // true if pulled up
    bool high;                          // true if active high (high = pressed)
    alarm_id_t timer_id;                // Id of timer, if running - 1 otherwise
    struct debounce_button_data* next;         // Pointer to next object in chain
} debounce_button_data;

extern void debounceButtonCreate(debounce_button_data* db, uint pin, uint delay_ms, event_callback_t event_callback, bool pull_up, bool high);
extern void debounceButtonDestroy(debounce_button_data* db);
