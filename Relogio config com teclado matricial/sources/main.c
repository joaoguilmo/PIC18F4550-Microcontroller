#pragma config PLLDIV = 5 // PLL para 20MHz
#pragma config CPUDIV = OSC1_PLL2 // PLL desligado
#pragma config FOSC = HS // Fosc = 20MHz; Tcy = 200ns
#pragma config WDT = OFF // Watchdog timer desativado
#pragma config PBADEN = OFF // Pinos do PORTB começam como digitais
#pragma config LVP = OFF // Desabilita gravação em baixa tensão
#pragma config DEBUG = ON // Habilita debug
#define _XTAL_FREQ 20000000
#include <xc.h>
#define DISP1 PORTEbits.RE2 
#define DISP2 PORTEbits.RE0 
#define DISP3 PORTAbits.RA2 
#define DISP4 PORTAbits.RA5 
#define delay 200

/*#define DISP4 PORTBbits.RB7 
#define DISP3 PORTBbits.RB6 
#define DISP2 PORTBbits.RB5 
#define DISP1 PORTBbits.RB4 
  */
char disp(char valor) {
    char disp7segcc[] = {
        0b00111111,
        0b00000110,
        0b01011011,
        0b01001111,
        0b01100110,
        0b01101101,
        0b01111101,
        0b00000111,
        0b01111111,
        0b01100111,
        0x80
    };
    return disp7segcc[valor];
}
char teclado_matricial( )
{
  PORTB = 0x07;
  NOP();        // não faz nada durante um ciclo de maquina para evitar capacitancia parazita nas chaves
  if(PORTBbits.RB4 == 0) {__delay_us(delay);while(PORTBbits.RB4 ==0){} return 1;}
  if(PORTBbits.RB5 == 0) {__delay_us(delay);while(PORTBbits.RB5 ==0){} return 4;}
  if(PORTBbits.RB6 == 0) {__delay_us(delay);while(PORTBbits.RB6 ==0){} return 7;}
  if(PORTBbits.RB7 == 0) {__delay_us(delay);while(PORTBbits.RB7 ==0){} return 14;}
    PORTB = 0x0B;
    NOP();
  if(PORTBbits.RB4 == 0) {__delay_us(delay);while(PORTBbits.RB4 ==0){} return 2;}
  if(PORTBbits.RB5 == 0) {__delay_us(delay);while(PORTBbits.RB5 ==0){} return 5;}
  if(PORTBbits.RB6 == 0) {__delay_us(delay);while(PORTBbits.RB6 ==0){} return 8;}
  if(PORTBbits.RB7 == 0) {__delay_us(delay);while(PORTBbits.RB7 ==0){} return 0;}
      PORTB = 0x0D;
      NOP();
  if(PORTBbits.RB4 == 0) {__delay_us(delay);while(PORTBbits.RB4 ==0){} return 3;}
  if(PORTBbits.RB5 == 0) {__delay_us(delay);while(PORTBbits.RB5 ==0){} return 6;}
  if(PORTBbits.RB6 == 0) {__delay_us(delay);while(PORTBbits.RB6 ==0){} return 9;}
  if(PORTBbits.RB7 == 0) {__delay_us(delay);while(PORTBbits.RB7 ==0){} return 15;}
        PORTB = 0x0E;
        NOP();
  if(PORTBbits.RB4 == 0) {__delay_us(delay);while(PORTBbits.RB4 ==0){} return 10;}
  if(PORTBbits.RB5 == 0) {__delay_us(delay);while(PORTBbits.RB5 ==0){} return 11;}
  if(PORTBbits.RB6 == 0) {__delay_us(delay);while(PORTBbits.RB6 ==0){} return 12;}
  if(PORTBbits.RB7 == 0) {__delay_us(delay);while(PORTBbits.RB7 ==0){} return 13;}
        return 10;
}
void config_disp(int valor, int cont)
{
    int valor1, valor2, valor3, valor4;
    if(cont == 4)
    {
        valor4 = valor;
    }
    if(cont == 3)
    {
        valor3 = valor;
    }
    if(cont == 2)
    {
        valor2 = valor;
    }
    if(cont == 1)
    {
        valor1 = valor;
    }
    PORTD = disp(valor4);
    DISP4 = 1;
    __delay_us(delay);
    DISP4 = 0;
        PORTD = disp(valor3);
    DISP3 = 1;
    __delay_us(delay);
    DISP3 = 0;
        PORTD = disp(valor2);
    DISP2 = 1;
    __delay_us(delay);
    DISP2 = 0;
        PORTD = disp(valor1);
    DISP1 = 1;
    __delay_us(delay);
    DISP1 = 0;
}
void main(void) {
    ADCON1 = 0x0F;
    TRISD = 0;
   /* TRISAbits.RA2 = 0;
    TRISAbits.RA5 = 0;
    TRISEbits.RE0 = 0;
    TRISEbits.RE2 = 0;*/
    TRISB = 0xF0;
    TRISEbits.RE2 = 0;
    TRISEbits.RE0 = 0;
    TRISAbits.RA5 = 0;
    TRISAbits.RA2 = 0;
    DISP1 = 0;
    DISP2 = 0;
    DISP3 = 0;
    DISP4 = 0;
    INTCON2bits.RBPU = 0;   //ativa resistor de pull up no portb
    int valor = 0,cont=4;
    while (1) 
    {
    valor = teclado_matricial();
    if(valor != 10)
    {
    config_disp(valor,cont);
    if(cont>0)
    {
    cont--;    
    }
    else
    {
        cont = 4;
    }
    }
    else
    {
        config_disp(valor,cont);
    }
    }
}
