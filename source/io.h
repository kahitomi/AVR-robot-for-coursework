#include "bool.h"
#include "sensor.h"

bool haveObstacle(){
	int _dis = get_distance();
	if(_dis > 300)
	{
		return true;
	}
	else
	{
		return false;
	}
};

bool haveDarkness(){
	int _light = v_load();
	if(_light >= 600)
	{
		return true;
	}
	else
	{
		return false;
	}
};

float getDistance(){
	int _digital = get_distance();
	float _dis;
	if(_digital > 217)
	{
		if(_digital < 1023)
		{
			_dis = -0.1205*(float)_digital + 56;
		}
		else
		{
			_dis = 5;
		}
	}
	else
	{
		_dis = 10000;
	}
	return _dis;
};