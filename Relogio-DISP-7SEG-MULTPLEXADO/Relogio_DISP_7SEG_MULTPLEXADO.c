

    

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

// PORTAS PARA KIT DIDATICO XM118
/*#define disp1 PORTAbits.RA5
#define disp2 PORTAbits.RA2
#define disp3 PORTEbits.RE0
#define disp4 PORTEbits.RE2
*/
// PORTAS PARA PICSIMLAB BOARD 3 
#define disp1 PORTBbits.RB7
#define disp2 PORTBbits.RB6
#define disp3 PORTBbits.RB5
#define disp4 PORTBbits.RB4

#define t_ms 5

void display4(char time,char count);
char display(char no);

void main() {
    ADCON1 = 0x0f;
    TRISD = 0x00;
    
    // PORTAS PARA KIT DIDATICO XM118
/*    TRISEbits.RE0 = 0;
    TRISEbits.RE2 = 0;    
    TRISAbits.RA2 = 0;
    TRISAbits.RA5 = 0;
*/
    // PORTAS PARA PICSIMLAB BOARD 3
    TRISBbits.RB7 = 0;
    TRISBbits.RB6 = 0;    
    TRISBbits.RB5 = 0;
    TRISBbits.RB4 = 0;    
    
    disp1 = 0;disp2 = 0;disp3 = 0;disp4 = 0;
    
    // time - valor do tempo em segundos
    // count - variavel auxiliar que conta os segundos apartir dos delays
    char time = 0,count = 0;
    
    while (time < 3600) {
        display4(time,count);
        count++;
        if(count == 50){
            time++;
            count = 0;
        }
        
    }
}

void display4(char time,char count) {
    char min1,min2,seg1,seg2;
    min1 = (time / 60) / 10;
    min2 = (time / 60) % 10;
    seg1 = (time % 60) / 10;
    seg2 = (time % 60) % 10;
    
    disp1 = 1;
    PORTD = display(min1);
    __delay_ms(t_ms);
    disp1 = 0;

    disp2 = 1;
    if(count >= 0 && count <= 5)
        PORTD = display(min2) | 0x80;
    else
        PORTD = display(min2);
    __delay_ms(t_ms);
    disp2 = 0;

    disp3 = 1;
    PORTD = display(seg1);
    __delay_ms(t_ms);
    disp3 = 0;

    disp4 = 1;
    PORTD = display(seg2);
    __delay_ms(t_ms);
    disp4 = 0;   
}

char display(char no) {
    char cc7seg[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67};
    return (cc7seg[no]);
}