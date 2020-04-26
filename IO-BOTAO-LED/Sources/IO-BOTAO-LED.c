#include<xc.h>
#pragma config PLLDIV = 5 // PLL para 20MHz
#pragma config CPUDIV = OSC1_PLL2 // PLL desligado
#pragma config FOSC = HS // Fosc = 20MHz; Tcy = 200ns
#pragma config WDT = OFF // Watchdog desativado
#pragma config PBADEN = OFF // Pinos do PORTB começam como digitais
#pragma config LVP = OFF // Desabilita gravação em baixa tensão

void main(void) {
    TRISDbits.TRISD0 = 1;
    TRISDbits.TRISD1 = 1;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    while (1) {
        if (PORTDbits.RD0) {
            PORTBbits.RB1 = 0;
            PORTBbits.RB0 = 1;
        } else {
            PORTBbits.RB1 = 1;
            PORTBbits.RB0 = 0;
        }
    }
}