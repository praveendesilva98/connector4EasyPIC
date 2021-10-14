#include "main.h"


#pragma config FOSC = HS 		//oscillator HS
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config WDT = OFF 		//Disable watchDog
#pragma config LVP = OFF 		//Disable low voltage programmig
#pragma config DEBUG = ON		//Debug ON

//void menu_game();


void main (void)
{
    // call the function that will init the PIC
    initMyPIC18F();
    glcd_Init(GLCD_ON);
    
    OSCCON|=0x70;
    //ADCON1=0x0F;
    CMCON=0x07;
    //TRISB=0;
    uartInit();
    ADCON0bits.CHS = 0;         //select analog input, AN0
    ADCON0bits.ADON = 1; 
    
    opening_menu();

}// end of the main function

// make sure to have an empty LAST line in any *.c file (just hit an Enter)!

