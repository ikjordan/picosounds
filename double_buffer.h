#pragma once
#include "pico/stdlib.h"

// Function to populate buffer
typedef uint32_t (*populateBuffer)(int16_t* pBuffer, uint32_t buffer_len);

// Data for buffers
typedef struct double_buffer
{
    uint32_t  buffer_number;         // Buffer that was last filled
    int16_t*  buffers[2];            // Address of buffers
    uint32_t  len_used[2];           // Number of entries (16 bit words) populated in buffer
    uint32_t  buffer_len;            // Length of buffers
    populateBuffer fn;               // Population function
} double_buffer;

// Create the buffers
extern void doubleBufferCreate(double_buffer* db, int16_t* buff0, int16_t* buff1, uint32_t buffer_len);

// Restart the buffers
extern void doubleBufferInitialise(double_buffer* db, populateBuffer fn, const int16_t** buff, uint32_t* num_samples);

// Populate the next buffer
extern void doubleBufferPopulateNext(double_buffer* db);

// Obtain the last populated buffer and number of samples in that buffer
inline void doubleBufferGetLast(double_buffer* db, const int16_t** buff, uint32_t* num_samples){*buff = db->buffers[db->buffer_number]; 
                                                                                           *num_samples = db->len_used[db->buffer_number];}