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

char i = 0;

// CONFIG PWM
// fpwm = 10kHz
// Tpwm = 100ns
// PR2 = (Tpwm / (4 * Tosc * PS)) + 1
// PR2 = (0.1m / (4 * 50n * 2)) + 1 ====== 249
// DC = [DC 9:0] / ((PR2 + 1)*4)
//      [DC 9:0] muda para cada passo

void interrupt NoPriorityISR(void) {
    PIR1bits.TMR1IF = 0;
    // contagem inicial timer1
    TMR1H = 0xeb;
    TMR1L = 0xa7;
    
    // DC[9:0] para geracao do clock
    char auxL[16] = {63,86,107,120,125,120,107,86,63,39,18,5,0,5,18,39};
    char auxH[16] = {2,2,3,1,0,1,3,2,2,2,1,3,0,3,1,2};
    
    
    CCPR1L = auxL[i];
    CCP1CONbits.CCP1M = 0b1100;
    CCP1CONbits.DC1B = auxH[i];
    
    i++;
    if(i == 16)i = 0;
}



void main(void){
    ADCON1 = 0x0f; // configura port como I/O digital
    TRISCbits.TRISC2 = 0;
    
// Config Timer 1
    T1CONbits.RD16 = 0;     // habilita registrador de leitura/escrita: 1 - 16bit
                            //                                          0 - 2x8bit
    T1CONbits.T1RUN = 0;    // escolhe clock: 1 - clock do timer1
                            //                0 - outro clock
    T1CONbits.T1CKPS1 = 0;  // prescale bit1 
    T1CONbits.T1CKPS0 = 0;  // prescale bit0 - Prescale 1:8
    T1CONbits.T1OSCEN = 0;  // halilita clock timer: 1 - enable
                            //                       0 - shut off
    T1CONbits.TMR1CS = 0;   // seleciona clock: 1 - clock externo
                            //                  0 - clock interno
    T1CONbits.TMR1ON = 1;   // habilita timer: 1 - enable
                            //                 0 - stops 
    PIE1bits.TMR1IE = 1;    // habilita interrupcao por estouro: 1 - enable
                            //                                   0 - disable

     // Config interrupcao
    RCONbits.IPEN = 0;      // habilita prioridade de interrupcao: 1 - enable
                            //                                     0 - disable
    INTCONbits.GIE = 1;     // habilita interrupcao: 1 - enable
                            //                       0 - disable
    INTCONbits.PEIE = 1;    // habilita interrupcao de periferico: 1 - enable
                            //                                     0 - disable
    
    // contagem inicial timer1
    TMR1H = 0xeb;
    TMR1L = 0xa7;
    
    // confg pwm e timer 2
    PR2 = 124;
    T2CON = 0b00000101;
    
    while(1);
}