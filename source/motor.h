#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include "debug.h"

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
   // PORTB |= _BV(PB3);    //always enable PB3 to test if it can rotate it CW or CCW
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
       //set pin2 to logic 1
      PORTB |= _BV(PB0);            //Set pin 2 high(logic 1)

      //set pin3 to logic 0
      PORTB &=~ _BV(PB1);          //Set pin 3 low(logic 0)       so that the motor can drive in one direction
      OCR0A=speed;                  //generate the PWM output
      
      break;
      
    case 2:  //turn counter-clockwise
      //set pin2 to logic 0
      PORTB &=~ _BV(PB0);          //Set pin 2 low(logic 0)       
      
      //set pin3 to logic 1
      PORTB |= _BV(PB1);            //Set pin 3 high(logic 1) so that the motor can drive in another direction
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
          RotateMotorA(1,200);
          RotateMotorB(1,100);
         break;
         
       case 2:  //backward             
          RotateMotorA(2,200);
          RotateMotorB(2,100);    
       break;
            
       case 3:  //turn right
          RotateMotorA(2,120);
          RotateMotorB(1,120);
       break;


       case 4:   //turn 180
          RotateMotorA(2,120);
          RotateMotorB(1,120);
       break;
       
      case 5:    //turn left
          RotateMotorA(1,120);
          RotateMotorB(2,120);
      break;

}


}

// int main(void)
// {     
//      Initi();
//      InitPWM();

//      while(1)
//       {       

//                Movement(5);
//                _delay_ms(5000);
//                Movement(3);
//                _delay_ms(5000);
//                Movement(3);
//                _delay_ms(5000);
//                Movement(4);
//                _delay_ms(5000);
//                Movement(5);
//               //  _delay_ms(3000);
//              //   RotateMotorA(1,100);
//              //   _delay_ms(3000);
//              //   RotateMotorA(0,0);
//               //  _delay_ms(3000);
//              //   RotateMotorA(2,20);
//              //   _delay_ms(3000);
//               //  RotateMotorA(2,240);
//              //   _delay_ms(3000);

                  
//          /*    if (Light <= 100)                   //if in the dark place, instant stop
//                   {    
//                     Movement(0);
//                                                   //disable the enable signal
//                     PORTB &=~ _BV(PB0);     //Set PortB pin 0 as zero, to stop sending Enable A signal    
//                     PORTB &=~ _BV(PB1);     //Set PortB pin 0 as zero, to stop sending Enable B signal 
//                   }

//              else if (Distance <= 100)                 //if in front there is an obstacle, stop and turn right on spot
//                   {
//                            Movement(0);    
//                            Movement(2);  
//                         if (Distance<=100)              //if there is still an obstacle, turn 180 on spot 
//                             {
//                                     Movement(4);             //turn 180 on spot
//                                    if (Distance<=100)
//                                     Movement(5);            //left turn  to go back to the previous point
//                             }
                                  
//                    }
//              else
//                    {                               
//                            Movement(1);                                    //go straight
//                    }   
//                      */

            
             

//        }
// }




