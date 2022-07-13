// polymorphism class
// entity includes players, enemies, objects,etc
// All entities should have 
// 	* health
// 	* movement capabilities (or lack thereof)
// 	* name
//  * ability to attack (or lack thereof)

#include <string>

class entity
{
public:
	entity();


public:
	std::string getName();
	int getSpeed();
	int getHp();
	int getDmg();

private:
	int hp;
	int speed; // can be set to 0 if necessary
	std::string name; 
	int dmg; // can be set to 0 if necessary
};
