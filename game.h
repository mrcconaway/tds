#include "olcPixelGameEngine.h"


class game : public olc::PixelGameEngine
{

public:
	game();
public:
	void draw();
	void drawCrosshair(int x , int y);
	

	bool OnUserCreate()override;
	bool OnUserUpdate(float fElapsedTime) override;




};
