/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}


const int PIN_TX = 25;

int main() {

		const uint BUTTON_PINS[] = {26,27,28,29};

		for(int x = 0; x<4; x++) {
			gpio_init(BUTTON_PINS[x]);
			gpio_set_dir(BUTTON_PINS[x], GPIO_IN);
		}

    //set_sys_clock_48();
    stdio_init_all();
    puts("WS2812 Smoke Test");

    // todo get free sm
    PIO pio = pio0;
    int sm = 0;

    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, PIN_TX, 800000, false);

    int t = 0;
		int len = 49;

		for (int i = 0; i < (len); ++i) {
				//put_pixel(urgb_u32(0, 0, 0));
				put_pixel(0);
		}
		sleep_ms(100);

		while(1) {

			/*
			for(uint8_t counter=0; counter<255; counter++)
			{
				int led_num = (len*counter/256)+1;
				for (int i = 0; i < (led_num); ++i) {
						put_pixel(urgb_u32((counter)/4, (255-counter)/8,0));
					}
				sleep_ms(100);
			}
			*/
			int my_test = 0;

			for (int i = 0; i < (4); ++i) {
				if(gpio_get(BUTTON_PINS[i])!=1){
						my_test++;
				}
			}

			if(my_test>2){
				int red = rand() % 64;
				int green = rand() % 64;
				int blue = rand() % 64;

				for (int i = 0; i < (len); ++i){
					put_pixel(urgb_u32(red,green,blue));
				}
			}
			else {
				for (int i = 0; i < (len); ++i){
					put_pixel(urgb_u32(0,0,0));
				}
			}

			sleep_ms(100);

		}

}
