#include "olcPixelGameEngine.h"
#include "game.h"
#include "scoreSystem.h"

#include <vector>
#include <random> 
#include <chrono>

game::game(){
	sAppName = "Top Down Shooter";
}


bool game::OnUserCreate()
{
	/* initialize random seed: */
    srand(time(NULL));

	SWR = ScreenWidth() / 256.0f; // Screen Width Ratio
	SHR = ScreenHeight() / 240.0f; // Screen Height Ratio
	defvelX = defvelX * SWR;
	defvelY = defvelX * SHR;
	reset();

	return true;
}

bool game::OnUserUpdate(float fElapsedTime)
{
	switch(state)
	{
		default: 
			onUserUpdateFlag = false;
			break;
		case PLAY:
			play();
			break;
		case END:
			end();
			break;
	}

	return onUserUpdateFlag;
}
void game::play()
{
    draw();
	if(invuln == true && (  getInvulnTime() > 3 ) ){
		invuln = false;
	}
	drawPlayer();
	drawCrosshair();
	getPlayerInput();

	drawEnemies();
	drawBullets(); 
	if(vecBullets.size() > 0) removeBullets();
	if(vecEnemy1.size() > 0) removeEnemies();

	spawnEnemies();

	if(gameOver) state = END;
}
void game::end()
{
	// game is over, make it so enemies and player cannot move
	enemy1.dx = 0.0f;
	enemy1.dy = 0.0f;
	draw();
	drawPlayer();
	drawCrosshair();
	drawEnemies();
	drawBullets(); 
	if(vecBullets.size() > 0) removeBullets();
    DrawString(ScreenWidth()/2, ScreenHeight() - ScreenHeight()/4, "Game Over!", olc::DARK_RED );
}

void game::reset()
{
	
	onUserUpdateFlag = true;
	state = PLAY;
	gameOver = false;
	score = scoreSystem();

	invuln = false;
	player = {float( ScreenWidth()/2),                  // spawn location x
			  float(ScreenHeight() - ScreenHeight()/4), // spawn location y
			  defvelX,                           	    // velocity x
			  defvelY,                          	    // velocity y
			  0, 									    // angle
			  defhp,                                    // hp
			  100,										// total hp
			  defdmg,                                   // dmg 
			  'p',										// player id
			  defluck};									// default luck


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
			  100,
			  100,
			  defdmg,
			  'e', 
			  defluck};
	vecEntity.push_back(player);
	vecEntity.push_back(enemy1);


	vecModelEnemy1 = {{ -3.0f , +7.0f }, // bottom left
					  { +3.0f , +7.0f }, // bottom right
					  { +3.0f , -4.0f }, // top right
					  { -3.0f , -4.0f } // top left
					 };
}

void game::gameOverMenu()
{
	// implement
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
		vecBullets.push_back({player.x, player.y, 1.25f*defvelX * distX / distance, 1.25f*defvelY * distY/distance, 0, 1,1, 50, 'p',0});
	}
}

bool game::hitDetection(const sEntity o, const sEntity entity)
{
	int oX = int(o.x);
	int oY = int(o.y);
	for(auto &e : entity){
		int oX = (o.x);
		int oY = int(o.y);
		if(oX >= int(e.x) + int(entity[0].first)  && // left side
		   oX <= int(e.x) + int(entity[1].first)  && // right side
		   oY <= int(e.y) + int(entity[0].second) && // top side
		   oY >= int(e.y) + int(entity[2].second)    // bot side
		  ){ e.hp -= o.dmg;  return true;}
	}
	return false;
}

bool game::playerHitDetection()
{
		int pX = int(player.x);
		int pY = int(player.y);
		for(auto &e : vecEnemy1){
			if(pX + int(vecModelPlayer[1].first) >= int(e.x) + int(vecModelEnemy1[0].first) &&   // p right edge past left e
			   pX + int(vecModelPlayer[0].first) <= int(e.x) + int(vecModelEnemy1[1].first) &&   // p left edge past right e
			   pY + int(vecModelPlayer[0].second) >= int(e.y) + int(vecModelEnemy1[2].second) && // p bot edge past top e
			   pY + int(vecModelPlayer[3].second) <= int(e.y) + int(vecModelEnemy1[0].second)    // p top edge past bot e
			){ 
				if(!invuln){ player.hp -= e.dmg; invuln = true; invulnTimer = std::chrono::high_resolution_clock::now(); };
				return true; 
			}
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
	auto i = remove_if(vecEntity.begin(), vecEntity.end(), [&](sEntity o){return o.hp <=0; });
	if( i != vecEntity.end()){
		score.incScore( i->totalhp/2);
		vecEntity.erase(i);
	}
}

void game::spawnEnemies()
{
	int spawn = rand() % 100000 ;
	if(spawn < 11){
		do{
			enemy1.x = float(rand() % ScreenWidth() );
			enemy1.y = float(rand() % ScreenHeight() );
		}while( 
			( (enemy1.x - player.x)*(enemy1.x - player.x) + (enemy1.y - player.y)*(enemy1.y - player.y) ) <= 1000
		 );
		vecEntity.push_back(enemy1);
	}
}


void game::draw()
{
    for(int x = 0; x < ScreenWidth(); ++x){
        for(int y = 0; y < ScreenHeight(); ++y){
	      	PixelGameEngine::Draw(x, y, olc::Pixel(180,255,150));
        }
    }
	if(!invuln)
		DrawString(ScreenWidth()-ScreenWidth()/6, ScreenHeight()/10 - 2, std::to_string(score.getScore()), olc::Pixel(225,0,0) ); // draw score
	else
		DrawString(ScreenWidth()-ScreenWidth()/6, ScreenHeight()/10 - 2, std::to_string(score.getScore()), olc::Pixel(224,213,126) ); // draw score
}

void game::drawCrosshair()
{
	DrawCircle(GetMouseX(), GetMouseY(), 3 * SWR, olc::Pixel(0,0,0));
}

void game::drawPlayer()
{
	drawWireFrameModel(vecModelPlayer, player.x, player.y, player.angle, SWR, SHR);
	olc::Pixel p;
	if(invuln){
		p = olc::Pixel(224,213,126);
	}
	else{
		p = olc::Pixel(255,0,0);
	}
	if(player.hp > 0){
		DrawLine( ScreenWidth()/10, ScreenHeight()/10, ScreenWidth()/10 + ScreenWidth()/8,ScreenHeight()/10, p);
	}
	if(player.hp > 25){
		DrawLine( ScreenWidth()/10 + ScreenWidth()/8 + 2, ScreenHeight()/10, ScreenWidth()/10 + 2*ScreenWidth()/8,ScreenHeight()/10, p);
	}
	if(player.hp > 50){
		DrawLine( ScreenWidth()/10 + 2*ScreenWidth()/8 + 2, ScreenHeight()/10, ScreenWidth()/10 + 3*ScreenWidth()/8,ScreenHeight()/10, p);
	}
	if(player.hp > 75){
		DrawLine( ScreenWidth()/10 + 3*ScreenWidth()/8 + 2, ScreenHeight()/10, ScreenWidth()/10 + 4*ScreenWidth()/8,ScreenHeight()/10, p);
	}

	if(playerHitDetection()){
		if(player.hp <= 0) gameOver = true;
	}
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
		e.x += e.dx * distX / distance * GetElapsedTime();
		e.y += e.dy * distY/distance * GetElapsedTime();
		enemyShoot(e, e.luck);
		drawWireFrameModel(vecModelEnemy1, e.x, e.y, e.angle, SWR, SHR, olc::Pixel(0,0,255));
		if(e.hp >= 0){
			DrawLine(int(e.x + vecModelEnemy1[0].first), int(e.y + vecModelEnemy1[2].second)-2, 
			         int(e.x +(vecModelEnemy1[1].first*(e.hp/e.totalhp))), 
					 int(e.y + vecModelEnemy1[2].second)-2, olc::Pixel(255,0,0));
		}
	}
}

int game::getInvulnTime()const
{
    std::chrono::high_resolution_clock::time_point tmp = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(tmp - invulnTimer).count();
}

void game::enemyShoot(const sEntity o, int chance)
{
	int roll = int(rand() % 100);
	float distX = o.x - player.x;
	float distY = o.y - player.y;
	float distance = sqrtf(distX * distX + distY * distY );
	if(roll < chance){
		vecBullets.push_back({o.x, o.y, 1.25f*defvelX * distX / distance, 
								1.25f*defvelY * distY/distance, 0, 1,1, 50});
	}
}