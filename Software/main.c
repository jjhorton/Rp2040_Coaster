#include "pico/stdlib.h"

int main() {
	//initalise the output pin
	gpio_init(11);
	gpio_set_dir(11, GPIO_OUT);

	while(1){
		gpio_put(11,1);
		sleep_ms(1000);
		gpio_put(11,0);
		sleep_ms(1000);
		}
	}
