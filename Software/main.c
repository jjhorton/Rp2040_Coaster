/**
 * RP2040 Coaster code 
 * WS2812 code base from https://github.com/raspberrypi/pico-examples
 * Copyright (c) 2022 James Horton
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

const int RGB_LOOKUP[][3] = {{255,14,240},{255,13,240},{255,12,240},{255,11,240},
														{255,10,240},{255,9,240},{255,8,240},{255,7,240},
														{255,6,240},{255,5,240},{255,4,240},{255,3,240},
														{255,2,240},{255,1,240},{255,0,240},{255,0,224},
														{255,0,208},{255,0,192},{255,0,176},{255,0,160},
														{255,0,144},{255,0,128},{255,0,112},{255,0,96 },
														{255,0,80 },{255,0,64 },{255,0,48 },{255,0,32 },
														{255,0,16 },{255,0,0  },{255,10,0 },{255,20,0 },
														{255,30,0 },{255,40,0 },{255,50,0 },{255,60,0 },
														{255,70,0 },{255,80,0 },{255,90,0 },{255,100,0},
														{255,110,0},{255,120,0},{255,130,0},{255,140,0},
														{255,150,0},{255,160,0},{255,170,0},{255,180,0},
														{255,190,0},{255,200,0},{255,210,0},{255,220,0},
														{255,230,0},{255,240,0},{255,250,0},{253,255,0},
														{215,255,0},{176,255,0},{138,255,0},{101,255,0},
														{62,255,0 },{23,255,0 },{0,255,16 },{0,255,54 },
														{0,255,92 },{0,255,131},{0,255,168},{0,255,208},
														{0,255,244},{0,228,255},{0,212,255},{0,196,255},
														{0,180,255},{0,164,255},{0,148,255},{0,132,255},
														{0,116,255},{0,100,255},{0,84,255 },{0,68,255 },
														{0,50,255 },{0,34,255 },{0,18,255 },{0,2,255  },
														{0,0,255  },{1,0,255  },{2,0,255  },{3,0,255  },
														{4,0,255  },{5,0,255 }};

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

int valueinarray(int val, int arr[], int len){
    int i;
    for(i = 0; i < len; i++){
        if(arr[i] == val) return 1;
    }
    return 0;
}

void setcenter(int red, int green, int blue){
		int inner[] = {16,17,18,23,24,25,30,31,32};
		int inner_len = sizeof(inner) / sizeof(inner[0]);

		int outer[] = {0,1,2,3,4,5,6,7,13,14,20,21,27,28,34,35,41,42,43,44,45,46,47,48};
		int outer_len = sizeof(outer) / sizeof(outer[0]);

		for (int i = 0; i < (49); ++i){
			if (valueinarray(i, inner, inner_len)) {
				put_pixel(urgb_u32(red,green,blue));
			}
			else {
				if (valueinarray(i, outer, outer_len)) {
					put_pixel(urgb_u32(0,4,0));
				}
				else{
				put_pixel(urgb_u32(0,0,0));
				}
			}
		}
		sleep_ms(100);
}

void setoutside(int red, int green, int blue){

		int outer[] = {0,1,2,3,4,5,6,7,13,14,20,21,27,28,34,35,41,42,43,44,45,46,47,48};
		int outer_len = sizeof(outer) / sizeof(outer[0]);

		for (int i = 0; i < (49); ++i){
			if (valueinarray(i, outer, outer_len)) {
				put_pixel(urgb_u32(red,green,blue));
			}
			else {
				put_pixel(urgb_u32(0,0,0));
			}
		}
		sleep_ms(100);
}

void set_blank(int led_num){
		sleep_ms(50);

		for (int i = 0; i < (led_num); ++i){
			put_pixel(urgb_u32(0,0,0));
		}
		sleep_ms(100);
}

void set_colour(int led_num, int red, int green, int blue){
		sleep_ms(50);

		for (int i = 0; i < (led_num); ++i){
			put_pixel(urgb_u32(red,green,blue));
		}
		sleep_ms(100);
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

    PIO pio = pio0;
    int sm = 0;

    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, PIN_TX, 800000, false);

    int t = 0;
		int len = 49;


		//Set all LED's to be off
		for (int i = 0; i < (len); ++i) {
				put_pixel(urgb_u32(0, 0, 0));
		}
		sleep_ms(100);

		while(1) {

			for(int level = 0; level<180; level++ ){

						int my_test = 0;
						for (int i = 0; i < (4); ++i) {
							if(gpio_get(BUTTON_PINS[i])!=1){
									my_test++;
							}
						}

						//Set the center LED's
						if(my_test>2){
							if (level < 90) {
								setcenter(RGB_LOOKUP[level][0], RGB_LOOKUP[level][1], RGB_LOOKUP[level][2]);
							}
							else {
								int pos = 179 - level;
								setcenter(RGB_LOOKUP[pos][0], RGB_LOOKUP[pos][1], RGB_LOOKUP[pos][2]);

							}

						}
						else{
							//setcenter(red, green, blue);
							set_colour(49,2,0,0);
							//setoutside(2,0,0);
						}

			}


		}

}
