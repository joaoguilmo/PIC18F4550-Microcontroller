/*
 * File:   main.c
 * Author: jguil
 *
 * Created on 26 de Março de 2019, 09:24
 */
#pragma config PLLDIV = 5 // PLL para 20MHz
#pragma config CPUDIV = OSC1_PLL2 // PLL desligado
#pragma config FOSC = HS // Fosc = 20MHz; Tcy = 200ns
#pragma config WDT = OFF // Watchdog desativado
#pragma config PBADEN = OFF // Pinos do PORTB começam como digitais
#pragma config LVP = OFF // Desabilita gravação em baixa tensão
#pragma config DEBUG = ON // habilita debug
#define _XTAL_FREQ 20000000
#include <xc.h>

void main(void) 
{
TRISD = 0x00;
TRISC = 0b00000011;
PORTD = 0xFF;
int i=0;
while(1)
{
    if(PORTCbits.RC0 == 0)
    {
       __delay_us(150);
        if(PORTCbits.RC0 == 1)
        {
            if(i <= 9)
             {
                i++;
                PORTD = ~i;
            }   
        }
      
      
    }
    if(PORTCbits.RC1 == 0)
    {
        __delay_us(150);
        if(PORTCbits.RC1 == 1)
        {
          
        if(i > 0)
        {
         i--;
         PORTD = ~i;
        }    
        }
      
    }
}
}
