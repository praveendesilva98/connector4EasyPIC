#include "uart.h"
#include "glcd.h"

//RC6=TX RC7=RX
//#pragma config FOSC=HS,MCLRE=OFF,WDT=OFF,LVP=OFF,DEBUG=ON



void uartInit()
{
    //TRISC=0xff; // bit 6 && 7 of port C must be set to 1 (but we already did it in init.c)
    // UART initialization settings
    TXSTAbits.TX9=0;
    TXSTAbits.TXEN=1;
    TXSTAbits.SYNC=0;
    TXSTAbits.BRGH=0;
    RCSTAbits.SPEN=1;
    RCSTAbits.CREN=1;
    SPBRG=12; // baudrate=Fosc/(64*(SPBRG+1)) (Fosc=8MHz)
} 

// not used
char uartRead()
{
    if(PIR1bits.RCIF!=0) return RCREG;
    return 0;
}

// not used
int uartAvailable()
{
    if(PIR1bits.RCIF!=0) return 1;
    return 0;
}

// check if TX bus is free, if yes send data
int uartWrite(char c)
{
    
    if(PIR1bits.TXIF!=0)
    {
        TXREG=c;        // send char c through serial port
        return 1;       
    }
    return 0;
} 

// not used
void uartWriteString(char *text)
{
    int i;
    for(i=0; i<15; i++)
    {
        uartWrite(text[i]);
    }
}
