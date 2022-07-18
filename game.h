#include "olcPixelGameEngine.h"

#include <vector>

class game : public olc::PixelGameEngine
{

public:
	game();
public:
	void draw();
	void drawPlayer();
	void drawCrosshair();

	void getPlayerInput();


	bool OnUserCreate()override;
	bool OnUserUpdate(float fElapsedTime) override;

private:
	struct sEntity
	{
		float x; 
		float y;
		float vel;
		int hp;
		int dmg;
	};

	sEntity player;




	float defvel = 50.0f;
	int defhp = 100;
	int defdmg = 25;
};
