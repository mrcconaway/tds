#include "entity.h"


#include <string>

// constructor
entity::entity(std::string n, int s, int h, int d)
{
	name = n;
	speed = s;
	hp = h;
	dmg = d;
}


// accessors
std::string entity::getName()
{
	return name;
}
int entity::getSpeed()
{
	return speed;
}
int entity::getHp()
{
	return hp;
}
int entity::getDmg()
{
	return dmg;
}

