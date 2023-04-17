#include "uart.h"
#include "gpio.h"


#define UART ((NRF_UART_REG*) 0x40002000)

typedef struct {
	volatile uint32_t TASKS_STARTRX;
    volatile uint32_t TASKS_STOPRX;
    volatile uint32_t TASKS_STARTTX;
    volatile uint32_t TASKS_STOPTX;
    volatile uint32_t RESERVED_0[3];
    volatile uint32_t TASKS_SUSPEND;
    volatile uint32_t RESERVED_1[56];
    volatile uint32_t EVENTS_CTS;
    volatile uint32_t EVENTS_NCTS;
    volatile uint32_t EVENTS_RXDRDY;
    volatile uint32_t RESERVED_2[4];
    volatile uint32_t EVENTS_TXDRDY;
    volatile uint32_t RESERVED_3;
    volatile uint32_t EVENTS_ERROR;
    volatile uint32_t RESERVED_4[7];
    volatile uint32_t EVENTS_RXTO;
    volatile uint32_t RESERVED_5[46];
    volatile uint32_t SHORTS;
    volatile uint32_t RESERVED_6[64];
    volatile uint32_t INTENSET;
    volatile uint32_t INTENCLR;
    volatile uint32_t RESERVED_7[93];
    volatile uint32_t ERRORSRC;
    volatile uint32_t RESERVED_8[31];
    volatile uint32_t ENABLE;
    volatile uint32_t RESERVED_9;
    volatile uint32_t PSEL_RTS;
    volatile uint32_t PSEL_TXD;
    volatile uint32_t PSEL_CTS;
    volatile uint32_t PSEL_RXD;
    volatile uint32_t RXD;
    volatile uint32_t TXD;
    volatile uint32_t RESERVED_10;
    volatile uint32_t BAUDRATE;
    volatile uint32_t RESERVED_11[17];
    volatile uint32_t CONFIG;
} NRF_UART_REG;

void uart_init(){
    //RX pin to input
    GPIO0->PIN_CNF[6] = 0;
    //TX pin to output
    GPIO1->PIN_CNF[8] = 1;

    UART->PSEL_RTS =~ 0;
    UART->PSEL_CTS =~ 0;
    UART->PSEL_RXD = 0x28; //6
    UART->PSEL_TXD = 0x06;
    UART->BAUDRATE = 9600; // 9600
    UART->ENABLE = 4;
    UART->TASKS_STARTRX = 1;

}

void uart_send(char letter){
    /*
    UART->TASKS_STARTTX = 1;
    UART->TXD = letter;
    while (!(UART->EVENTS_TXDRDY));
    UART->EVENTS_TXDRDY = 0;
    UART->TASKS_STOPTX = 1;
    */

    UART->TASKS_STARTTX= 1;
    UART->TXD = letter;
    
    while(UART->EVENTS_TXDRDY != 1){
        //wait
    }
    UART->EVENTS_TXDRDY = 0;
    UART->TASKS_STOPTX = 1;
}

char uart_read(){
 /*
    UART->TASKS_STARTRX = 1;
    if (UART->EVENTS_RXDRDY){
    UART->EVENTS_RXDRDY = 0;
    return UART->RXD;
    }
    return '\0';
*/
    UART->TASKS_STARTRX = 1;
    if(UART->EVENTS_RXDRDY != 1){
        return '\0';
    }
    UART->EVENTS_RXDRDY = 0;
    char letter = UART->RXD;
    return letter;
}

