#include <stdbool.h>
#include <stm8s.h>
#include <stdio.h>
#include "main.h"
#include "milis.h"
//#include "delay.h"
//#include "uart1.h"
#include "daughterboard.h"

uint32_t time = 0;
unsigned char string[20] = "zacatek";
unsigned char tmp_string[20];
unsigned char bagr;
uint8_t index = 0;
uint8_t k = 0;


void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz

    GPIO_Init(LED1_PORT, LED1_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(S1_PORT, S1_PIN, GPIO_MODE_IN_FL_NO_IT);

    init_milis();


    //UART
    UART1_DeInit();
    UART1_Init(9600,
        UART1_WORDLENGTH_8D,
        UART1_STOPBITS_1,
        UART1_PARITY_NO, 
        UART1_SYNCMODE_CLOCK_DISABLE, 
        UART1_MODE_TXRX_ENABLE);


    enableInterrupts(); // globálně povolí přerušovací systém
    UART1_ITConfig(UART1_IT_RXNE, ENABLE); // povolí přerušení při přijmu znaků
}


int putchar (int c) 
{
    while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
        ;
    UART1_SendData8(c);
    return c;
}


void rx_action(void)
{
    
    REVERSE(LED1);
            //vlajka zůstává nahoře dokud není předchozí znak přečten
            //při přečtení předchozích dat vlajka sama spadne 
    bagr = UART1_ReceiveData8();    //aby nám fungovalo i vypnutí pomocí klávesy


    if (bagr == '\n') {
                //obssah tmp_string nakopíruje do string
        k = 0;
        do {
            (string[k] = tmp_string[k]);
        } while (string[k++]);
        index = 0;
        tmp_string[0] = '\0';
    } else if (bagr == '\r'){
                ;
    } else {
        tmp_string[index] = bagr;
        tmp_string[index+1] = '\0';
        index++;
    }
}



int main(void)  {

    init();

    while (1) {

        //if (UART1_GetFlagStatus(UART1_FLAG_RXNE)) {
           //rx_action();
        //}
        if (milis() - time > 3333) {

            time = milis();

            //putchar('S');
            //putchar('M');
            //putchar('E');
            //putchar('G');
            //putchar('R');
            //putchar('\n');
            printf("Hola hola Smegr:\n"); 
            printf("    %s\n", string);
        }
    }
}
/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"