/*
 * File:   L8.c
 * Author: PEDRO
 *
 * Created on 21 de Maio de 2019, 10:40
 * 
 * Desenvolva um firmware capaz de:
? Verificar o estado de um bot�o ligado ao pino RB0 do uC.
? Configurar o CCP2 para gerar na sa�da RC1 um sinal PWM com frequ�ncia de 4 kHz e:
o Duty Cycle = 25%, caso o bot�o n�o esteja pressionado;
o Duty Cycle = 75%, caso o bot�o esteja pressionado.
? Escrever no display alfanum�rico as seguintes informa��es:
o Linha 1: 12 Vdc Lampada
o Linha 2: Duty-Cycle = 25% OU Duty-Cycle = 75%, em fun��o do estado do bot�o.
Desenvolva um firmware capaz de:
? Verificar o estado de um bot�o ligado ao pino RB0 do uC.
? Configurar o CCP2 para gerar na sa�da RC1 um sinal PWM com frequ�ncia de 4 kHz e:
o Duty Cycle = 25%, caso o bot�o n�o esteja pressionado;
o Duty Cycle = 75%, caso o bot�o esteja pressionado.
? Escrever no display alfanum�rico as seguintes informa��es:
o Linha 1: 12 Vdc Lampada
o Linha 2: Duty-Cycle = 25% OU Duty-Cycle = 75%, em fun��o do estado do bot�o.

 */

// Bibliotecas
#include <xc.h>
#include "nxlcd.h"

//==============================================================================

// Configura��es
#pragma config PLLDIV = 5 // PLL para 20MHz
#pragma config CPUDIV = OSC1_PLL2 // PLL desligado
#pragma config FOSC = HS // Fosc = 20MHz; Tcy = 200ns
#pragma config WDT = OFF // Watchdog timer desativado
#pragma config PBADEN = OFF // Pinos do PORTB come�am como digitais
#pragma config LVP = OFF // Desabilita grava��o em baixa tens�o
#pragma config DEBUG = ON // Habilita debug
#pragma config MCLRE = ON // Habilita MCLR e desabilita RE3 como I/O
#define _XTAL_FREQ 20000000

#pragma config CCP2MX = ON // Pino RC1 utilizado em CCP2

//Vari�veis globais


//==============================================================================


// Fun��o Principal
void main(void)
{
    TRISD = 0;
    TRISE = 0;
    TRISCbits.TRISC1 = 0;
    TRISBbits.TRISB0 = 1;           // Pino RB0 � entrada
    INTCON2bits.RBPU = 0;
 
    // Configura��o do TIMER2
    PR2 = 77;
    
    // BIT 7 = 0 --> N�o implementado
    // Bit 63 = 0000 --> Postscale 1:1
    // Bit 2 = 1 --> Timer2 ligado
    // Bit 1,0 = 11 --> Prescaler 16.
    T2CON = 0b00000111;
    
    CCPR2L =  0b01110101;
    CCP2CON = 0b00011100;  // CCP2 em modo PWM
            
    // Configurar CCP2 modo PWM com DC=25%
    CCPR2L = 0b00010011; 
    CCP2CONbits.DC2B =  78&3; // CCP2 em modo PWM
    
    // Display Alfanum�rico
    OpenXLCD(FOUR_BIT & LINES_5X7); // Modo 4 bits de dados e caracteres 5x7
    WriteCmdXLCD(0x01);             // Limpa o LCD com retorno do cursor
    __delay_ms(2);                  // Atraso de 2ms para aguardar a execu��o do comando
    
    WriteCmdXLCD(0x81);             // Come�a a escrever na 1� linha, 2� coluna
    putrsXLCD("12 Vdc Lampada");           // Escreve a string no display
    WriteCmdXLCD(0x0C);             // Faz o cursor n�o piscar   
    
    
    //=======================================================================================
    while(1)
    {
        // Leitura do bot�o
        if(PORTBbits.RB0 == 0)          // Se o bot�o for pressionado    
        {
            __delay_us(150);
            WriteCmdXLCD(0xC0);             // Come�a a escrever na 2� linha, 1� coluna
            putrsXLCD("Duty Cycle = 75%");  // Escreve a string no display
            CCPR2L =  0b00111010;
            CCP2CON = 0b00101100;

        }

        else                            // Se o bot�o n�o for pressionado              
        {
            WriteCmdXLCD(0xC0);             // Come�a a escrever na 2� linha, 1� coluna
            putrsXLCD("Duty Cycle = 25%");  // Escreve a string no display
            CCPR2L = 0b00010011; 
            CCP2CONbits.DC2B =  78&3;
        }
    }
    //=======================================================================================
    
} // Fim fun��o principal
