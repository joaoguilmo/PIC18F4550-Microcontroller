#include <xc.h>
#include "nxlcd.h"
// Configurações
#pragma config PLLDIV = 5 // PLL para 20MHz
#pragma config CPUDIV = OSC1_PLL2 // PLL desligado
#pragma config FOSC = HS // Fosc = 20MHz; Tcy = 200ns
#pragma config WDT = OFF // Watchdog timer desativado
#pragma config PBADEN = OFF // Pinos do PORTB começam como digitais
#pragma config LVP = OFF // Desabilita gravação em baixa tensão
#pragma config DEBUG = ON // Habilita debug
#define _XTAL_FREQ 20000000
#define RS PORTEbits.RE0
#define RW PORTEbits.RE1
#define E  PORTEbits.RE2
#define delay 250000


char teclado_matricial( )
{
  PORTB = 0x07;
  NOP();        // não faz nada durante um ciclo de maquina para evitar capacitancia parazita nas chaves
  if(PORTBbits.RB4 == 0) {__delay_us(delay);while(PORTBbits.RB4 ==0){} return 1;}
  else if(PORTBbits.RB5 == 0) {__delay_us(delay);while(PORTBbits.RB5 ==0){} return 4;}
  else if(PORTBbits.RB6 == 0) {__delay_us(delay);while(PORTBbits.RB6 ==0){} return 7;}
  else if(PORTBbits.RB7 == 0) {__delay_us(delay);while(PORTBbits.RB7 ==0){} return 21;}
    PORTB = 0x0B;
    NOP();
  if(PORTBbits.RB4 == 0) {__delay_us(delay);while(PORTBbits.RB4 ==0){} return 2;}
  else if(PORTBbits.RB5 == 0) {__delay_us(delay);while(PORTBbits.RB5 ==0){} return 5;}
  else if(PORTBbits.RB6 == 0) {__delay_us(delay);while(PORTBbits.RB6 ==0){} return 8;}
  else if(PORTBbits.RB7 == 0) {__delay_us(delay);while(PORTBbits.RB7 ==0){} return 0;}
      PORTB = 0x0D;
      NOP();
  if(PORTBbits.RB4 == 0) {__delay_us(delay);while(PORTBbits.RB4 ==0){} return 3;}
  else if(PORTBbits.RB5 == 0) {__delay_us(delay);while(PORTBbits.RB5 ==0){} return 6;}
  else if(PORTBbits.RB6 == 0) {__delay_us(delay);while(PORTBbits.RB6 ==0){} return 9;}
  else if(PORTBbits.RB7 == 0) {__delay_us(delay);while(PORTBbits.RB7 ==0){} return 22;}
        PORTB = 0x0E;
        NOP();
  if(PORTBbits.RB4 == 0) {__delay_us(delay);while(PORTBbits.RB4 ==0){} return 17;}
  else if(PORTBbits.RB5 == 0) {__delay_us(delay);while(PORTBbits.RB5 ==0){} return 18;}
  else if(PORTBbits.RB6 == 0) {__delay_us(delay);while(PORTBbits.RB6 ==0){} return 19;}
  else if(PORTBbits.RB7 == 0) {__delay_us(delay);while(PORTBbits.RB7 ==0){} return 20;}
        return 0xA0;
}
void main(void) 
{
    char valor;
    INTCON2bits.RBPU = 0;
    ADCON1 = 0x0F;
    TRISB = 0xF0;
    PORTB = 0;
    TRISD = 0x00;
    PORTD = 0;
    TRISEbits.RE0 = 0;
    TRISEbits.RE1 = 0;
    TRISEbits.RE2 = 0;
    PORTEbits.RE0 = 0;
    PORTEbits.RE1 = 0;
    PORTEbits.RE2 = 0;
    OpenXLCD(FOUR_BIT & LINES_5X7);
    WriteCmdXLCD(0x01);
    __delay_ms(2);
    WriteCmdXLCD(0x87); // Seleciona a posição Coluna 5 e Linha 1
    putrsXLCD ("TECLA:");
    WriteCmdXLCD(0xC7); // Seleciona a posição Coluna 8 e Linha 1
  while(1)
 {
  valor = teclado_matricial();
  if(valor != (0xA0))
  {
     valor = 0x30 + valor;
     WriteCmdXLCD(0xC7); // Seleciona a posição Coluna 8 e Linha 1
      putcXLCD(valor);
  }
 }
}
    