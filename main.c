#include <avr/io.h>
#include <util/delay.h>

int main()
{
  DDRB=_BV(PB7); //sets the data direction register of Port B to output on PB7
  while(1)
  {
    PORTB=_BV(PB7); //Outputs 1 on PB7
    _delay_ms(1000);
    PORTB=0; //Stops output on port B
    _delay_ms(1000);
  }
}