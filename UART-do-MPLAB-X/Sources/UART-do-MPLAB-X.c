/*
 * File:   main.c
 * Author: jguil
 *
 * Created on 19 de Março de 2019, 08:53
 */
#include <xc.h>
#include <stdio.h>
#pragma config FOSC = HS // Fosc = 20MHz; Tcy = 200ns
#pragma config CPUDIV = OSC1_PLL2 // OSC/1 com PLL off
#pragma config WDT = OFF // Watchdog desativado
#pragma config LVP = OFF // Desabilita gravação em baixa
#pragma config DEBUG = ON // Habilita debug
#pragma config PBADEN = OFF // Habilita MCLR
#pragma config MCLRE = ON // Habilita MCLR

void putch(unsigned char data) {
    while (!PIR1bits.TXIF) // wait until the transmitter is ready
        continue;
    TXREG = data; // send one character
}

void init_uart(void) {
    TXSTAbits.TXEN = 1; // enable transmitter
    RCSTAbits.SPEN = 1; // enable serial port
}
// Exercício 0:
/*
void main(void) {
    init_uart();

    while (1) {
        printf("\n\nApenas um exemplo de uso do printf().\n");
    }
}
 */


// Exercício 3.1: Dada a sequência de números inteiros não-nulos: 3, 5, 7, 2, 4, 6, 9, seguida por 0,
//imprimir seus quadrados
/*
void main(void) 
{
    init_uart();
    int a[] = {3,5,7,2,4,6,9,0};
    int i=0;
    int x=1;
    while(x)
    {
        for(i=0;a[i]!=0;i++)
        {
          printf("\n Valor = %d e Novo valor = %d",a[i],a[i]*a[i]);  
        }
        x = 0;
    }
    printf("\n Terminou");
}
*/

//Exercicio 3.2
//Calcular a soma dos 10 primeiros números inteiros positivos e imprimir:
//?A soma dos 10 primeiros números inteiros positivos é: {valor_calculado}?
/*
void main(void)
{
    init_uart();
    int x = 1;
    int v[] = {1,2,3,4,5,6,7,8,9,10};
    int total = 0;
    int i;
    while(x)
    {
        for(i=0;i<=9;i++)
        {
            total += v[i];
            printf("\t %d",v[i]);
        }
        
        printf("\n O valor da soma = %d \n",total);    
        x = 0;
        
    }
}
*/

// Exercicio 3.3
//3.3 - Dado um número de quatro dígitos (ex: 1234), separar cada um de seus dígitos e
//imprimir:
//?Milhar: {dígito_do_milhar}
//Centena: {dígito_da_centena}
//Dezena: {dígito_da_dezena}
//Unidade: {dígito_da_unidade}?
/*
void main(void)
{
    init_uart();
    int x = 1;
    int valor = 1234, aux=0;
    int v[4];
    
       aux=valor/1000;
       printf("\n Milhar %d",aux);
       valor = valor-(1000*aux);
       aux = valor/100;
       printf("\n Centena %d",aux);
       valor = valor-(100*aux);
       aux = valor/10;
       printf("\n Dezena %d",aux);
       valor = valor-(10*aux);     
       printf("\n Unidade %d",valor);
       while(1)
       {
           
       }
}
*/
//Exercicio 3.4
//3.4 - Dada uma variável de 8 bits sem sinal com o valor igual a 0x34, desligar o bit 5 e ligar o
//bit 1 desta variável. Imprimir o novo valor em decimal e em hexadecimal

void main(void)
{
    init_uart();
    unsigned int a = 0x34;
    unsigned int b = 0b11011111;
    a = a&b;
    a = a|0b00000010;
    printf("\n %d",a);
    printf("\n 0x%x",a);
    while(1)
    {
        
    }
}