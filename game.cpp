#include "olcPixelGameEngine.h"
#include "game.h"


game::game(){
	sAppName = "Top Down Shoot";
}


bool game::OnUserCreate()
{
	player = {float( ScreenHeight()/2),               // spawn location x
			  float(ScreenWidth() - ScreenWidth()/4), // spawn location y
			  defvel,                                 // velocity
			  defhp,                                  // hp
			  defdmg};                                // dmg 


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
	for(int x = player.x - 5; x < player.x+5; ++x){
		for(int y = player.y - 5; y < player.y+5; ++y){
      		PixelGameEngine::Draw(x, y, olc::Pixel(0,0,0));
		}
	}
}