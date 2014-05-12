#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "bool.h"
#include "map.h"
#include "motor.h"

#define MAX 9999999

float radius = 30;//=====================This is an important number
float localRadius = 100; //local search radius

float angle = 90;

float startPointX = 0.00;
float startPointY = 0.00;

float endPointX = 0.00;
float endPointY = 0.00;

float locationX = 0.00;
float locationY = 0.00;

int localP = 0;

double timeStart = 0;
double timeNow = 0;
double timePrefer = 0;

float localMap[8][2];

typedef struct step{
	struct step *next;
	struct step *parent;
	float he[8];
	float X;
	float Y;
	int stepNum;
	short father;
}step;

//add one step;
step *addStep(step *end, float x, float y, int number, short father, float heuristic[8]){
	step *p;
	p = (step *)malloc(sizeof(step));
	p->X = x;
	p->Y = y;
	p->he[0] = heuristic[0];
	p->he[1] = heuristic[1];
	p->he[2] = heuristic[2];
	p->he[3] = heuristic[3];
	p->he[4] = heuristic[4];
	p->he[5] = heuristic[5];
	p->he[6] = heuristic[6];
	p->he[7] = heuristic[7];
	p->stepNum = number;
	p->father = father;
	p->parent = end;
	p->next = NULL;
	end->next = p;
	end = p;
	return end;
};

//Move Robot
void move(float _distance, float _angle){
	float _turn = _angle - angle;

	timeStart = clock();

	if(_turn == 0)
	{
		//do nothing
	}
	else if(_turn == 45 || _turn == -315)
	{
		Movement(3);
	}
	else if(_turn == 90 || _turn == -270)
	{
		Movement(4);
	}
	else if(_turn == 135 || _turn == -225)
	{
		Movement(5);
	}
	else if(_turn == 180 || _turn == -180)
	{
		Movement(6);
	}
	else if(_turn == 225 || _turn == -135)
	{
		Movement(7);
	}
	else if(_turn == 270 || _turn == -90)
	{
		Movement(8);
	}
	else if(_turn == 315 || _turn == -45)
	{
		Movement(9);
	}
	_angle = angle;
	//movement
	//if(_distance == -1)
};

void stop(){
	Movement(0);
};

//location Robot
void location(){};

//if the robot reach the purpose place
bool ifReach(){
	location();
	timeNow = clock();
	if( timeNow - timeStart >= timePrefer)
	{
		if(abs(locationX-endPointX) < 5 && abs(locationY-endPointY) < 5)
		{
			return true;
		}
	}
	return false;
};

//will be used in AStar
bool ifBlock(float xo, float yo, float x1, float y1, float x2, float y2){
	float distance;
	float k = (y1-y2)/(x1-x2);

	if(isinf(k))
	{
		distance = abs( x1 - xo );
	}
	else
	{
		distance = abs( k*xo - yo + y1 - k*x1 ) / sqrt(pow(k, 2) + 1);
	}

	if(distance > radius)
	{
		return false;
	}
	else
	{
		float d1 = sqrt( pow((x1-xo), 2) + pow((y1-yo), 2) );
		float d2 = sqrt( pow((x2-xo), 2) + pow((y2-yo), 2) );
		float d = sqrt( pow((x2-x1), 2) + pow((y2-y1), 2) );
		if(d1 <= radius || d2 <= radius)
		{
			return true;
		}
		else if(d > d1 && d > d2)
		{
			return true;
		}
		return false;
	}
};

//will be used in AStar
bool ifObstacle(float xo, float yo){
	obstacle *p = mapRoot->next;
	short i = mapLength;

	for(i; i>0; i--)
	{
		if(ifBlock(xo, yo, p->leftX, p->leftY, p->rightX, p->rightY) == true)
		{
			return true;
		}
		p = p->next;
	}

	return false;
};

//show path
void showSteps(step *root){
	step *p = root;
	while(p->next != NULL)
	{
		printf("%i--(%f, %f)\n",p->stepNum, p->X, p->Y);
		// printf("    -0.%f\n",p->he[0]);
		// printf("    -1.%f\n",p->he[1]);
		// printf("    -2.%f\n",p->he[2]);
		// printf("    -3.%f\n",p->he[3]);
		// printf("    -4.%f\n",p->he[4]);
		// printf("    -5.%f\n",p->he[5]);
		// printf("    -6.%f\n",p->he[6]);
		// printf("    -7.%f\n",p->he[7]);
		p = p->next;
	};
	printf("%i--(%f, %f)\n",p->stepNum, p->X, p->Y);
	// printf("    -0.%f\n",p->he[0]);
	// printf("    -1.%f\n",p->he[1]);
	// printf("    -2.%f\n",p->he[2]);
	// printf("    -3.%f\n",p->he[3]);
	// printf("    -4.%f\n",p->he[4]);
	// printf("    -5.%f\n",p->he[5]);
	// printf("    -6.%f\n",p->he[6]);
	// printf("    -7.%f\n",p->he[7]);
};

//A star path search, return steps
step *AStar(float x1, float y1, float x2, float y2){
	step *stepRoot = (step *)malloc(sizeof(step));
	step *stepEnd = (step *)malloc(sizeof(step));
	step *hisRoot = (step *)malloc(sizeof(step));
	step *hisEnd = (step *)malloc(sizeof(step));
	step *hisUpdate = (step *)malloc(sizeof(step));
	step *p = stepRoot;

	float fake_he[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
	int stepLength = 1;
	int loopNum = 0;
	float x, y, choice, cx, cy;
	short i, c, cf, f, from;
	bool isOld, isInhistory;

	stepRoot->X = x1;
	stepRoot->Y = y1;
	stepRoot->stepNum = stepLength;
	stepRoot->father = -1;
	stepEnd = stepRoot;

	hisRoot->X = x1;
	hisRoot->Y = y1;
	hisRoot->stepNum = 0;
	hisEnd = hisRoot;

	while(1)
	{
		loopNum++;
		if(loopNum > 10000)
		{
			break;
		}

		isOld = false;
		isInhistory = false;
		from = stepEnd->father;
		//if the point is passed
		p = stepRoot;
		while(p->next != NULL)
		{
			if(p->X == stepEnd->X)
			{
				if(p->Y == stepEnd->Y)
				{
					// printf("@ Old position!\n");
					stepEnd = p;
					isOld = true;
					isInhistory = true;
					break;
				}
			}
			p = p->next;
		};
		//check the history
		p = hisRoot;
		while(p->next != NULL)
		{
			if(p->X == stepEnd->X)
			{
				if(p->Y == stepEnd->Y)
				{
					// printf("@ History position!\n");
					hisUpdate = p;
					if(isOld == false)
					{
						stepEnd->he[0] = hisUpdate->he[0];
						stepEnd->he[1] = hisUpdate->he[1];
						stepEnd->he[2] = hisUpdate->he[2];
						stepEnd->he[3] = hisUpdate->he[3];
						stepEnd->he[4] = hisUpdate->he[4];
						stepEnd->he[5] = hisUpdate->he[5];
						stepEnd->he[6] = hisUpdate->he[6];
						stepEnd->he[7] = hisUpdate->he[7];
					}
					// printf("    -0.%f\n",hisUpdate->he[0]);
					// printf("    -1.%f\n",hisUpdate->he[1]);
					// printf("    -2.%f\n",hisUpdate->he[2]);
					// printf("    -3.%f\n",hisUpdate->he[3]);
					// printf("    -4.%f\n",hisUpdate->he[4]);
					// printf("    -5.%f\n",hisUpdate->he[5]);
					// printf("    -6.%f\n",hisUpdate->he[6]);
					// printf("    -7.%f\n",hisUpdate->he[7]);
					isInhistory = true;
					break;
				}
			}
			p = p->next;
		};
		//create heuristic score for neighbors
		stepLength = stepEnd->stepNum;


		choice = MAX;
		c = -1;
		cx = 0;
		cy = 0;
		cf = -1;
		for(i = 7; i>=0; i--)
		{
			
			switch(i)
			{
				case 0:
					x = stepEnd->X + radius;
					y = stepEnd->Y + radius;
					f = 4;
					break;
				case 7:
					x = stepEnd->X + radius;
					y = stepEnd->Y - radius;
					f = 5;
					break;
				case 6:
					x = stepEnd->X + radius;
					y = stepEnd->Y;
					f = 3;
					break;
				case 5:
					x = stepEnd->X - radius;
					y = stepEnd->Y + radius;
					f = 7;
					break;
				case 4:
					x = stepEnd->X - radius;
					y = stepEnd->Y - radius;
					f = 0;
					break;
				case 3:
					x = stepEnd->X - radius;
					y = stepEnd->Y;
					f = 6;
					break;
				case 2:
					x = stepEnd->X;
					y = stepEnd->Y + radius;
					f = 1;
					break;
				case 1:
					x = stepEnd->X;
					y = stepEnd->Y - radius;
					f = 2;
					break;
			}

			//new position, calculate heuristic
			if(isInhistory == false)
			{
				if(from == i)
				{
					stepEnd->he[i] = MAX;
				}
				else if(ifObstacle(x, y) == true)
				{
					stepEnd->he[i] = MAX;
				}
				else
				{
					//heuristic function
					// stepEnd->he[i] = sqrt( pow((x2-x), 2)+pow((y2-y), 2) );
					stepEnd->he[i] = stepLength * radius + sqrt( pow((x2-x), 2)+pow((y2-y), 2) );
				}
			}
			else
			{
				// History update
				if(from == i)
				{
					stepEnd->he[i] = MAX;
					hisUpdate->he[i] = MAX;
				}
			}

			// printf("%i. %f\n", i, stepEnd->he[i]);

			//select the min heuristic direction
			if(stepEnd->he[i] < choice)
			{
				choice = stepEnd->he[i];
				c = i;
				cx = x;
				cy = y;
				cf = f;
			}
		}

		//no choice, back to the father
		if(choice == MAX)
		{
			// printf("Back!\n");
			stepEnd = stepEnd->parent;
			continue;
		}
		
		// printf("Choice is %i\n", c);
		//choose one derection,and continue
		if(c != -1)
		{
			stepEnd->he[c] = MAX;
			if(isInhistory == true)
			{
				//upDate history
				// printf("Update History\n");
				hisUpdate->he[c] = MAX;
			}
			else
			{
				//add new history (this point)
				// printf("Add to history\n");
				hisEnd = addStep(hisEnd, stepEnd->X, stepEnd->Y, 0, 0, stepEnd->he);
			}
			stepLength++;
			// printf("Choice:%i: %i--(%f, %f)\n", c, stepLength, cx, cy);
			stepEnd = addStep(stepEnd, cx, cy, stepLength, cf, fake_he);
			if(sqrt( pow((x2-cx), 2)+pow((y2-cy), 2) ) < radius)
			{
				// printf("Last step\n");
				stepLength++;
				stepEnd = addStep(stepEnd, x2, y2, stepLength, 0, fake_he);
				break;
			}
		}
	}

	// showSteps(hisRoot);
	// printf("loop: %i\n", loopNum);
	free(stepEnd);
	return stepRoot;
};

void StartlocalSearch(){
	location();
	localMap[0][0] = locationX + localRadius;
	localMap[0][1] = locationY + localRadius;
	localMap[1][0] = locationX + localRadius;
	localMap[1][1] = locationY - localRadius;
	localMap[2][0] = locationX + localRadius;
	localMap[2][1] = locationY;
	localMap[3][0] = locationX;
	localMap[3][1] = locationY + localRadius;
	localMap[4][0] = locationX;
	localMap[4][1] = locationY - localRadius;
	localMap[5][0] = locationX - localRadius;
	localMap[5][1] = locationY;
	localMap[6][0] = locationX - localRadius;
	localMap[6][1] = locationY - localRadius;
	localMap[7][0] = locationX - localRadius;
	localMap[7][1] = locationY + localRadius;
	localP = 0;
};

step *localSearch(){
	location();
	localP++;
	return *AStar(locationX, locationY, localMap[localP-1][0], localMap[localP-1][1]);
};