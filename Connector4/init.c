#include "init.h"

// --- init the PIC18F device
void initMyPIC18F(void)
{
    //OSCCON|=0x70;
    //ADCON1=0x0F;
    //CMCON=0x07;
    TRISB = 0;      // set PORTB as output, used for GLCD
    
    TRISA = 0xFF;   // set PORTA as input, used for ADC
    
    TRISC = 0xFF;   // set PORTC as input, used for ADC
    TRISE = 0x00;   // set PORTE as output

}
// make sure to have an empty LAST line in any *.c file (just hit an Enter)!

