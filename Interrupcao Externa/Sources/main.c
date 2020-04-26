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

// ============================================================================

// Funções das ISRs
void interrupt high_priority HighPriorityISR(void)
{
    __delay_ms(250);
    int i;
    INTCON3bits.INT1IF = 0;     // Limpa flag do INT2
    for(i=0;i<3;i++)
    {
    PORTD = 0b01010101;
    __delay_ms(250);
    
    PORTD = 0b10101010;
    __delay_ms(250);
    } // end for
            
} // end interrupt high_priority

void interrupt low_priority LowPriorityISR(void)
{
    __delay_ms(250);
    int i, j, k, aux = 0b00000001;
    INTCON3bits.INT2IF = 0;      // Limpa flag do INT2
    PORTD = 0xFF;
    for(i=0;i<3;i++)
    {
        for(j=0;j<7;j++)
        {
            aux = aux << 1;
            PORTD = ~aux;
            __delay_ms(200);
        } // end for j
        
        for(k=7; k>0; k--)
        {
            aux = aux >> 1;
            PORTD  = ~aux;
            __delay_ms(200);
        } // end for k  
        
    } // end for i
    
} // end interrupt low_priority

// ============================================================================

// Função Principal
void main(void)
{
    INTCON2bits.RBPU = 0;
    PORTD = 0xFF;
    TRISD = 0x00;
    TRISB = 0xFF;
    
    // INT1 é de alta prioridade
    INTCON3bits.INT1IF = 0;         // Limpa flag do INT1
    INTCON2bits.INTEDG1 = 0;        // Transição de descida
    INTCON3bits.INT1IE = 1;         // Habilita a interrupção externa INT1
    
    // INT2 é de baixa prioridade
    INTCON3bits.INT2IF = 0;         // Limpa flag do INT2
    INTCON2bits.INTEDG2 = 0;        // Transição de descida
    INTCON3bits.INT2IP = 0;         // Interrupção de baixa prioridade
    INTCON3bits.INT2IE = 1;         // Habilita a interrupção externa INT2
    
    RCONbits.IPEN = 1;              // Habilita prioridade de interrupção  
    INTCONbits.GIEL = 1;            // Habilita interrupção de baixa prioridade
    INTCONbits.GIEH = 1;            // Habilita interrupção de alta prioridade
    
    for(;;)
    {
        PORTD = 0x00;
        __delay_ms(250);            // Frequência de 2Hz, T = 1/f = 1/2 = 0.5 s, ou seja, fica aceso por 0,25 s e apagado por 0,25s
        PORTD = 0xFF;
        __delay_ms(250);       
    } // end for
    
} // end Função Principal