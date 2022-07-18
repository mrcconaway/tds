#include "olcPixelGameEngine.h"

#include <vector>

class game : public olc::PixelGameEngine
{

public:
	game();
public:
	void DrawWireFrameModel(const std::vector<std::pair<float,float>> &vecModelCoordinates,
						    float x, float y, float r = 0.0f, float s = 1.0f, 
						    const olc::Pixel= olc::WHITE);
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
		float angle;
		float vel;
		int hp;
		int dmg;
	};

	sEntity player;
	std::vector<std::pair<float,float>> vecModelPlayer;




	float defvel = 50.0f;
	int defhp = 100;
	int defdmg = 25;
};
