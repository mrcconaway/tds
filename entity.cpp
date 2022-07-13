#include "entity.h"


#include <string>

// constructor
entity::entity(){}


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

