/*
 * File:   main.c
 * Author: jguil
 *
 * Created on 26 de Mar�o de 2019, 09:24
 */
#pragma config PLLDIV = 5 // PLL para 20MHz
#pragma config CPUDIV = OSC1_PLL2 // PLL desligado
#pragma config FOSC = HS // Fosc = 20MHz; Tcy = 200ns
#pragma config WDT = OFF // Watchdog desativado
#pragma config PBADEN = OFF // Pinos do PORTB come�am como digitais
#pragma config LVP = OFF // Desabilita grava��o em baixa tens�o
#pragma config DEBUG = ON // habilita debug
#define _XTAL_FREQ 20000000
#include <xc.h>

void main(void) {
    TRISD = 0x00;
    TRISC = 0b00000011;
    PORTD = 0xFF;
    int i = 0;
    int x = 1;
    while (1) {
        if (PORTCbits.RC0 == 0) {
            __delay_us(150);
            if (PORTCbits.RC0 == 1) {
                __delay_ms(500);
                x = 1;
                PORTD = ~x;
                for (i = 0; i <= 4; i++) {
                    __delay_ms(500);
                    x = x << 1;
                    PORTD = ~x;
                }
                PORTD = 0xFF;

            }


        }
        if (PORTCbits.RC1 == 0) {
            __delay_us(150);
            if (PORTCbits.RC1 == 1) {
                x = 0b00001000;
                __delay_ms(500);
                PORTD = ~x;
                for (i = 0; i <= 4; i++) {
                    __delay_ms(500);
                    x = x >> 1;
                    PORTD = ~x;
                }
                PORTD = 0xFF;

            }

        }
    }
}
