#include <xc.h>
#include "nxlcd.h"

#pragma config FOSC = HS // Fosc = 20MHz; Tcy = 200ns
#pragma config CPUDIV = OSC1_PLL2 // OSC/1 com PLL off
#pragma config WDT = OFF // Watchdog desativado
#pragma config LVP = OFF // Desabilita gravação em baixa
#pragma config DEBUG = ON // Habilita debug
#pragma config MCLRE = ON // Habilita MCLR

void main()
{
    long int volt[10];
    long int m, x, v, dec = 0, dec1 = 0, dec2 = 0, dec3 = 0, aux = 0;
    char i=0;
    
    TRISD = 0;              // Pinos D são saídas
    TRISE = 0;              // Pinos E são saídas
    TRISAbits.TRISA0 = 1;   // Pino A0 é entrada
    
    ADCON1 = 0b00001110;    // Bit 0 - 3: Somente o Pino AN0 é analógico
                            // Bit 4: VDD como referência
                            // Bit 5: VSS como referência
                            // Bits 6 e 7: não implementados
    
    ADCON0 = 0b00000001;    // Bit 0: A/D Converter ligado 
                            // Bit 1: A/D Converter idle (oscioso)
                            // Bit 2 - 5: AN0 Acionado
    
    ADCON2 = 0b10010101;    // Resultado alinhado à direita,
                            // TACQ = 4TAD e TAD = FOSC/16
    
    __delay_ms(1);         // Delay de 50 ciclos de máquina, para descarregar o cap
     
    // Display Alfanumérico
        OpenXLCD(FOUR_BIT & LINES_5X7); // Modo 4 bits de dados e caracteres 5x7
        WriteCmdXLCD(0x01);             // Limpa o LCD com retorno do cursor
        __delay_ms(2);                  // Atraso de 2ms para aguardar a execução do comando
        
    while(1)
    {                
        
        
            ADCON0bits.GO_DONE = 1 ;            // Inicia a conversão A/D
            while(ADCON0bits.GO_DONE);
            volt[i] = 256 * ADRESH + ADRESL;    // Separa a "palavra" de 10 bits em 1 de 8 bits e 1 de 2 bits multiplicada por 256  
            i++;                                    // para juntar no fim e formar um único número
            if(i == 10)
            {
                m = (volt[0] + volt[1] + volt[2] + volt[3] + volt[4] + volt[5] + volt[6] + volt[7] + volt[8] + volt[9])/10;
                i = 0;
            }        

        /*  1 bit ----------- 4,88mV
            m     ----------- x
         */
        x = 4888 * m;                   // Tranforma de número binário para número decimal, valor em uV
        v = x / 1000000;
        aux = x%1000;
        dec1 = aux/100;
        aux = aux%100;
        dec2 = aux/10;
        dec3 = aux%10;
        
        WriteCmdXLCD(0x80);             // Começa a escrever na 1ª linha, 1ª coluna
        putrsXLCD("Conversor A / D");   // Escreve a string no display
        WriteCmdXLCD(0x0C);             // Faz o cursor não piscar  

        WriteCmdXLCD(0xC0);             // Começa a escrever na 2ª linha, 1ª coluna
        putsXLCD("V_ANO = ");           // Escreve a string no display, com espaços para colocar os valores de tensão
       
        WriteCmdXLCD(0xC7);
        putcXLCD(0x30+v);
        putsXLCD(",");
        putcXLCD(0x30 + dec1);
        putcXLCD(0x30 + dec2);
        putcXLCD(0x30 + dec3);
        WriteCmdXLCD(0xCE);
        putsXLCD("_V");
        
    }
    
}