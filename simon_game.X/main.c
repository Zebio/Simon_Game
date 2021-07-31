/*-----------------------SIMON GAME-----------------------*/
/*
 * Arquivo:          main.c
 * Microcontrolador: PIC16F877
 */


// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.



#include <xc.h>

#include <stdio.h>
#include <stdlib.h>


/*---------------------------DEFINIÇÕES DE PROJETO----------------------------*/
/*FREQUÊNCIA DO CLOCK = 4MHz*/
#define _XTAL_FREQ 4000000 

#define AZUL         1
#define VERMELHO     2
#define AMARELO      3
#define VERDE        4

#define LED_AZUL     PORTCbits.RC5
#define LED_VERMELHO PORTCbits.RC4
#define LED_AMARELO  PORTCbits.RC6
#define LED_VERDE    PORTCbits.RC3

#define LCD1         PORTD
#define LCD2         PORTA

#define BT_Start       PORTBbits.RB0
#define BT_AZUL        PORTBbits.RB1
#define BT_VERMELHO    PORTBbits.RB2
#define BT_AMARELO     PORTBbits.RB3
#define BT_VERDE       PORTBbits.RB4

int  TMR0_aux=0;
int vetor_sequencia[12]={0,0,0,0,0,0,0,0,0,0,0,0};
int botao_press=0;

int int_para_7seg(int numero)
{
    switch(numero)
    {
        case 0:
            return 0b00111111;
            break;
        case 1:
            return 0b00000110;
            break;
        case 2:
            return 0b01011011;
            break;
        case 3:
            return 0b01001111;
            break;
        case 4:
            return 0b01100110;
            break;
        case 5:
            return 0b01101101;
            break;
        case 6:
            return 0b01111101;
            break;
        case 7:
            return 0b00000111;
            break;
        case 8:
            return 0b01111111;
            break;
        case 9:
            return 0b01101111;
            break;
        
    }
    return 0b00000000;
}

void imprime_sequencia(int sequencia)
{
    int dezenas =sequencia/10;
    int unidades=sequencia%10;
    
    LCD2= int_para_7seg (dezenas);
    LCD1= int_para_7seg (unidades);
    
}


void delay_500ms()
{
    // 500 ms = 64us(PRESCALLER) X 62(TMR0) X 125(TMR0_AUX) = 500ms
    
    TMR0=256-62;
    TMR0_aux=0;
    INTCONbits.T0IF=0;
    INTCONbits.T0IE=1;
    while(TMR0_aux<125)
    {
        
    }
    
}

void delay_100ms()
{
    // 500 ms = 64us(PRESCALLER) X 62(TMR0) X 125(TMR0_AUX) = 500ms
    
    TMR0=256-62;
    TMR0_aux=0;
    INTCONbits.T0IF=0;
    INTCONbits.T0IE=1;
    while(TMR0_aux<25)
    {
        
    }
    
}

int gera_cor_aleatoria()
{
   // unsigned char valor = 0x03 & TMR0;
   // valor++;
    return (rand()% 4)+1;
    //return (int)valor;
}

void desliga_leds()
{
    LED_AZUL=0;
    LED_VERMELHO=0;
    LED_AMARELO=0;
    LED_VERDE=0;
}

void liga_leds()
{
    LED_AZUL=1;
    LED_VERMELHO=1;
    LED_AMARELO=1;
    LED_VERDE=1;
}

void sinalizacao(int botao)
{
    switch(botao)
        {
            case AZUL:
                LED_AZUL=1;
                //GERA O BARULHO DO BUZZER PWM PARA A COR
                break;
            case VERMELHO:
                LED_VERMELHO=1;
                //GERA O BARULHO DO BUZZER PWM PARA A COR
                break;
            case AMARELO:
                LED_AMARELO=1;
                //GERA O BARULHO DO BUZZER PWM PARA A COR
                break;
            case VERDE:
                LED_VERDE=1;
                //GERA O BARULHO DO BUZZER PWM PARA A COR
                break;
            default:
                liga_leds();
        }
        delay_500ms();
        desliga_leds();//DESLIGA TODOS LEDS
        
        //FUNÇÂO DESLIGA BUZZER
    
}

void print_sequencia()
{
    int i=0;
    while (vetor_sequencia[i]!=0)
    {
        sinalizacao(vetor_sequencia[i]);
        delay_100ms();
        i++;
    }
}
void botao_pressionado()
{
    int botao=0;
    while(botao==0)
    {
        if(!BT_AZUL)
        {
            botao=AZUL;
        }
            
         if(!BT_VERMELHO)
        {
            botao=VERMELHO;
        }
            
        else if(!BT_AMARELO)
        {
           botao=AMARELO; 
        }
            
        else if(!BT_VERDE)
        {
             botao=VERDE;
        }
           
    }
    sinalizacao(botao);
    botao_press=botao;
   
    
}

int verifica_sequencia()
{
    int i=0;
    while (vetor_sequencia[i]!=0)
    {
        botao_pressionado();
        if(botao_press==vetor_sequencia[i])
        {
            delay_100ms();
            i++;
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

void limpa_vetor()
{
    for (int i=0;i<12;i++)
    {
        vetor_sequencia[i]=0;
    }
}

void __interrupt () my_isr_routine (void)
{

    if (INTCONbits.T0IF)//verifica o bit da interrupção do timer0
    {
        TMR0=256-125;
        TMR0_aux++;
        INTCONbits.T0IF=0;      
    }
    
    return;
}


void main(void) {
    OPTION_REG  = 0b00010101;//prescaller 64, fonte do timer0 clock interno, Portb ull-ups ON
    INTCON      = 0b10100000;//interrupção do timer0 ativada
    TRISC       = 0b00000000; //outputs PORTC
    TRISA       = 0b00000000; //outputs PORTA
    TRISD       = 0b00000000; //outputs PORTD
    TRISB       = 0b11111111; //ALL INPUTS PORTB
    TRISE       = 0b11111111;
    WPUB        = 0b11111111;
    IOCB        = 0b00000000;
    ANSELH      = 0b00000000;
    
    int sequencia=0;
    imprime_sequencia(sequencia);
    liga_leds();
    delay_500ms();
    desliga_leds();
    
    while(1)
    {
        sequencia=0;
        imprime_sequencia(sequencia);
        liga_leds();
        delay_500ms();
        desliga_leds();
        limpa_vetor();
        while(BT_Start)
        {
            
        }
        /* Intializes random number generator */
        srand((unsigned) TMR0);
        do 
        {    
            vetor_sequencia[sequencia]=gera_cor_aleatoria();
            sequencia++;
            imprime_sequencia(sequencia);
            print_sequencia();
            delay_100ms();
            if(verifica_sequencia()==0)
            {
               sequencia=0;
            }
            if (sequencia==12)
            {
                sequencia =0;//zerou o jogo
                liga_leds();
                delay_100ms();
                desliga_leds();
                liga_leds();
                delay_100ms();
                desliga_leds();
                liga_leds();
                delay_100ms();
                desliga_leds();
                liga_leds();
                delay_100ms();
                desliga_leds();
             }
        }while(sequencia);
    }
    
    return;
}
