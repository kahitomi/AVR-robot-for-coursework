#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include "debug.h"

#define turn45Time 1100
#define turn90Time turn45Time*2-100
#define turn135Time turn45Time*3-150
#define turn180Time turn45Time*4-10
#define Anum 250
#define Bnum 150

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

//Call Movement() 



void Initi();
void InitPWM();
void RotateMotorA(uint8_t,uint8_t);
void RotateMotorB(uint8_t,uint8_t);
void Movement(uint8_t);


void Initi()
{
    DDRB=0xFF;    //Set portB as output
    //  _delay_ms(500);    //delay 500ms
  //  PORTB |= _BV(PB3);    //always enable PB3 to test if it can rotate it CW or CCW
  //  PORTB |= _BV(PB4);    //Always enable PB4 to test if it can rotate

}


void InitPWM()
{
         //Timer Clock = CPU Clock (No Prescalling)
         //Fast PWM mode
         //non-inverting PWM output 
      DDRB |= _BV(PB3); /* PWM out */
      DDRB |= _BV(PB4); /* inv. PWM out */

      TCCR0B |= _BV(CS02);      
      TCCR0B |= _BV(CS00);     //CLKIO/1024   cs 02=1 cs00=1


      TCCR0A |= _BV(WGM00);
      TCCR0A |= _BV(WGM01);
      TCCR0B |= _BV(WGM02);       //SET all thest as 1 to get Fast PWM mode with OCRA at top

      TCCR0A |= _BV(COM0A0);

      TCCR0A |= _BV(COM0B1);

   
}
//*******
//To control the speed, since the timer is 8-bit timer0 ranging from 0-255.
//         speed
// Vout=  -------- x 5v   SO THAT different speed value will change the direction
//         255 




//*Motor A control
void RotateMotorA(uint8_t dir, uint8_t speed)
{
  
  switch(dir)
  {
    case 0:  //Stop
            //disable the enable signal
            //set pin2 to logic 1
      PORTB &=~ _BV(PB0);            //Set pin 2 high(logic 0)

      //set pin3 to logic 0
      PORTB &=~ _BV(PB1);          //Set pin 3 low(logic 0)       so that the motor can instant stop
      break;
      
    case 1:  //turn clockwise
      //set pin2 to logic 0
      PORTB &=~ _BV(PB0);          //Set pin 2 low(logic 0)       
      
      //set pin3 to logic 1
      PORTB |= _BV(PB1);            //Set pin 3 high(logic 1) so that the motor can drive in another direction
      OCR0A=speed; 
      break;
      
    case 2:  //turn counter-clockwise
                       //generate the PWM output

             //set pin2 to logic 1
      PORTB |= _BV(PB0);            //Set pin 2 high(logic 1)

      //set pin3 to logic 0
      PORTB &=~ _BV(PB1);          //Set pin 3 low(logic 0)       so that the motor can drive in one direction
     OCR0A=speed;                  //generate the PWM output
      
      break;
      
  }
}
//**************

//*Motor B control
void RotateMotorB(uint8_t dir, uint8_t speed)
{
  switch(dir)
  {
    case 0:  //Stop
      PORTB &= ~_BV(PB2);            //Set pin 10 high(logic 0)
      PORTB &= ~_BV(PB5);          //Set pin 12 low(logic 0)       so that the motor can stop instantly
      break;
      
    case 1:  //turn clockwise

      //set pin10 to logic 1
      PORTB |= _BV(PB2);            //Set pin 10 high(logic 1)
      //set pin12 to logic 0
      PORTB &= ~_BV(PB5);          //Set pin 12 low(logic 0)       so that the motor can drive in one direction
      OCR0B=speed;                  //generate the PWM output
      
      break;
      
    case 2:  //turn counter-clockwise
      
       

      //set pin10 to logic 0
      PORTB &= ~_BV(PB2);          //Set pin 2 low(logic 0)       
      //set pin12 to logic 1
      PORTB |= _BV(PB5);            //Set pin 3 high(logic 1) so that the motor can drive in another direction
       OCR0B=speed;                  //generate the PWM output


      break;
      
  }
}
//*******************************

//*****Control movement
void Movement(uint8_t MoveDir)
{
   switch(MoveDir)
     {
       case 0:  //Stop
          RotateMotorA(0,0);
          RotateMotorB(0,0);
         break;
         
       case 1:  //forward              //go straight
          RotateMotorA(2,Anum);
          RotateMotorB(2,Bnum);
         break;
         
       case 2:  //backward             
          RotateMotorA(1,Anum);
          RotateMotorB(1,Bnum);    
       break;
            
       case 3:  //turn left 45
          RotateMotorA(1,Anum);
          RotateMotorB(2,Bnum);
          _delay_ms(turn45Time);
          RotateMotorA(0,0);
          RotateMotorB(0,0);
       break;


       case 4:   //turn left 90
          RotateMotorA(1,Anum);
          RotateMotorB(2,Bnum);
          _delay_ms(turn90Time);
          RotateMotorA(0,0);
          RotateMotorB(0,0);
       break;
       
      case 5:    //turn left 135
          RotateMotorA(1,Anum);
          RotateMotorB(2,Bnum);
          _delay_ms(turn135Time);
          RotateMotorA(0,0);
          RotateMotorB(0,0);
      break;

      case 6:    //turn back
          RotateMotorA(1,Anum);
          RotateMotorB(2,Bnum);
          _delay_ms(turn180Time);
          RotateMotorA(0,0);
          RotateMotorB(0,0);
      break;

      case 7:    //turn right 45
          RotateMotorA(2,Anum);
          RotateMotorB(1,Bnum);
          _delay_ms(turn45Time);
          RotateMotorA(0,0);
          RotateMotorB(0,0);
      break;

      case 8:    //turn right 90
          RotateMotorA(2,Anum);
          RotateMotorB(1,Bnum);
          _delay_ms(turn90Time);
          RotateMotorA(0,0);
          RotateMotorB(0,0);
      break;

      case 9:    //turn right 135
          RotateMotorA(2,Anum);
          RotateMotorB(1,Bnum);
          _delay_ms(turn135Time);
          RotateMotorA(0,0);
          RotateMotorB(0,0);
      break;
    }
}




