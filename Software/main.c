#include "pico/stdlib.h"

int main() {
	//initalise the output pins
	setup_output();

	while(1){
		gpio_put(11,1);
		sleep_ms(1000);
		gpio_put(11,0);
		sleep_ms(1000);
		}
	}
}
