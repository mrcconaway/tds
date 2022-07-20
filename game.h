#include "olcPixelGameEngine.h"

#include <vector>

class game : public olc::PixelGameEngine
{

public:
	game();
public:
	void drawWireFrameModel(const std::vector<std::pair<float,float>> &vecModelCoordinates,
						    float x, float y, float r = 0.0f, float sx = 1.0f, float sy = 1.0f,
						    const olc::Pixel p = olc::WHITE);
	void draw();
	void drawPlayer();
	void drawCrosshair();
	void drawBullets();

	void getPlayerInput();

	void removeBullets();

	bool OnUserCreate()override;
	bool OnUserUpdate(float fElapsedTime) override;

private:
	struct sEntity
	{
		float x; 
		float y;
		float dx;
		float dy;
		float angle;
		int hp;
		int dmg;
	};

	sEntity player;
	std::vector<std::pair<float,float>> vecModelPlayer;
	
	std::vector<sEntity> vecBullets; // idea: use hp of a bullet for 'penetration' of enemies. 
									 // Each enemy takes off their respective hp point(s) (bullets defualt to 100 hp)



	float defvelX = 50.0f;
	float defvelY = 50.0f;
	int defhp = 100;
	int defdmg = 25;

	float SWR; // Screen Width Ratio
	float SHR; // Screen Height Ratio
};
