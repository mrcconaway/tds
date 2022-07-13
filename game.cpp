#include "olcPixelGameEngine.h"
#include "game.h"


game::game(){
	sAppName = "Top Down Shoot";
}


bool game::OnUserCreate()
{
	
	return true;
}

bool game::OnUserUpdate(float fElapsedTime)
{
    draw();
	return true;
}

void game::drawCrosshair(int x, int y)
{
	if(GetMouseX() == x)
       	PixelGameEngine::Draw(x, y, olc::Pixel(255,255,255));
	else if(GetMouseY() == y)
       	PixelGameEngine::Draw(x, y, olc::Pixel(255,255,255));
	else
      	PixelGameEngine::Draw(x, y, olc::Pixel(0,0,0));
}

void game::draw()
{
    for(int x = 0; x < ScreenWidth(); ++x){
        for(int y = 0; y < ScreenHeight(); ++y){
			drawCrosshair(x,y);	
        }
    }


}
