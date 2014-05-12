// #include <avr/io.h>!!!!!!!!!!!!!!!!!!!!!!!!!
// #include <util/delay.h>!!!!!!!!!!!!!!!!!!!!!!!!!
#include <stdio.h>

//user source
#include "source/io.h"
#include "source/move.h"


bool moving = false; //If robot is moving
bool inDark = false; //If robot is under dark
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

void GoToUnknow(){ //Random move
	double _random = rand()%100;
	int direction = (int)ceil(_random*8/100);
	move(-1, direction);
}; 

void GoToDark(){}; //calculate the nearest dark place the the path to it. Then move

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

	//init
	initMap();

	////////////////
	//test code
	////////////////
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
		// _delay_ms(200);!!!!!!!!!!!!!!!!!!!!!!

		printf("=====\n");
		switch(process)
		{
			case 0: //Random search
				printf("Now it is random search\n");
				if(moving == true)
				{
					stop_sign = ifStop();
					if(inDark == true)
					{
						//under dark
						if(stop_sign == 0)
						{
							//out dark
							addD();
							inDark = false;
						}
						if(stop_sign == 1)
						{
							//out dark and there is obstacle
							stop();
							addO();
							moving = false;
							addD();
							inDark = false;
						}
						if(stop_sign == 3)
						{
							//stop because of obstacle, under dark
							stop();
							addO();
							moving = false;
						}
					}
					else
					{
						//under light
						if(stop_sign == 1)
						{
							//stop because of obstacle
							stop();
							addO();
							moving = false;
						}
						else if(stop_sign == 2)
						{
							//under dark
							addD();
							inDark = true;
						}
						else if(stop_sign == 3)
						{
							//stop because of obstacle, under dark
							stop();
							addO();
							addD();
							inDark = true;
							moving = false;
						}
					}	
				}
				else
				{
					//random search
					GoToUnknow();
					moving = true;
				}
				break;
			case 1: //Go to Dark places
				if(moving == true)
				{
					stop_sign = ifStop();
					if(stop_sign == 1)
					{
						GoToDark();
					}
					else if(stop_sign == 2)
					{
						moving = false;
						process = 2;
					}
					if(ifReach())
					{
						moving = false;
						localSearch();
					}
				}
				else
				{
					GoToDark();
					moving = true;
				}
				break;
			case 2: //In dark, stopping
				stop_sign = ifStop();
				if(stop_sign != 2)
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
			case 3: //localsearch()
				break;
		}

		break; //This should be deleted!!!!!!!!!!!!
	}

	return -1;
}