/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
// For ADC input:
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/timer.h"



// This example uses the DMA to capture many samples from the ADC.
//
// - We are putting the ADC in free-running capture mode at 0.5 Msps
//
// - A DMA channel will be attached to the ADC sample FIFO
//
// - Configure the ADC to right-shift samples to 8 bits of significance, so we
//   can DMA into a byte buffer
//
// This could be extended to use the ADC's round robin feature to sample two
// channels concurrently at 0.25 Msps each.
//
// It would be nice to have some analog samples to measure! This example also
// drives waves out through a 5-bit resistor DAC, as found on the reference
// VGA board. If you have that board, you can take an M-F jumper wire from
// GPIO 26 to the Green pin on the VGA connector (top row, next-but-rightmost
// hole). Or you can ignore that part of the code and connect your own signal
// to the ADC input.

// Channel 0 is GPIO26
#define CAPTURE_CHANNEL 0
#define CAPTURE_DEPTH 4000

uint8_t capture_buf[CAPTURE_DEPTH];



int main() {
    stdio_set_translate_crlf(&stdio_usb, false);
    stdio_init_all();



    // Init GPIO for analogue use: hi-Z, no pulls, disable digital input buffer.
    adc_gpio_init(26 + CAPTURE_CHANNEL);

    adc_init();
    adc_select_input(CAPTURE_CHANNEL);
    adc_fifo_setup(
        true,    // Write each completed conversion to the sample FIFO
        true,    // Enable DMA data request (DREQ)
        1,       // DREQ (and IRQ) asserted when at least 1 sample present
        false,   // We won't see the ERR bit because of 8 bit reads; disable.
        true     // Shift each sample to 8 bits when pushing to FIFO
    );

    // Divisor of 0 -> full speed. Free-running capture with the divider is
    // equivalent to pressing the ADC_CS_START_ONCE button once per `div + 1`
    // cycles (div not necessarily an integer). Each conversion takes 96
    // cycles, so in general you want a divider of 0 (hold down the button
    // continuously) or > 95 (take samples less frequently than 96 cycle
    // intervals). This is all timed by the 48 MHz ADC clock.
    adc_set_clkdiv(0);

    sleep_ms(4000);
    printf("Arming DMA\n");
    sleep_ms(1000);
    // Set up the DMA to start transferring data as soon as it appears in FIFO
    uint dma_chan = dma_claim_unused_channel(true);
    dma_channel_config cfg = dma_channel_get_default_config(dma_chan);

    // Reading from constant address, writing to incrementing byte addresses
    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_8);
    channel_config_set_read_increment(&cfg, false);
    channel_config_set_write_increment(&cfg, true);

    // Pace transfers based on availability of ADC samples
    channel_config_set_dreq(&cfg, DREQ_ADC);

    dma_channel_configure(dma_chan, &cfg,
        capture_buf,    // dst
        &adc_hw->fifo,  // src
        CAPTURE_DEPTH,  // transfer count
        true            // start immediately
    );

    printf("Starting capture\n");
    adc_run(true);

    // Once DMA finishes, stop any new conversions from starting, and clean up
    // the FIFO in case the ADC was still mid-conversion.
    dma_channel_wait_for_finish_blocking(dma_chan);
    printf("Capture finished\n");
    adc_run(false);
    adc_fifo_drain();

    // Print samples to stdout so you can display them in pyplot, excel, matlab
    uint32_t start_time, end_time;
 
    
    // Start timer
    start_time =  timer_hw->timelr;

     for (int i = 0; i < CAPTURE_DEPTH; i += 4000) {
    fwrite(&capture_buf[i],4000, 1, stdout);
    fflush(stdout);
    //sleep_ms(100);
    //puts("");
    }
    end_time = timer_hw->timelr;;
    uint32_t elapsed_time = end_time - start_time;
  

    
    // Print elapsed time in microseconds
    printf("\nElapsed time: %u us\n",elapsed_time);

     start_time =  timer_hw->timelr;

     for (int i = 0; i < CAPTURE_DEPTH; i += 4000) {
    fwrite(&capture_buf[i],4000, 1, stdout);
    fflush(stdout);
    //sleep_ms(100);
    //puts("");
    }
    end_time = timer_hw->timelr;;
    uint32_t elapsed_time = end_time - start_time;
  

    
    // Print elapsed time in microseconds
    printf("\nElapsed time: %u us\n",elapsed_time);

     start_time =  timer_hw->timelr;

     for (int i = 0; i < CAPTURE_DEPTH; i += 4000) {
    fwrite(&capture_buf[i],4000, 1, stdout);
    fflush(stdout);
    //sleep_ms(100);
    //puts("");
    }
    end_time = timer_hw->timelr;;
    uint32_t elapsed_time = end_time - start_time;
  

    
    // Print elapsed time in microseconds
    printf("\nElapsed time: %u us\n",elapsed_time);
    
    while (1){
        printf("hello\n");
        sleep_ms(2000);
    }
    {
        /* code */
    }
    
   
}
