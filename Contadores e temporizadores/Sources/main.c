#include <xc.h>
//______________________________________________________________________________
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

//______________________________________________________________________________
// Fun��es das ISRs
void interrupt NoPriorityISR(void)
{
    int cont;
    if(PIR1bits.TMR1IF == 1)
    {
        PIR1bits.TMR1IF = 0;    // Limpa a flag do Timer 1
        TMR1H = 0x0B;
        TMR1L = 0xDC;           // Adiciona o CI para nova contagem
        cont++;
        if(cont == 5)
        {
            PORTDbits.RD0 ^= 1; // Liga e desliga o led
        } // end if cont = 5
        
        if(cont >= 5)           // Conta at� 5
        { 
            cont = 0;
        }   // end if cont > 5
        
    } // end if  flag 1
    
    if(PIR2bits.TMR3IF == 1)
    {
        PIR2bits.TMR3IF = 0;  // Limpa a flag do Timer 3
        TMR3H = 0x3C;
        TMR3L = 0xB0;         // Adiciona o CI para nova contagem
        PORTDbits.RD1 ^= 1;   // ^= OU EXCLUSIVO: Temos que ter pelo menos 1 nivel l�gico alto  
    } // end if flag 3

} // end interrupt

//______________________________________________________________________________
// Fun��o Principal
void main(void)
{
    INTCON2bits.RBPU = 0;   // Habilita resistores de Pull-up
    ADCON1 = 0x0F;
    PORTD = 0xFF;           // N�o permite que os leds acendam
    TRISD = 0x00;           // Configura dos pinos D como sa�da
    
    PIE1bits.TMR1IE = 1;    // Habilita interrup��o por estouro
    PIE2bits.TMR3IE = 1;
    
    // CONFIGURA��ES DAS INTERRUP��ES
    RCONbits.IPEN = 0;          // N�o existe prioridade de interrup��es
    INTCONbits.GIE = 1;         // Habilita todas as interrup��es 
    INTCONbits.PEIE = 1;        // Habilita todas as interrup��es perif�ricas
    
    // T=100ms -> CI = Tov - T / (Tcy - PS) = 65536 - 100m / (200n * 8) -> CI = 3036 = 0xBDC
    TMR1H = 0x0B;
    TMR1L = 0xDC;
    
    // Configura��o do Timer 1
    T1CON = 0b00110101; //0b00111011 errei
    // bit 
    // bit 6: T1RUN : v� se o timer 1 est� habilitado ou desabilitado
    // bit 4 e 5: T1CKPS1 : T1CKPS0: Prescale de 1:8
    // bit 3: Habilita o oscilador do timer 1
    // bit 2: Quando bit 1 for 0, este bit � ignorado: quando eu tiver uma fonte de contagens, eu preciso ter sincronismo do pino externo e o clock
    // bit 1 = 0: Oscilador interno
    // bit 0: Liga o Timer 1
    
    // T=80ms -> CI = Tov - T / (Tcy - PS) = 65536 - 80m / (200n * 8) -> CI = 15536 = 0x3CB0
    TMR3H = 0x3C;
    TMR3L = 0xB0;
    
    // Configura��o do Timer 3
    T3CON  = 0b00110001;
    // bit 7 = 0: Utiliza duas "palavras" de 8 bits
    // bit 6 e 3 Como n�o estou trabalhando com CCP posso colocar qualquer valor nestes bits
    // bit 5 a 4 Valor de Prescalre de 1:8 = 11
    // bit 2 = 0 Utiliza Clock interno
    // bit 1 = 1 Habilita Timer 3
    
    
    while(1);               // Loop infinito
    
} //end main