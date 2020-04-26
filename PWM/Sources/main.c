/*
 * File:   L8.c
 * Author: PEDRO
 *
 * Created on 21 de Maio de 2019, 10:40
 * 
 * Desenvolva um firmware capaz de:
? Verificar o estado de um botão ligado ao pino RB0 do uC.
? Configurar o CCP2 para gerar na saída RC1 um sinal PWM com frequência de 4 kHz e:
o Duty Cycle = 25%, caso o botão não esteja pressionado;
o Duty Cycle = 75%, caso o botão esteja pressionado.
? Escrever no display alfanumérico as seguintes informações:
o Linha 1: 12 Vdc Lampada
o Linha 2: Duty-Cycle = 25% OU Duty-Cycle = 75%, em função do estado do botão.
Desenvolva um firmware capaz de:
? Verificar o estado de um botão ligado ao pino RB0 do uC.
? Configurar o CCP2 para gerar na saída RC1 um sinal PWM com frequência de 4 kHz e:
o Duty Cycle = 25%, caso o botão não esteja pressionado;
o Duty Cycle = 75%, caso o botão esteja pressionado.
? Escrever no display alfanumérico as seguintes informações:
o Linha 1: 12 Vdc Lampada
o Linha 2: Duty-Cycle = 25% OU Duty-Cycle = 75%, em função do estado do botão.

 */

// Bibliotecas
#include <xc.h>
#include "nxlcd.h"

//==============================================================================

// Configurações
#pragma config PLLDIV = 5 // PLL para 20MHz
#pragma config CPUDIV = OSC1_PLL2 // PLL desligado
#pragma config FOSC = HS // Fosc = 20MHz; Tcy = 200ns
#pragma config WDT = OFF // Watchdog timer desativado
#pragma config PBADEN = OFF // Pinos do PORTB começam como digitais
#pragma config LVP = OFF // Desabilita gravação em baixa tensão
#pragma config DEBUG = ON // Habilita debug
#pragma config MCLRE = ON // Habilita MCLR e desabilita RE3 como I/O
#define _XTAL_FREQ 20000000

#pragma config CCP2MX = ON // Pino RC1 utilizado em CCP2

//Variáveis globais


//==============================================================================


// Função Principal
void main(void)
{
    TRISD = 0;
    TRISE = 0;
    TRISCbits.TRISC1 = 0;
    TRISBbits.TRISB0 = 1;           // Pino RB0 é entrada
    INTCON2bits.RBPU = 0;
 
    // Configuração do TIMER2
    PR2 = 77;
    
    // BIT 7 = 0 --> Não implementado
    // Bit 63 = 0000 --> Postscale 1:1
    // Bit 2 = 1 --> Timer2 ligado
    // Bit 1,0 = 11 --> Prescaler 16.
    T2CON = 0b00000111;
    
    CCPR2L =  0b01110101;
    CCP2CON = 0b00011100;  // CCP2 em modo PWM
            
    // Configurar CCP2 modo PWM com DC=25%
    CCPR2L = 0b00010011; 
    CCP2CONbits.DC2B =  78&3; // CCP2 em modo PWM
    
    // Display Alfanumérico
    OpenXLCD(FOUR_BIT & LINES_5X7); // Modo 4 bits de dados e caracteres 5x7
    WriteCmdXLCD(0x01);             // Limpa o LCD com retorno do cursor
    __delay_ms(2);                  // Atraso de 2ms para aguardar a execução do comando
    
    WriteCmdXLCD(0x81);             // Começa a escrever na 1ª linha, 2ª coluna
    putrsXLCD("12 Vdc Lampada");           // Escreve a string no display
    WriteCmdXLCD(0x0C);             // Faz o cursor não piscar   
    
    
    //=======================================================================================
    while(1)
    {
        // Leitura do botão
        if(PORTBbits.RB0 == 0)          // Se o botão for pressionado    
        {
            __delay_us(150);
            WriteCmdXLCD(0xC0);             // Começa a escrever na 2ª linha, 1ª coluna
            putrsXLCD("Duty Cycle = 75%");  // Escreve a string no display
            CCPR2L =  0b00111010;
            CCP2CON = 0b00101100;

        }

        else                            // Se o botão não for pressionado              
        {
            WriteCmdXLCD(0xC0);             // Começa a escrever na 2ª linha, 1ª coluna
            putrsXLCD("Duty Cycle = 25%");  // Escreve a string no display
            CCPR2L = 0b00010011; 
            CCP2CONbits.DC2B =  78&3;
        }
    }
    //=======================================================================================
    
} // Fim função principal
