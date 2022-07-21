#include "olcPixelGameEngine.h"
#include "game.h"

#include <vector>
#include <random> 
#include <time.h>

game::game(){
	sAppName = "Top Down Shoot";
}


bool game::OnUserCreate()
{
	/* initialize random seed: */
    srand(time(NULL));

	SWR = ScreenWidth() / 256.0f; // Screen Width Ratio
	SHR = ScreenHeight() / 240.0f; // Screen Height Ratio
	defvelX = defvelX * SWR;
	defvelY = defvelX * SHR;


	player = {float( ScreenWidth()/2),                  // spawn location x
			  float(ScreenHeight() - ScreenHeight()/4), // spawn location y
			  defvelX,                           	    // velocity x
			  defvelY,                          	    // velocity y
			  0, 									    // angle
			  defhp,                                    // hp
			  defdmg};                                  // dmg 


	vecModelPlayer = {{ -5.0f , +6.0f },
					  { +5.0f , +6.0f },
					  { +5.0f , -6.0f },
					  { -5.0f , -6.0f }
					 };

	enemy1 = {float(rand() % ScreenWidth() + 20), 
			  float(rand() % ScreenHeight() + 1), 
			  defvelX/2,
			  defvelY/2,
			  0,
			  defhp,
			  defdmg };
	vecEnemy1.push_back(enemy1);


	vecModelEnemy1 = {{ -3.0f , +7.0f }, // bottom left
					  { +3.0f , +7.0f }, // bottom right
					  { +3.0f , -4.0f }, // top right
					  { -3.0f , -4.0f } // top left
					 };
	return true;
}

bool game::OnUserUpdate(float fElapsedTime)
{
    draw();
	drawPlayer();
	drawCrosshair();
	getPlayerInput();

	drawEnemies();
	drawBullets(); 
	if(vecBullets.size() > 0) removeBullets();
	if(vecEnemy1.size() > 0) removeEnemies();

	return true;
}

void game::getPlayerInput()
{
	if(GetKey(olc::W).bHeld){
		if(player.y > 0)
			player.y -= player.dy *GetElapsedTime();

	}
	if(GetKey(olc::S).bHeld){
		if(player.y < ScreenHeight())
			player.y += player.dy*GetElapsedTime();
	}
	if(GetKey(olc::A).bHeld){
		if(player.x > 0)
			player.x -= player.dx*GetElapsedTime() * SWR;
	}
	if(GetKey(olc::D).bHeld){
		if(player.x < ScreenWidth())
			player.x += player.dx*GetElapsedTime() * SWR;
	}
	if(GetMouse(0).bPressed){
		float distX = GetMouseX() - player.x;
		float distY = GetMouseY() - player.y;
		float distance = sqrtf(distX * distX + distY * distY );
		vecBullets.push_back({player.x, player.y, 1.25f*defvelX * distX / distance, 1.25f*defvelY * distY/distance, 0, 1, 50});
	}
}

bool game::hitDetection(const sEntity o)
{
	int oX = int(o.x);
	int oY = int(o.y);
	for(auto &e : vecEnemy1){
		if(
			oX <= int(e.x + vecModelEnemy1[1].first) &&
			oX >= int(e.x + vecModelEnemy1[2].first ) &&
			oY <= int(e.y + vecModelEnemy1[1].second) &&
			oY >= int(e.y + vecModelEnemy1[3].second) 
		   ){ e.hp -= o.dmg; return true; }
	}

	return false;

}

void game::removeBullets()
{
	auto i = remove_if(vecBullets.begin(), vecBullets.end(), [&](sEntity o){return ( o.hp <=0  || o.x < 1 || o.y < 1 || o.x > ScreenWidth() || o.y > ScreenHeight()) ; });
	if( i != vecBullets.end()){
		vecBullets.erase(i);
	}
}

void game::removeEnemies(){
	auto i = remove_if(vecEnemy1.begin(), vecEnemy1.end(), [&](sEntity o){return o.hp <=0; });
	if( i != vecEnemy1.end()){
		vecEnemy1.erase(i);
	}
}


void game::draw()
{
    for(int x = 0; x < ScreenWidth(); ++x){
        for(int y = 0; y < ScreenHeight(); ++y){
	      	PixelGameEngine::Draw(x, y, olc::Pixel(180,255,150));
        }
    }
}

void game::drawCrosshair()
{
	DrawCircle(GetMouseX(), GetMouseY(), 3 * SWR, olc::Pixel(0,0,0));
}

void game::drawPlayer()
{
	drawWireFrameModel(vecModelPlayer, player.x, player.y, player.angle, SWR, SHR);
}

void game::drawWireFrameModel(const std::vector<std::pair<float,float>> &vecModelCoordinates, float x, float y, float r , float sx, float sy, const olc::Pixel p)
{
	// pair.first = x coordinate
	// pari.second = y coordinate

 	// Create translated model vector of coordinate pairs
	std::vector<std::pair<float,float>> vecTransformedCoordinates;
	int verts = vecModelCoordinates.size();
	vecTransformedCoordinates.resize(verts);

	// rotate
	for(int i = 0 ; i < verts; i++){
		vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
		vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
	}
	// Scale
	for(int i = 0 ; i < verts; i++){
		vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * sx;
		vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * sy;
	}
	// Translate
	for(int i =0 ; i < verts; i++){
		vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first+x;
		vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second+y;
	}
	// Draw Closed Polygon
	for(int i = 0; i < verts; i++){
		int j = (i+1);
		DrawLine(vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second, 
				vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second, p);
	}
}

void game::drawBullets()
{
	for(auto &b : vecBullets){
		b.x += b.dx * GetElapsedTime();
		b.y += b.dy * GetElapsedTime();
		DrawCircle(b.x, b.y, 0.5 *SWR );
		if(hitDetection(b)){
			b.hp -= 1;
		}
	}
}

void game::drawEnemies()
{
	for(auto &e : vecEnemy1){
		float distX = player.x - e.x;
		float distY = player.y - e.y;
		float distance = sqrtf(distX * distX + distY * distY );
		// vecBullets.push_back({player.x, player.y, 1.25f*defvelX * distX / distance, 1.25f*defvelY * distY/distance, 0, 100, 50});
		
		e.x += e.dx * distX / distance * GetElapsedTime();
		e.y += e.dy * distY/distance * GetElapsedTime();
		drawWireFrameModel(vecModelEnemy1, e.x, e.y, e.angle, SWR, SHR, olc::Pixel(0,0,255));
	}
}
