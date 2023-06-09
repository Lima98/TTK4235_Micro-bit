#include "uart.h"
#include "gpio.h"
#include <stdint.h>
#include <stdio.h>

#define GPIO0 ((NRF_GPIO_REGS0*)0x50000000) //gpio 0 base adress
#define GPIO1 ((NRF_GPIO_REGS1*)0x50000300) //gpio 1 base adress

int BTN_PIN_A = 14;
int BTN_PIN_B = 23;
int matrix_status;

void toggle_matrix(){
    if(!matrix_status){
        gpio_lights_on();
		matrix_status = 1;
    }
    else{
		gpio_lights_off();
		matrix_status = 0;
		}
    }

ssize_t _write(int fd, const void *buf, size_t count){
	char * letter = (char *)(buf);
	for(int i = 0; i < count; i++){
		uart_send(*letter);
		letter++;
	}
	return count;
}

int main(){
	uart_init();
	gpio_init();
    matrix_status = 0;

	int sleep = 0;
	while(1){
		/* Check if button B is pressed;
		 * toggle the LED matrix. */
		if( !(GPIO0->IN & (1 << BTN_PIN_B)) ){
            uart_send('B');
            toggle_matrix();
		}

		/* Check if button A is pressed;
		 * toggle LED matrix. */
		if( !(GPIO0->IN & (1 << BTN_PIN_A)) ){
			uart_send('a');
            toggle_matrix();
		}

		sleep = 10000;
		while(--sleep);
	}
	return 0;
}


