#include "olcPixelGameEngine.h"
#include "game.h"

#include <vector>

game::game(){
	sAppName = "Top Down Shoot";
}


bool game::OnUserCreate()
{
	player = {float( ScreenHeight()/2),               // spawn location x
			  float(ScreenWidth() - ScreenWidth()/4), // spawn location y
			  45, 									  // angle
			  defvel,                                 // velocity
			  defhp,                                  // hp
			  defdmg};                                // dmg 

	vecModelPlayer = {{+5.0f,  -5.0f},
					  {-5.0f,  -5.0f},
					//   {player.x + 5, player.y + 5},
					//   {player.x - 5, player.y + 5}
					 };


	return true;
}

bool game::OnUserUpdate(float fElapsedTime)
{
    draw();
	drawPlayer();
	drawCrosshair();
	getPlayerInput();

	return true;
}

void game::getPlayerInput()
{
	if(GetKey(olc::W).bHeld){
		if(player.y > 0)
			player.y -= player.vel*GetElapsedTime();

	}
	if(GetKey(olc::S).bHeld){
		if(player.y < ScreenHeight())
			player.y += player.vel*GetElapsedTime();
	}
	if(GetKey(olc::A).bHeld){
		if(player.x > 0)
			player.x -= player.vel*GetElapsedTime();
	}
	if(GetKey(olc::D).bHeld){
		if(player.x < ScreenWidth())
			player.x += player.vel*GetElapsedTime();
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
	for(int x = (GetMouseX()) - 5; x < int(GetMouseX())+5; ++x){
    	PixelGameEngine::Draw(x, int(GetMouseY()), olc::Pixel(0,0,0));
	}
	for(int y = int(GetMouseY()) - 5; y < int(GetMouseY())+5; ++y){
    	PixelGameEngine::Draw(GetMouseX(), y, olc::Pixel(0,0,0));
	}
}

void game::drawPlayer()
{
	// for(int x = player.x - 5; x < player.x+5; ++x){
	// 	for(int y = player.y - 5; y < player.y+5; ++y){
    //   		PixelGameEngine::Draw(x, y, olc::Pixel(0,0,0));
	// 	}
	// }
	DrawWireFrameModel(vecModelPlayer, player.x, player.y, player.angle);
}

void game::DrawWireFrameModel(const std::vector<std::pair<float,float>> &vecModelCoordinates, float x, float y, float r , float s, const olc::Pixel)
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
		vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
		vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
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
				vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second);
	}

}