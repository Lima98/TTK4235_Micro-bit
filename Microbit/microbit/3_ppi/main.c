#include "gpio.h"
#include "gpiote.h"
#include "ppi.h"

int main(){
  // Configure LED Matrix
	for(int i = 4; i <= 12; i++){
		GPIO0->DIRSET = (1 << i); //Usikker å 0 eller 1 på begge disse.
		GPIO0->OUTCLR = (1 << i);
	}

    init_PPI();
    init_GPIOTE();
  

    while(1);
    return 0;
}