#include <xc.h>

// Configura��es
#pragma config PLLDIV = 5           // PLL para 20MHz
#pragma config CPUDIV = OSC1_PLL2   // PLL desligado
#pragma config FOSC = HS            // Fosc = 20MHz; Tcy = 200ns
#pragma config WDT = OFF            // Watchdog timer desativado
#pragma config PBADEN = OFF         // Pinos do PORTB come�am como digitais
#pragma config LVP = OFF            // Desabilita grava��o em baixa tens�o
#pragma config DEBUG = ON           // Habilita debug

#define _XTAL_FREQ 20000000

// Vari�vel global
char ch;

// Fun��o da ISR
void interrupt NoPriorityISR (void)
{
    PIR1bits.RCIF = 0;              // zera flag da interrup��o
    ch = RCREG;                     // grava o char recebido
}

void main(void)
{
    ADCON1 = 0x0F;                  // Configura pinos como Digitais
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;           // Configura��o dos pinos RE0 e RE1 com sa�da
    
    PORTE = 0x00;                   // Leds iniciam apagados
    
    TRISCbits.TRISC2 = 0;           // RC2 como sa�da
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;           // RC6 e RC7 como entrada
    
    TXSTAbits.BRGH = 0 ;            // Baixa velocidade de comunica��o (N�s escolhemos se � baixa ou alta velocidade)
    TXSTAbits.SYNC = 0 ;            // Comunica��o ass�ncrona
    RCSTAbits.SPEN = 1 ;            // Habilita Porta Serial
    RCSTAbits.CREN = 1 ;            // Recebimento de dados cont�nuo
    BAUDCONbits.BRG16 = 1 ;         // BRG possui 16 bits
    SPBRGH = 0 ;
    SPBRG = 64 ;                    // BR=19200, n = 64 (cabe em 8 bits)
    
    // Configura��o do PWM
    PR2 = 155;                       // PR2 = (TPWM / 4*TOSC*PRESCALE DO TMR2)-1 = ((1/2K) / (4*50e-9*16)) - 1 = 155,25 ~= 155
    T2CON = 0b00000111;             // Bits 1-0: Prescale de 16
                                    // Bit 2: Timer2 is on
                                    // Bits 6-3: 0000 - Postscale 1:1
                                    // Bit 7: N�o configurado
    
    // Configura��o de Interrup��o
    PIR1bits.RCIF = 0 ;             // Zera flag da interrup��o RX USART
    PIE1bits.RCIE = 1 ;             // Habilita interrup��o RX USART
    RCONbits.IPEN = 0 ;             // Interrup��es sem prioridade
    INTCONbits.PEIE = 1 ;           // Habilita interrup��es perif�ricos
    INTCONbits.GIE = 1 ;            // Habilita interrup��es geral
    
    while(1)
    {
        switch(ch)              
        {
            case 'L':   
            case 'l':
            PORTEbits.RE1 = 1;                          // Acende led vermelho
            PORTEbits.RE0 = 0;                          // Apaga led verde
            CCP1CON = 0b00001100;                       // CCP1 configurado como PWM
            // C�lculo do Duty Cycle de 90%:
            CCPR1L = 187/4; // ou 2x shift para direita 561>>2                        //  n�mero de 10 bits = ((PR2 + 1)*4)*DC = (38+1)*4*0,9 = 140,4 ~= 140
            CCP1CONbits.DC1B = 187%4; 
            break;
            
            case 'H':
            case 'h':
            PORTEbits.RE1 = 0;                          // Apaga led vermelho
            PORTEbits.RE0 = 1;                          // Acende led verde
            CCP1CON = 0b00001100;                       // CCP1 configurado como PWM
            // C�lculo do Duty Cycle de 90%:
            CCPR1L = 561/4; // ou 2x shift para direita 561>>2                        //  n�mero de 10 bits = ((PR2 + 1)*4)*DC = (38+1)*4*0,9 = 140,4 ~= 140
            CCP1CONbits.DC1B = 561%4; 
            break;
            
            default:
            PORTD = 0x00 ;                              // Receber char diferente apaga os LEDs
        }
    }
}