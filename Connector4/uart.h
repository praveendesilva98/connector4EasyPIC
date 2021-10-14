#ifndef UART_H
#define	UART_H

#include <htc.h>

void uartInit();
char uartRead();
int uartAvailable();
int uartWrite(char c);
void uartWriteString(char *text);
//void main(void);

#endif

