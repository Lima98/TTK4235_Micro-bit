#include "uart.h"
#include "gpio.h"


#define UART ((NRF_UART_REG*) 0x40002000)

void uart_init(){
    //RX pin to input
    GPIO0->PIN_CNF[6] = 0;
    //TX pin to output
    GPIO1->PIN_CNF[8] = 1;

    UART->PSEL_RTS =~ 0;
    UART->PSEL_CTS =~ 0;
    UART->PSEL_RXD = 0x28; //6
    UART->PSEL_TXD = 0x06; //8
    UART->BAUDRATE = 0x00275000; // 9600
    UART->ENABLE = 4;
    UART->TASKS_STARTRX = 1;

}

void uart_send(char letter){
    UART->TASKS_STARTTX = 1;
    UART->TXD = letter;
    while (!(UART->EVENTS_TXDRDY));
    UART->EVENTS_TXDRDY = 0;
    UART->TASKS_STOPTX = 1;
}

char uart_read(){
    UART->TASKS_STARTRX = 1;
    if (UART->EVENTS_RXDRDY){
    UART->EVENTS_RXDRDY = 0;
    return UART->RXD;
    }
    return '\0';

}

