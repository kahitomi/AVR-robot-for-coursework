#include <stdlib.h>
#include <math.h>
#include "bool.h"

//dark places
#define darkPlacesNum 2
//Max map size
#define MaxMapSize 100
//Same obstacle area
#define lineArea 20

typedef struct darkPlace{
	struct darkPlace *next;
	float X;
	float Y;
}darkPlace;
darkPlace *darkRoot, *darkEnd;
short darkLength = 0;

//obstacles in map
typedef struct obstacle{
	struct obstacle *next;
	float leftX;
	float leftY;
	float rightX;
	float rightY;
}obstacle;
obstacle *mapRoot, *mapEnd;
short mapLength = 0;

//obstacle cluster
// typedef struct disMatrix{
// 	float dis;
// 	float angle;
// }disMatrix;

//init Map
void initMap(){
	mapRoot = (obstacle *)malloc(sizeof(obstacle));
	mapEnd = (obstacle *)malloc(sizeof(obstacle));
	mapEnd = mapRoot;
	darkRoot = (darkPlace *)malloc(sizeof(darkPlace));
	darkEnd = (darkPlace *)malloc(sizeof(darkPlace));
	darkEnd = darkRoot;
};

//Show map
void showMap(){
	obstacle *p = mapRoot->next;
	short i = mapLength;
	printf("Start print\n");
	for(i; i>0; i--)
	{
		printf("Obstacle:(%f,%f),(%f,%f)\n",p->leftX,p->leftY,p->rightX,p->rightY);
		p = p->next;
	}
	free(p);
};

void showObstracle(obstacle *p){
	printf("Obstacle:(%f,%f),(%f,%f)\n",p->leftX,p->leftY,p->rightX,p->rightY);
};

//select an obstacle by cNum
obstacle *selObstracle(int oNum){
	obstacle *p = mapRoot;
	short i;
	for(i = 0; i<oNum; i++)
	{
		p = p->next;
	}
	return p;
};

//return center X
float centerX(obstacle *p){
	return (p->leftX + p->rightX)/2;
};
//return center Y
float centerY(obstacle *p){
	return (p->leftY + p->rightY)/2;
};
//compare a number with an array
bool isDeny(int deny[MaxMapSize+2], int target){
	int i = 0;
	// printf("compare %i\n", target);
	while(deny[i] != -1)
	{
		if(deny[i] == target)
		{
			return true;
		}
		i++;
	}
	return false;
}


//add one new obstacle
void addObstacle(float x1,float y1,float x2,float y2){
	obstacle *pointer;
	pointer = (obstacle *)malloc(sizeof(obstacle));
	pointer->leftX = x1;
	pointer->leftY = y1;
	pointer->rightX = x2;
	pointer->rightY = y2;
	mapEnd->next = pointer;
	mapEnd = pointer;
	mapLength++;

	if(mapLength > MaxMapSize)
	{
		//Reduce obstacles
		obstacle *pointerI = mapRoot;
		obstacle *pointerJ = mapRoot;
		float disMatrix[mapLength][mapLength];
		int cluster[mapLength], deny[mapLength+1], del[mapLength+1];
		float min, minNum, k, b, n, m, l, o, p, q, xi, yi, dis, x_1, x_2, y_1, y_2;
		int i = mapLength-1, tempMapLength = mapLength, j, cNum, cDeny, delNum, target, temp;
		delNum = 0;

		//create disMatrix
		for(i; i>=0; i--)
		{
			printf("Obstacle:(%f,%f),(%f,%f)\n",pointerI->leftX,pointerI->leftY,pointerI->rightX,pointerI->rightY);
			pointerJ = mapRoot;
			for(j = tempMapLength-1; j>=0; j--)
			{
				disMatrix[i][j] = sqrt( pow( (centerX(pointerI) - centerX(pointerJ)), 2) + pow( (centerY(pointerI) - centerY(pointerJ)), 2) );
				pointerJ = pointerJ->next;
				printf("%i\t", (int)disMatrix[i][j]);
			}
			printf("\n");
			pointerI = pointerI->next;
		}

		//cluster and reduce
		deny[0] = -1;
		cDeny = 1;
		for(i = tempMapLength-1; i>=0; i--)
		{
			if(isDeny(deny, i) == true)
			{
				continue;
			}
			printf("------%i\n", i);
			target = i;
			cluster[0] = -1;
			cNum = 1;
			deny[cDeny-1] = i;
			deny[cDeny] = -1;
			cDeny++;
			cluster[cNum-1] = i;
			cluster[cNum] = -1;
			cNum++;
			while(1)
			{
				min = 999999;
				minNum = -1;
				for(j = tempMapLength-1; j>=0; j--)
				{
					if(disMatrix[target][j] < min && target != j)
					{
						if(isDeny(deny, j) == true)
						{
							continue;
						}
						min = disMatrix[target][j];
						minNum = j;
					}
				}

				// printf("choice min: %f\n", min);
				if(minNum == -1)
				{
					break;
				}

				if(min <= lineArea)
				{
					if(cNum <= 3)
					{
						if(cNum == 3)
						{
							//calculate a center line
							k = 0;
							b = 0;
							n = 0;
							m = 0;
							l = 0;
							o = 0;
							p = 0;
							q = 0;
							for(j = 2; j>=0; j--)
							{
								pointerI = selObstracle(cluster[j]);
								xi = centerX(pointerI);
								yi = centerY(pointerI);
								n += 2*xi*xi;
								m += 2*xi;
								l += -2*yi*xi;
								o += 2*xi;
								p += 2;
								q += -2*yi;		
							}
							b = (n*q - o*l)/(o*m - n*p);
							k = (-m*b-l)/n;
						}
						target = minNum;
						cluster[cNum-1] = target;
						cluster[cNum] = -1;
						cNum++;

						deny[cDeny-1] = target;
						deny[cDeny] = -1;
						cDeny++;
					}
					else
					{
						pointerI = selObstracle(minNum);
						dis = abs(k*centerX(pointerI) - centerY(pointerI) + b)/sqrt(1+k*k);
						// printf("%i: Distance %f\n", target, dis);
						if(dis < lineArea)
						{
							// printf("Apply\n");
							target = minNum;
							cluster[cNum-1] = target;
							cluster[cNum] = -1;
							cNum++;
							deny[cDeny-1] = target;
							deny[cDeny] = -1;
							cDeny++;
						}
						break;
					}
				}
				else
				{
					break;
				}
			}
			for (int j = 0; j < cNum; j++)
			{
				printf("%i\t", cluster[j]);
			}
			printf("\n");

			if(cNum == 2)
			{
				continue;
			}
			
			//calculate a center line
			if(cNum == 3)
			{
				x_1 = centerX(selObstracle(cluster[0]));
				y_1 = centerY(selObstracle(cluster[0]));
				x_2 = centerX(selObstracle(cluster[1]));
				y_2 = centerY(selObstracle(cluster[1]));
				del[delNum] = cluster[0];
				delNum++;
				del[delNum] = cluster[1];
				delNum++;
			}
			else
			{
				k = 0;
				b = 0;
				n = 0;
				m = 0;
				l = 0;
				o = 0;
				p = 0;
				q = 0;
				for(j = cNum-1; j>0; j--)
				{
					del[delNum] = cluster[j-1];
					delNum++;
					pointerI = selObstracle(cluster[j-1]);
					xi = centerX(pointerI);
					yi = centerY(pointerI);
					// printf("point: %f, %f\n", xi, yi);
					n += 2*xi*xi;
					m += 2*xi;
					l += -2*yi*xi;
					o += 2*xi;
					p += 2;
					q += -2*yi;
					if(j == cNum-1)
					{
						x_1 = xi;
						x_2 = xi;
					}
					else
					{
						if(xi<x_1)
						{
							x_1 = xi;
						}
						if(xi>x_2)
						{
							x_2 = xi;
						}
					}
				}
				b = (n*q - o*l)/(o*m - n*p);
				k = (-m*b-l)/n;
				y_1 = x_1*k+b;
				y_2 = x_2*k+b;
			}
				
			//inset new obstacle
			printf("New obstacle insert (%f, %f, %f, %f)\n", x_1, y_1, x_2, y_2);
			obstacle *pointerTemp;
			pointerTemp = (obstacle *)malloc(sizeof(obstacle));
			pointerTemp->leftX = x_1;
			pointerTemp->leftY = y_1;
			pointerTemp->rightX = x_2;
			pointerTemp->rightY = y_2;
			mapEnd->next = pointerTemp;
			mapEnd = pointerTemp;
			mapLength++;
		}
		//del old unclustered obstacles
		printf("del %i\n",delNum);
		for(i = 0; i<delNum-1; i++)
		{
			for(j = i+1; j<delNum; j++)
			{
				if(del[j] < del[i])
				{
					temp = del[j];
					del[j] = del[i];
					del[i] = temp;
				}
			}
		}
		pointerI = mapRoot;
		j = 0;
		k = 0;
		for(i = 0; i<tempMapLength; ++i)
		{
			if(del[j] == i)
			{
				if(i == 0)
				{
					k = 1;
					pointerJ = pointerI;
					pointerI = pointerI->next;
				}
				else
				{
					pointerJ->next = pointerI->next;
					pointerI = pointerI->next;
				}
				j++;
			}
			else
			{
				pointerJ = pointerI;
				pointerI = pointerI->next;
			}	
		}
		if(k)
		{
			mapRoot = mapRoot->next;
		}
	}
};

//add one new dark place
void addDark(float x,float y){
	darkPlace *p;
	p = (darkPlace *)malloc(sizeof(darkPlace));
	p->X = x;
	p->Y = y;
	darkEnd->next = p;
	darkEnd = p;
	darkLength++;
};