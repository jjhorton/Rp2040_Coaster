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

			for(int red = 0; red<64; red++ ){
				for(int green = 0; green<64; green++ ){
					for(int blue = 0; blue<64; blue++ ){

						red=red+(rand()%8);
						green=green+(rand()%8);
						blue=blue+(rand()%8);

						int my_test = 0;
						for (int i = 0; i < (4); ++i) {
							if(gpio_get(BUTTON_PINS[i])!=1){
									my_test++;
							}
						}

						//Set the center LED's
						if(my_test>2){
							setcenter(red, green, blue);
						}
						else{
							//setcenter(red, green, blue);
							setoutside(2,0,0);
						}


					}
				}
			}


		}

}
