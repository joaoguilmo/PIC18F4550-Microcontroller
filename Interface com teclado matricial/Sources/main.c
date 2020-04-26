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
#define delay 6000

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
int config_relogio()
{
    int valor, minutos_d = 0, minutos_u = 0, segundos_d = 0, segundos_u = 0;
    do
    {
    minutos_d = teclado_matricial();
    if(minutos_d >= 6){minutos_d = 10;}
    PORTD = disp(minutos_d);
    DISP4 = 1;
    __delay_us(25);
    }while(minutos_d == 10);
    do
    {
    minutos_u = teclado_matricial();
    PORTD = disp(minutos_d);
    DISP4 = 1;
    __delay_us(25);
    DISP4 = 0;
    PORTD = disp(minutos_u);
    DISP3 = 1;
    __delay_us(25);
    DISP3 = 0;
    }while(minutos_u == 10);
    do
    {
    segundos_d = teclado_matricial();
    if(segundos_d >= 6){segundos_d = 10;}
    PORTD = disp(minutos_d);
    DISP4 = 1;
    __delay_us(25);
    DISP4 = 0;
    PORTD = disp(minutos_u);
    DISP3 = 1;
    __delay_us(25);
    DISP3 = 0;
    PORTD = disp(segundos_d);
    DISP2 = 1;
    __delay_us(25);
    DISP2 = 0;
    }while(segundos_d == 10);
    do
    {
    segundos_u = teclado_matricial();
    PORTD = disp(minutos_d);
    DISP4 = 1;
    __delay_us(25);
    DISP4 = 0;
    PORTD = disp(minutos_u);
    DISP3 = 1;
    __delay_us(25);
    DISP3 = 0;
    PORTD = disp(segundos_d);
    DISP2 = 1;
    __delay_us(25);
    DISP2 = 0;
    PORTD = disp(segundos_u);
    DISP1 = 1;
    __delay_us(25);
    DISP1 = 0;
    }while(segundos_u == 10);
    minutos_d = minutos_d*1000;
            minutos_u *=100;
            segundos_d *= 10;
            valor = minutos_d + minutos_u + segundos_d + segundos_u;
    return valor;
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
    int valor = 0, minutos = 0, segundos = 0, minutos_d = 0, minutos_u = 0, segundos_d = 0, segundos_u = 0;
    int count_seg = 0, count_min = 0;
    char a = 0;
    valor = config_relogio();
    count_min = valor/100;
    count_seg = valor%100;
    while (1) {
        
        minutos = count_min;
        segundos = count_seg;
        minutos_d = minutos /10;
        minutos_u = minutos %10;
        segundos_d = segundos /10;
        segundos_u = segundos %10;
        PORTD = disp(minutos_d);
        DISP4 = 1;
        __delay_ms(5);
        DISP4 = 0;
        PORTD = disp(minutos_u);
        DISP3 = 1;
        __delay_ms(5);
        DISP3 = 0;
        PORTD = disp(segundos_d);
        DISP2 = 1;
        __delay_ms(5);
        DISP2 = 0;
        PORTD = disp(segundos_u);
        DISP1 = 1;
        __delay_ms(5);
        DISP1 = 0;
        a++;
        if(a == 50 )
        {
            count_seg++;
            a = 0;
        }
        /*PORTD = 0b10000000;
        DISP2 = 1;
        __delay_ms(100);
        DISP2 = 0;*/
        if (count_seg == 59) 
        {
            count_seg = 0;
            count_min++;
            if (count_min == 59) 
            {
                count_min = 0;
                count_seg = 0;
            }
        }

    }
}
