#include "olcPixelGameEngine.h"
#include "game.h"

#include <vector>

game::game(){
	sAppName = "Top Down Shoot";
}


bool game::OnUserCreate()
{
	SWR = ScreenWidth() / 256.0f; // Screen Width Ratio
	SHR = ScreenHeight() / 240.0f; // Screen Height Ratio
	defvelX = defvelX * SWR;
	defvelY = defvelX * SHR;


	player = {float( ScreenHeight()/2),               // spawn location x
			  float(ScreenWidth() - ScreenWidth()/4), // spawn location y
			  defvelX,                           	  // velocity x
			  defvelY,                          	  // velocity y
			  0, 									  // angle
			  defhp,                                  // hp
			  defdmg};                                // dmg 


	vecModelPlayer = {{ -5.0f , +6.0f },
					  { +5.0f , +6.0f },
					  { +5.0f , -6.0f },
					  { -5.0f , -6.0f }
					 };


	return true;
}

bool game::OnUserUpdate(float fElapsedTime)
{
    draw();
	drawPlayer();
	drawCrosshair();
	getPlayerInput();

	drawBullets(); 
	if(vecBullets.size() > 0) removeBullets();

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
		vecBullets.push_back({player.x, player.y, defvelX * distX / distance, defvelY * distY/distance, 0, 100, 50});
	}
}

void game::removeBullets()
{
	auto i = remove_if(vecBullets.begin(), vecBullets.end(), [&](sEntity o){return ( o.hp <=0  || o.x < 1 || o.y < 1 || o.x > ScreenWidth() || o.y > ScreenHeight()) ; });
	if( i != vecBullets.end()){
		vecBullets.erase(i);
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
		b.x += b.dx * GetElapsedTime() * SWR;
		b.y += b.dy * GetElapsedTime() * SHR;
		DrawCircle(b.x, b.y, 0.5 *SWR );
	}
}
