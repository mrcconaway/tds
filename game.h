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
		float defvel;
	};

	sEntity player;

};
