#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
// #include <time.h>

//user source
#include "source/io.h"
#include "source/move.h"


bool moving = false; //If robot is moving
bool inDark = false; //If robot is under dark
bool arrive = false; //If robot is arrive
bool inlocalSearch = false; //If robot is in local search
short process = 0; //indicate the process: 0. Random search; 1. go to Dark places; 2. stop; 3. localsearch
short stop_sign = 0; //indicate why robot stop moving. 0: unknow, 1: obstacle, 2: under dark, 3: obstacle and dark
int homeNum = 2; // the number of dark places for robot
step *robotSteps; //store path
step *robotGo; //go path

void blink(){
	PORTB=_BV(PB7); //Outputs 1 on PB7
    _delay_ms(200);
    PORTB=0;
    _delay_ms(200);
};


//functions
//Return a stop sign including why stop
short ifStop(){
	short sign = 0;
	if(haveDarkness() == true)
	{
		sign = 1;
	}
	if(haveObstacle() == true)
	{
		sign = 2;
	}
	if(haveObstacle() == true && sign == 1)
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
	if(abs(direction - angle) < 90 || abs(direction - angle) > 90)
	{
		GoToUnknow();
	}
	else
	{
		move(-1, direction);
	}
}; 

void GoToNext(){ // go to the next testStep
	location();
	float _x = robotGo->X - locationX;
	float _y = robotGo->Y - locationY;
	float _duration;
	float _angleTemp = 90;
	if(_x == 0)
	{
		_duration = _y;
		_angleTemp = 90;
	}
	else
	{
		_duration = sqrt(_x*_x+_y*_y);
		_angleTemp = (float)floor(atan(_y/_x)/PI*180/45)*45;
	}
	move(_duration, _angleTemp);
	if(robotGo->next == NULL)
	{
		if(inlocalSearch == false)
		{
			inlocalSearch = true;
			StartlocalSearch();
			robotSteps = localSearch();
		}
	}
	else
	{
		robotGo = robotGo->next;
	}
};

void GoToDark(){ //calculate the nearest dark place the the path to it. Then move
	location();
	darkPlace *d = darkRoot->next;
	endPointX = d->X;
	endPointY = d->Y;
	robotSteps = AStar(locationX, locationY, d->X, d->Y);
	robotGo = robotSteps->next;
	GoToNext();
}; 

void addO(){
	location();
	float x1,x2,y1,y2,xc,yc,n;
	float _dis = getDistance();
	if(angle == 0 || angle == 180)
	{
		xc = locationX;
		yc = locationY + cos(angle*PI/180)*_dis;
		x1 = radius+xc;
		y1 = yc;
		x1 = -radius+xc;
		y1 = yc;
	}
	else
	{
		float k = tan(angle*PI/180);
		xc = cos(angle*PI/180)*_dis + locationX;
		yc = sin(angle*PI/180)*_dis + locationY;
		n = atan(-1/k);
		x1 = -cos(n*PI/180)*radius + xc;
		y1 = -sin(n*PI/180)*radius + yc;
		x2 = cos(n*PI/180)*radius + xc;
		y2 = sin(n*PI/180)*radius + yc;
		addObstacle(x1, y1, x2, y2);
	}	
};

void addD(){ 
	location();
	darkPlace *p = darkRoot->next;
	int i = 0;
	int fake = 0;
	if(darkLength > 0)
	{
	// 	for(i; i<darkLength; i++)
	// 	{
		// if(locationX > 0 && locationX < 100)
		// {

		// }
		// else
		// {
		// 	blink();
		// }
		if(sqrt(pow(p->X - locationX, 2) + pow(p->Y - locationY, 2)) < 30)
		{
			fake = 1;
		}
	// 	}
	}

	if(fake == 0)
	{
		blink();

		addDark(locationX, locationY);
		if(darkLength == darkPlacesNum)
		{
			stop();
			// _delay_ms(1000);
			//turn back;
			Movement(6);
			// _delay_ms(1000);
			//stay under this dark place
			process = 2;
		}
	}
};






// int main(void)
// {     
//      Initi();
//      InitPWM();

//      while(1)
//       {       

//       			// Movement(1);
//                Movement(4);
//                _delay_ms(1000);
//                Movement(4);
//                _delay_ms(1000);
//                Movement(4);
//                _delay_ms(1000);
//                Movement(4);
//                _delay_ms(1000);
//                Movement(0);
//                _delay_ms(10000);

//              //   RotateMotorA(1,100);
//              //   sdelay(3000);
//              //   RotateMotorA(0,0);
//               //  sdelay(3000);
//              //   RotateMotorA(2,20);
//              //   sdelay(3000);
//               //  RotateMotorA(2,240);
//              //   sdelay(3000);

                  
//              // if (Light <= 100)                   //if in the dark place, instant stop
//              //      {    
//              //        Movement(0);
//              //                                      //disable the enable signal
//              //        PORTB &=~ _BV(PB0);     //Set PortB pin 0 as zero, to stop sending Enable A signal    
//              //        PORTB &=~ _BV(PB1);     //Set PortB pin 0 as zero, to stop sending Enable B signal 
//              //      }

//              // else if (Distance <= 100)                 //if in front there is an obstacle, stop and turn right on spot
//              //      {
//              //               Movement(0);    
//              //               Movement(2);  
//              //            if (Distance<=100)              //if there is still an obstacle, turn 180 on spot 
//              //                {
//              //                        Movement(4);             //turn 180 on spot
//              //                       if (Distance<=100)
//              //                        Movement(5);            //left turn  to go back to the previous point
//              //                }
                                  
//              //       }
//              // else
//              //       {                               
//              //               Movement(1);                                    //go straight
//              //       }   
                     

            
             

//        }
// }







int main(void)
{

	//init
	initMap();

	Initi();
    InitPWM();
    init_adc();
    // DDRB=_BV(PB7);

	////////////////
	//test code
	////////////////

	// move(-1, 0);

	// addObstacle(20, 138, 60, 147);
	// addObstacle(45, 144, 67, 122);
	// addObstacle(77, 139, 47, 113);
	// addObstacle(49, 120, 60, 90);
	// addObstacle(80, 80, 64, 49);
	// addObstacle(71, 69, 88, 41);
	// addObstacle(79, 47, 114, 38);
	// addObstacle(91, 31, 111, 48);
	// addObstacle(109, 38, 140, 49);
	// addObstacle(123, 51, 149, 40);
	// addObstacle(161, 62, 170, 81);
	// addObstacle(161, 78, 185, 87);
	// showMap();

	// showObstracle(selObstracle(0));

	// printf("%c\n", ifBlock(100, 100, 90, 110, 110, 90));

	// step *testSteps = AStar(1,1,120,90);
	// showSteps(testSteps);
	//////////////////

	//Main loop
	for (;;) {
		sdelay(100);
		_delay_ms(100);

		// printf("=====\n");
		switch(process)
		{
			case 0: //Random search
				// printf("Now it is random search\n");
				if(moving == true)
				{
					// stop_sign = ifStop();
						//under dark
						if(inDark == true)
						{
							if(haveDarkness() == false)
							{
								//out dark
								// if(darkLength == 1)
								// {
									// addD();
								// }
								inDark = false;
							}
						}
						else
						{
							if(haveDarkness() == true)
							{
								//under dark
								addD();
								inDark = true;
								stop();
								moving = false;
								// stop();
								// process = 2;
							}
						}
						
						if(haveObstacle() == true)
						{
							//out dark and there is obstacle
							stop();
							addO();
							moving = false;
						}
						
				}
				else
				{
					//random search
					GoToUnknow();
					moving = true;
				}
				// Movement(6);
				// stop();
				break;
			case 1: //Go to Dark places
				if(moving == true)
				{
					if(ifReach() == true)
					{
						stop();
						GoToNext();
					}
					if(haveObstacle() == true)
					{
						//obstacle
						stop();
						addO();
						if(inlocalSearch == false)
						{
							// on the way
							GoToDark();
						}
						else
						{
							//local search
							robotSteps = localSearch();
						}
					}
					if(haveDarkness() == true)
					{
						location();
						if(abs(endPointX-locationX) < 100 && abs(endPointY-locationY) < 100)
						{
							moving = false;
							process = 2;
						}
						else
						{
							//ignore it
						}
					}
				}
				else
				{
					if(inlocalSearch == false)
					{
						GoToDark();
					}
					else
					{
						StartlocalSearch();
					}
					moving = true;
				}
				break;
			case 2: //In dark, stopping
				stop();
				if(haveDarkness() == false)
				{
					//dark is removed
					homeNum--;
					if(homeNum == 1)
					{
						//go to back up
						process = 1;
					}
					else if(homeNum == 0)
					{
						//start random search again
						process = 0;
						homeNum = 2;
						darkLength = 0;
						free(darkRoot);
						free(darkEnd);
						darkRoot = (darkPlace *)malloc(sizeof(darkPlace));
						darkEnd = (darkPlace *)malloc(sizeof(darkPlace));
						darkEnd = darkRoot;
					}
					
				}
				break;
			// case 3: //localsearch()
			// 	break;
		}

		// break; //This should be deleted!!!!!!!!!!!!
	}

	return -1;
}