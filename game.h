#include "olcPixelGameEngine.h"
#include "scoreSystem.h"

#include <vector>
#include <chrono>

class game : public olc::PixelGameEngine
{

public:
	game();

private:
	struct sEntity
	{
		float x; 
		float y;
		float dx;
		float dy;
		float angle;
		int hp;
		int totalhp;
		int dmg;
	};
	enum STATE
	{
		MENU = 0,
		PLAY = 1,
		PAUSE = 2,
		END = 3
	};
public:
	void drawWireFrameModel(const std::vector<std::pair<float,float>> &vecModelCoordinates,
						    float x, float y, float r = 0.0f, float sx = 1.0f, float sy = 1.0f,
						    const olc::Pixel p = olc::WHITE);
	void draw();
	void drawPlayer();
	void drawCrosshair();
	void drawBullets();
	void drawEnemies();


	void getPlayerInput();

	bool hitDetection(const sEntity o);
	bool playerHitDetection();

	void removeBullets();
	void removeEnemies();
	void spawnEnemies();

	bool OnUserCreate()override;
	bool OnUserUpdate(float fElapsedTime) override;
	void play();
	void end();
	void reset();

	void gameOverMenu();

	int getInvulnTime()const;

private:
	STATE state;
	sEntity player;
	std::vector<std::pair<float,float>> vecModelPlayer;
	bool invuln;
    std::chrono::high_resolution_clock::time_point invulnTimer; 

	
	std::vector<sEntity> vecBullets; // idea: use hp of a bullet for 'penetration' of enemies. 
									 // Each enemy takes off 1 hp point(s) (bullets defualt to 1 hp)

	sEntity enemy1;
	std::vector<sEntity> vecEnemy1;
	std::vector<std::pair<float,float>> vecModelEnemy1;


	float defvelX = 50.0f;
	float defvelY = 50.0f;
	int defhp = 100;
	int defdmg = 25;

	float SWR; // Screen Width Ratio
	float SHR; // Screen Height Ratio

	bool gameOver;
	bool onUserUpdateFlag;

	scoreSystem score;
};
