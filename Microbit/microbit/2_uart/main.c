#include "uart.h"
#include <stdint.h>

#define GPIO0 ((NRF_GPIO_REGS0*)__GPIO_BASE_ADDRESS__) //gpio 0 base adress
#define GPIO1 ((NRF_GPIO_REGS1*)__GPIO_BASE_ADDRESS__) //gpio 1 base adress

#define BTN_PIN_A 14;
#define BTN_PIN_B 23;
int matrix_status;

typedef struct {
	volatile uint32_t RESERVED0[321];
	volatile uint32_t OUT;
	volatile uint32_t OUTSET;
	volatile uint32_t OUTCLR;
	volatile uint32_t IN;
	volatile uint32_t DIR;
	volatile uint32_t DIRSET;
	volatile uint32_t DIRCLR;
	volatile uint32_t LATCH;
	volatile uint32_t DETECTMODE;
	volatile uint32_t RESERVED1_0[118];
	volatile uint32_t PIN_CNF[32];
} NRF_GPIO_REGS0;

typedef struct {
	volatile uint32_t RESERVED0[321];
	volatile uint32_t OUT;
	volatile uint32_t OUTSET;
	volatile uint32_t OUTCLR;
	volatile uint32_t IN;
	volatile uint32_t DIR;
	volatile uint32_t DIRSET;
	volatile uint32_t DIRCLR;
	volatile uint32_t LATCH;
	volatile uint32_t DETECTMODE;
	volatile uint32_t RESERVED1_1[118];
	volatile uint32_t PIN_CNF[10];
} NRF_GPIO_REGS1;

void toggle_matrix(){
    if(!matrix_status){
        __GPIO1__->OUTSET = (1 << 21);
        __GPIO1__->OUTSET = (1 << 22);
        __GPIO1__->OUTSET = (1 << 15);
        __GPIO1__->OUTSET = (1 << 24);
        __GPIO1__->OUTSET = (1 << 19);

        __GPIO1__->OUTSET = (1 << 28);
        __GPIO1__->OUTSET = (1 << 11);
        __GPIO1__->OUTSET = (1 << 31);
        __GPIO2__->OUTSET = (1 << 5);
        __GPIO1__->OUTSET = (1 << 30);
    }
    else{
        __GPIO1__->OUTCLR = (1 << 21);
        __GPIO1__->OUTCLR = (1 << 22);
        __GPIO1__->OUTCLR = (1 << 15);
        __GPIO1__->OUTCLR = (1 << 24);
        __GPIO1__->OUTCLR = (1 << 19);

        __GPIO1__->OUTCLR = (1 << 28);
        __GPIO1__->OUTCLR = (1 << 11);
        __GPIO1__->OUTCLR = (1 << 31);
        __GPIO2__->OUTCLR = (1 << 5);
        __GPIO1__->OUTCLR = (1 << 30);
    }
}

int main(){

	// Configure leds (dere må sjekke selv hvilken GPIO modul de ulike knappene tilhører)
	__GPIO1__->PIN_CNF[21] = 1; //Row 1
	__GPIO1__->PIN_CNF[22] = 1; //Row 2
	__GPIO1__->PIN_CNF[15] = 1; //Row 3
	__GPIO1__->PIN_CNF[24] = 1; //Row 4
	__GPIO1__->PIN_CNF[19] = 1; //Row 5

	__GPIO1__->PIN_CNF[28] = 1; //Col 1
	__GPIO1__->PIN_CNF[11] = 1; //Col 2
	__GPIO1__->PIN_CNF[31] = 1; //Col 3
	__GPIO2__->PIN_CNF[5] = 1;  //Col 4
	__GPIO1__->PIN_CNF[30] = 1; //Col 5 
	
    matrix_status = 1;

	// Configure buttons (dere må sjekke selv hvilken GPIO modul de ulike knappene tilhører)
	__GPIO1__->PIN_CNF[BTN_PIN_A] = 0; // button A 
	__GPIO1__->PIN_CNF[BTN_PIN_B] = 0; // button B
	
	int sleep = 0;
	while(1){

		/* Check if button B is pressed;
		 * turn on LED matrix if it is. */
		if( !((1 << BTN_PIN_B) & __GPIO1__->IN) ){
            uart_send('B');
            toggle_matrix();
		}

		/* Check if button A is pressed;
		 * turn off LED matrix if it is. */
		if( !((1 << BTN_PIN_A) & __GPIO1__->IN) ){
			uart_send('A');
            toggle_matrix();
		}

		sleep = 10000;
		while(--sleep);
	}
	return 0;
}


