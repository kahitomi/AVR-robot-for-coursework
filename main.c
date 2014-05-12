#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <time.h>

//user source
#include "source/io.h"
#include "source/move.h"

#include "source/sensor.h"


bool moving = false; //If robot is moving
bool inDark = false; //If robot is under dark
bool arrive = false; //If robot is arrive
bool inlocalSearch = false; //If robot is in local search
short process = 0; //indicate the process: 0. Random search; 1. go to Dark places; 2. stop; 3. localsearch
short stop_sign = 0; //indicate why robot stop moving. 0: unknow, 1: obstacle, 2: under dark, 3: obstacle and dark
int homeNum = 2; // the number of dark places for robot
step *robotSteps; //store path


//functions
//Return a stop sign including why stop
short ifStop(){
	short sign = 0;
	if(haveDarkness())
	{
		sign = 1;
	}
	if(haveObstacle())
	{
		sign = 2;
	}
	if(haveObstacle() && sign == 1)
	{
		sign = 3;
	}
	return sign;
}

void Go(float x, float y){
};

void GoToUnknow(){ //Random move
	double _random = rand()%100;
	int direction = (int)ceil(_random*8/100);
	switch(direction)
	{
		case 1:
			direction = 90;
			break;
		case 2:
			direction = 135;
			break;
		case 3:
			direction = 180;
			break;
		case 4:
			direction = 225;
			break;
		case 5:
			direction = 270;
			break;
		case 6:
			direction = 315;
			break;
		case 7:
			direction = 0;
			break;
		case 8:
			direction = 45;
			break;
	}
	move(-1, direction);
}; 

void GoToDark(){ //calculate the nearest dark place the the path to it. Then move
	robotSteps = darkRoot->next;
}; 

void GoToNext(){ // go to the next testStep

};

void addO(){
	location();
	// addObstacle();
};

void addD(){
	location();
	addDark(locationX, locationY);
	if(darkLength == darkPlacesNum*2-1)
	{
		stop();
		//turn back;
		//stay under this dark place
		process = 2;
	}
};






int main(void)
{     
     Initi();
     InitPWM();

     while(1)
      {       

               Movement(1);
               _delay_ms(5000);
               // Movement(3);
               // _delay_ms(5000);
               // Movement(3);
               // _delay_ms(5000);
               // Movement(4);
               // _delay_ms(5000);
               // Movement(5);
              //  _delay_ms(3000);
             //   RotateMotorA(1,100);
             //   _delay_ms(3000);
             //   RotateMotorA(0,0);
              //  _delay_ms(3000);
             //   RotateMotorA(2,20);
             //   _delay_ms(3000);
              //  RotateMotorA(2,240);
             //   _delay_ms(3000);

                  
             if (Light <= 100)                   //if in the dark place, instant stop
                  {    
                    Movement(0);
                                                  //disable the enable signal
                    PORTB &=~ _BV(PB0);     //Set PortB pin 0 as zero, to stop sending Enable A signal    
                    PORTB &=~ _BV(PB1);     //Set PortB pin 0 as zero, to stop sending Enable B signal 
                  }

             else if (Distance <= 100)                 //if in front there is an obstacle, stop and turn right on spot
                  {
                           Movement(0);    
                           Movement(2);  
                        if (Distance<=100)              //if there is still an obstacle, turn 180 on spot 
                            {
                                    Movement(4);             //turn 180 on spot
                                   if (Distance<=100)
                                    Movement(5);            //left turn  to go back to the previous point
                            }
                                  
                   }
             else
                   {                               
                           Movement(1);                                    //go straight
                   }   
                     

            
             

       }
}







// int main(void)
// {

// 	//init
// 	initMap();

// 	////////////////
// 	//test code
// 	////////////////

// 	move(-1, 0);

// 	// addObstacle(20, 138, 60, 147);
// 	// addObstacle(45, 144, 67, 122);
// 	// addObstacle(77, 139, 47, 113);
// 	// addObstacle(49, 120, 60, 90);
// 	// addObstacle(80, 80, 64, 49);
// 	// addObstacle(71, 69, 88, 41);
// 	// addObstacle(79, 47, 114, 38);
// 	// addObstacle(91, 31, 111, 48);
// 	// addObstacle(109, 38, 140, 49);
// 	// addObstacle(123, 51, 149, 40);
// 	// addObstacle(161, 62, 170, 81);
// 	// addObstacle(161, 78, 185, 87);
// 	// showMap();

// 	// showObstracle(selObstracle(0));

// 	// printf("%c\n", ifBlock(100, 100, 90, 110, 110, 90));

// 	// step *testSteps = AStar(1,1,120,90);
// 	// showSteps(testSteps);
// 	//////////////////

// 	//Main loop
// 	for (;;) {
// 		// _delay_ms(200);!!!!!!!!!!!!!!!!!!!!!!

// 		printf("=====\n");
// 		switch(process)
// 		{
// 			case 0: //Random search
// 				printf("Now it is random search\n");
// 				if(moving == true)
// 				{
// 					stop_sign = ifStop();
// 					if(inDark == true)
// 					{
// 						//under dark
// 						if(stop_sign == 0)
// 						{
// 							//out dark
// 							addD();
// 							inDark = false;
// 						}
// 						if(stop_sign == 1)
// 						{
// 							//out dark and there is obstacle
// 							stop();
// 							addO();
// 							moving = false;
// 							addD();
// 							inDark = false;
// 						}
// 						if(stop_sign == 3)
// 						{
// 							//stop because of obstacle, under dark
// 							stop();
// 							addO();
// 							moving = false;
// 						}
// 					}
// 					else
// 					{
// 						//under light
// 						if(stop_sign == 1)
// 						{
// 							//stop because of obstacle
// 							stop();
// 							addO();
// 							moving = false;
// 						}
// 						else if(stop_sign == 2)
// 						{
// 							//under dark
// 							addD();
// 							inDark = true;
// 						}
// 						else if(stop_sign == 3)
// 						{
// 							//stop because of obstacle, under dark
// 							stop();
// 							addO();
// 							addD();
// 							inDark = true;
// 							moving = false;
// 						}
// 					}	
// 				}
// 				else
// 				{
// 					//random search
// 					GoToUnknow();
// 					moving = true;
// 				}
// 				break;
// 			case 1: //Go to Dark places
// 				if(moving == true)
// 				{
// 					stop_sign = ifStop();
// 					if(stop_sign == 1)
// 					{
// 						//obstacle
// 						if(inlocalSearch == false)
// 						{
// 							// on the way
// 							stop();
// 							addO();
// 							GoToDark();
// 						}
// 						else
// 						{
// 							//local search
// 							// localSearch();
// 						}
// 					}
// 					else if(stop_sign == 2 || stop_sign == 3)
// 					{
// 						location();
// 						if(abs(endPointX-locationX) < 100 && abs(endPointY-locationY) < 100)
// 						{
// 							moving = false;
// 							process = 2;
// 						}
// 						else
// 						{
// 							//ignore it
// 						}
// 					}
// 					if(ifReach())
// 					{
// 						moving = false;
// 						// localSearch();
// 					}
// 				}
// 				else
// 				{
// 					if(inlocalSearch == false)
// 					{
// 						GoToDark();
// 					}
// 					else
// 					{
// 						StartlocalSearch();
// 					}
// 					moving = true;
// 				}
// 				break;
// 			case 2: //In dark, stopping
// 				stop_sign = ifStop();
// 				if(stop_sign != 2)
// 				{
// 					//dark is removed
// 					homeNum--;
// 					if(homeNum == 1)
// 					{
// 						//go to back up
// 						process = 1;
// 					}
// 					else if(homeNum == 0)
// 					{
// 						//start random search again
// 						process = 0;
// 						homeNum = 2;
// 						darkLength = 0;
// 						free(darkRoot);
// 						free(darkEnd);
// 						darkRoot = (darkPlace *)malloc(sizeof(darkPlace));
// 						darkEnd = (darkPlace *)malloc(sizeof(darkPlace));
// 						darkEnd = darkRoot;
// 					}
					
// 				}
// 				break;
// 			case 3: //localsearch()
// 				break;
// 		}

// 		break; //This should be deleted!!!!!!!!!!!!
// 	}

// 	return -1;
// }