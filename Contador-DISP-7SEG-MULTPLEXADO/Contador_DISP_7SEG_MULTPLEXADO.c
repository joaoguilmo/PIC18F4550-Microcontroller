#include <xc.h>
// Configurações
#pragma config PLLDIV = 5 // PLL para 20MHz
#pragma config CPUDIV = OSC1_PLL2 // PLL desligado
#pragma config FOSC = HS // Fosc = 20MHz; Tcy = 200ns
#pragma config WDT = OFF // Watchdog timer desativado
#pragma config PBADEN = OFF // Pinos do PORTB começam como digitais
#pragma config LVP = OFF // Desabilita gravação em baixa tensão
#pragma config DEBUG = ON // Habilita debug
#define _XTAL_FREQ 20000000
#define DISP1 PORTEbits.RE2 
#define DISP2 PORTEbits.RE0 
#define DISP3 PORTAbits.RA2 
#define DISP4 PORTAbits.RA5 

char disp(char valor)
{
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
                       0b01100111 
                      };
    return disp7segcc[valor];
}
void verifica_exibe_valor(int i)
{
unsigned int valor = i;
char milhar, centena, dezena, unidade;
milhar = valor / 1000;
valor = valor % 1000;
centena = valor / 100;
valor = valor % 100;
dezena = valor / 10;
unidade = valor % 10;
PORTD = disp(milhar);
DISP4 = 1;
__delay_us(100);
DISP4 = 0;
PORTD = disp(centena);
DISP3 = 1;
__delay_us(100);
DISP3 = 0;
PORTD = disp(dezena);
DISP2 = 1;
__delay_us(100);
DISP2 = 0;
PORTD = disp(unidade);
DISP1 = 1;
__delay_us(100);
DISP1 = 0;
}
void main(void) 
{
ADCON1 = 0x0F;
TRISD = 0x00;
TRISB = 0xFF;
TRISAbits.RA2 = 0;
TRISAbits.RA5 = 0;
TRISEbits.RE0 = 0;
TRISEbits.RE2 = 0;
PORTD = 0x00;
DISP1 = 0;
DISP2 = 0;
DISP3 = 0;
DISP4 = 0;
int i=0;
while(1)
{
    verifica_exibe_valor(i);
    if(PORTBbits.RB0 == 0 && i<9999)
    {
        __delay_ms(150);
     if(PORTBbits.RB0 == 1)
     {
         i+=10;
     }
    }
    if(PORTBbits.RB1 == 0 && i>0)
    {
        __delay_ms(150);
     if(PORTBbits.RB1 == 1)
     {
         i-=5;
     }
    }
    
}


}
