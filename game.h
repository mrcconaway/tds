#include "olcPixelGameEngine.h"


class game : public olc::PixelGameEngine
{

public:
	game();

  void draw();
	

	bool OnUserCreate()override;
	bool OnUserUpdate(float fElapsedTime) override;




};
