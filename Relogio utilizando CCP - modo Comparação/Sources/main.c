/*
 * File:   L8.c
 * Author: PEDRO
 *
 * Created on 7 de Maio de 2019, 09:40
 * 
 * Desenvolva um firmware capaz de:
    ? Habilitar interrup��es e entrar em um loop infinito vazio.
    ? Gerar a temporiza��o de 1 segundo usando o CCP1 em modo de compara��o e com o TIMER 1 como base de tempo de CCP1.
    ? Tratar os contadores de um rel�gio: hora, minuto e segundo, na rotina de tratamento da interrup��o do CCP1.
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

//Vari�veis globais
int cont = 0, seg_u = 0, seg_d = 0, min_u = 0, min_d = 0, h_d = 0,h_u = 0;
int seg = 0, min = 0, h = 0;

//==============================================================================
// ISR
void interrupt NoPriorityISR(void)
{
    PIR1bits.CCP1IF = 0;                 // Limpa flag do CCP1
    cont++;
    
    WriteCmdXLCD(0xC7);                 // Come�a a escrever na 2� linha, 8� coluna
    putrsXLCD(":");                     // Escreve : no display
    WriteCmdXLCD(0xCA);                 // Come�a a escrever na 2� linha, 8� coluna
    putrsXLCD(":");                     // Escreve : no display    
    
    if(cont == 100)
    {
        seg++;
        seg_d = seg / 10;
        seg_u = seg % 10;
        WriteCmdXLCD(0xCB);                 
        putcXLCD(0x30 + seg_d);
        putcXLCD(0x30 + seg_u);
        cont = 0;
    } 
    
    if(seg > 59)
    {
        seg = 0x00;
        min++;
        min_d = min / 10;
        min_u = min % 10;
        WriteCmdXLCD(0xC8);                 
        putcXLCD(0x30 + min_d);
        putcXLCD(0x30 + min_u);
    }
    
    if(min > 59)
    {
        min = 0x00;
        h++;
        h_d = h / 10;
        h_u = h % 10;
        WriteCmdXLCD(0xC5);                 
        putcXLCD(0x30 + h_d);
        putcXLCD(0x30 + h_u);
    }
    
    if(h > 23)
    {
        h = 0x00;
        seg++;
    }
    
    
    
} // Fim do procedimento de interrup��o

// Fun��o Principal
void main(void)
{
    // A configura��o dos pinos D e E � feita pela biblioteca nxlcd
    
    //Inicializa��o do LCD
    OpenXLCD(FOUR_BIT & LINES_5X7); // Modo 4 bits de dados e caracteres 5x7
    WriteCmdXLCD(0x01);             // Limpa o LCD com retorno do cursor
    __delay_ms(2);                  // Atraso de 2ms para aguardar a execu��o do comando

    WriteCmdXLCD(0x85);             // Come�a a escrever na 1� linha, 6� coluna
    putrsXLCD("RELOGIO");           // Escreve rel�gio no display
    WriteCmdXLCD(0x0C);             // Faz o cursor n�o piscar   
     
    // Sele��o do TIMER 1 como base de tempo para o CCP1
    T3CONbits.T3CCP1 = 1;           // Bit 3 do T3CON
    T3CONbits.T3CCP2 = 0;           // Bit 6 do T3CON
    
    // Configura��o do TIMER 1
    TMR1H = 0x00;                   // Zera os registradores do Timer 1
    TMR1L = 0x00;
    
    T1CON = 0b00000001;
    // BIT 7: Habilita como 2 opera��es de 8 bits
    // BIT 6: Fonte de clock derivada de outra fonte
    // BIT 5: Prescale 1:1
    // BIT 4: Prescale 1:1
    // BIT 3: Oscilador do Timer 1 desligado
    // BIT 2: Ignorado
    // BIT 1: Fonte de clock interno
    // BIT 0: Habilita Timer 1    
            
    // Configura��o do CCP1
    CCPR1H = 0xC3;
    CCPR1L = 0x50;
    
    CCP1CON = 0b00001011;           // 4 �LTIMOS BITS (1011): Modo de compara��o: Reseta o timer
    PIR1bits.CCP1IF = 0;            // Limpa o flag do CCP1
    PIE1bits.CCP1IE = 1;            // Habilita interrup��o do CCP1
    
    
    // Configura��o das Interrup��es
    RCONbits.IPEN = 0;              // Sem n�veis de prioridade de interrup��o
    INTCONbits.GIEH = 1;            // Habilita interrup��es
    INTCONbits.GIEL = 1;            // Habilita interrup��es de perif�ricos
    
    WriteCmdXLCD(0xCB);                 
        putcXLCD(0x30 + 0);
        putcXLCD(0x30 + 0);
        
        WriteCmdXLCD(0xC8);                 
        putcXLCD(0x30 + 0);
        putcXLCD(0x30 + 0);
        
        WriteCmdXLCD(0xC5);                 
        putcXLCD(0x30 + 0);
        putcXLCD(0x30 + 0);
    // Loop infinito
    while(1);
    
} // Fim fun��o principal