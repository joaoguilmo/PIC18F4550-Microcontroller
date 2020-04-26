#include <xc.h>

// Configurações
#pragma config PLLDIV = 5           // PLL para 20MHz
#pragma config CPUDIV = OSC1_PLL2   // PLL desligado
#pragma config FOSC = HS            // Fosc = 20MHz; Tcy = 200ns
#pragma config WDT = OFF            // Watchdog timer desativado
#pragma config PBADEN = OFF         // Pinos do PORTB começam como digitais
#pragma config LVP = OFF            // Desabilita gravação em baixa tensão
#pragma config DEBUG = ON           // Habilita debug

#define _XTAL_FREQ 20000000

// Variável global
char ch;

// Função da ISR
void interrupt NoPriorityISR (void)
{
    PIR1bits.RCIF = 0;              // zera flag da interrupção
    ch = RCREG;                     // grava o char recebido
}

void main(void)
{
    ADCON1 = 0x0F;                  // Configura pinos como Digitais
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;           // Configuração dos pinos RE0 e RE1 com saída
    
    PORTE = 0x00;                   // Leds iniciam apagados
    
    TRISCbits.TRISC2 = 0;           // RC2 como saída
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;           // RC6 e RC7 como entrada
    
    TXSTAbits.BRGH = 0 ;            // Baixa velocidade de comunicação (Nós escolhemos se é baixa ou alta velocidade)
    TXSTAbits.SYNC = 0 ;            // Comunicação assíncrona
    RCSTAbits.SPEN = 1 ;            // Habilita Porta Serial
    RCSTAbits.CREN = 1 ;            // Recebimento de dados contínuo
    BAUDCONbits.BRG16 = 1 ;         // BRG possui 16 bits
    SPBRGH = 0 ;
    SPBRG = 64 ;                    // BR=19200, n = 64 (cabe em 8 bits)
    
    // Configuração do PWM
    PR2 = 155;                       // PR2 = (TPWM / 4*TOSC*PRESCALE DO TMR2)-1 = ((1/2K) / (4*50e-9*16)) - 1 = 155,25 ~= 155
    T2CON = 0b00000111;             // Bits 1-0: Prescale de 16
                                    // Bit 2: Timer2 is on
                                    // Bits 6-3: 0000 - Postscale 1:1
                                    // Bit 7: Não configurado
    
    // Configuração de Interrupção
    PIR1bits.RCIF = 0 ;             // Zera flag da interrupção RX USART
    PIE1bits.RCIE = 1 ;             // Habilita interrupção RX USART
    RCONbits.IPEN = 0 ;             // Interrupções sem prioridade
    INTCONbits.PEIE = 1 ;           // Habilita interrupções periféricos
    INTCONbits.GIE = 1 ;            // Habilita interrupções geral
    
    while(1)
    {
        switch(ch)              
        {
            case 'L':   
            case 'l':
            PORTEbits.RE1 = 1;                          // Acende led vermelho
            PORTEbits.RE0 = 0;                          // Apaga led verde
            CCP1CON = 0b00001100;                       // CCP1 configurado como PWM
            // Cálculo do Duty Cycle de 90%:
            CCPR1L = 187/4; // ou 2x shift para direita 561>>2                        //  número de 10 bits = ((PR2 + 1)*4)*DC = (38+1)*4*0,9 = 140,4 ~= 140
            CCP1CONbits.DC1B = 187%4; 
            break;
            
            case 'H':
            case 'h':
            PORTEbits.RE1 = 0;                          // Apaga led vermelho
            PORTEbits.RE0 = 1;                          // Acende led verde
            CCP1CON = 0b00001100;                       // CCP1 configurado como PWM
            // Cálculo do Duty Cycle de 90%:
            CCPR1L = 561/4; // ou 2x shift para direita 561>>2                        //  número de 10 bits = ((PR2 + 1)*4)*DC = (38+1)*4*0,9 = 140,4 ~= 140
            CCP1CONbits.DC1B = 561%4; 
            break;
            
            default:
            PORTD = 0x00 ;                              // Receber char diferente apaga os LEDs
        }
    }
}