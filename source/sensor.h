#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
//#include "debug.h"

/*
#define CPU_FOCS 120000000  // cup runns at 12MHz.

#define _BV(bit) (1 << (bit))

#define PORTB.pin0 MotorASpeed
#define PORTB.pin1 MotorBSpeed
#define PORTB.pin2 MotorA1
#define PORTB.Pin3 MotorA2
#define PORTB.pin4 MotorB1
#define PORTB.pin5 MotorB2




#define STOP 0       //Motor Stops
#define Forward   1       //go straight
#define Backward  2       //go backwards
#define Right   3    //turn right
#define Turn180  4   //turn 180 on spot
#define LeftTurn  5  //turn left
*/
#define ADCREF_V     3.3
#define ADCMAXREAD   1023.0   /* 10 bit ADC */
void Initi();
void InitPWM();
void RotateMotorA(uint8_t,uint8_t);
void RotateMotorB(uint8_t,uint8_t);
void Movement(uint8_t);

void init_adc(void);
//unit16_t GetADC(uint8_t);
int v_load(void);
//Initialize the ADC port
int get_distance(void);



void init_adc (void)
{
    /* REFSx = 0 : Select AREF as reference
     * ADLAR = 0 : Right shift result
     *  MUXx = 0 : Default to channel 0
     */
    ADMUX = 0x00;
    /*  ADEN = 1 : Enable the ADC
     * ADPS2 = 1 : Configure ADC prescaler
     * ADPS1 = 1 : F_ADC = F_CPU / 64
     * ADPS0 = 0 :       = 187.5 kHz
     */
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);
}


int v_load(void)
{
     uint16_t adcread;

     ADMUX = 0x00;
     _delay_ms(100);
     /* Start single conversion */
     ADCSRA |= _BV ( ADSC );
     /* Wait for conversion to complete */
     while ( ADCSRA & _BV ( ADSC ) );
     adcread = ADC;
    
    // printf("ADC=%4d", adcread);  
 
     return (int) (adcread);
}





int get_distance(void)
{
     uint16_t adcread;
     ADMUX= 0x01;
     _delay_ms(100);
     /* Start single conversion */
     ADCSRA |= _BV ( ADSC );
     /* Wait for conversion to complete */
     while ( ADCSRA & _BV ( ADSC ) );
     adcread = ADC;
    
    // printf("ADC=%4d", adcread);  
 
     return (int) (adcread);
}