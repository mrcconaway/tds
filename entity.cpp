#include "entity.h"


#include <string>

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

