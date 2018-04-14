//PS Vita imports
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vita2d.h>
#include <psp2/ctrl.h>



//General imports
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//PS Vita stuff

//Input stuff

int FPS = 0;


//Stuff stuff
typedef struct{
	int x, y;
	int CTRLVAR;
	int Health;
	int Power;
	int Spd;
	SDL_Texture* PlayerTex;
} Block;

typedef struct{
	int x, y;
	int Health;
	int Spd;
} Enemy;

SDL_Texture* PlayerTex = NULL;
SDL_Surface* PlayerImg = NULL;


//SDL Stuff
int processEvents(SDL_Window *window, Block *block){
	
	SDL_Event event;
	int done = 0;

	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_WINDOWEVENT_CLOSE:{
				if(window)
				{
					SDL_DestroyWindow(window);
					window = NULL;
					done = 1;
				}
			}
			break;
			case SDL_QUIT:
			done = 1;
			break;
		}
	}

	return done;
}


void Render(SDL_Renderer *renderer, Block *block, Enemy *enemy)
{
	

	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	SceCtrlData ctrl;
	sceCtrlPeekBufferPositive(0, &ctrl, 1);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
    
    

    SDL_Rect Player = {block->x, block->y, 25, 25};
	if(ctrl.buttons & (SCE_CTRL_CROSS)){
		if(block->Power > 1){
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		}
		else{
			SDL_SetRenderDrawColor(renderer, 255, 100, 100, 180);
			block->Power += 1.5;
		}
	}
	else{
		SDL_SetRenderDrawColor(renderer, 255, 100, 100, 180);
		
	}


	SDL_RenderCopy(renderer, block->PlayerTex, NULL, &Player);

	
	

	

	SDL_SetRenderDrawColor(renderer, 100, 100, 250, 190);
	
	SDL_Rect EnemyR = {enemy->x, enemy->y, 25, 25};
	SDL_RenderFillRect(renderer, &EnemyR);

  //Game room GUI 

  //Top bar box
	SDL_SetRenderDrawColor(renderer, 87, 87, 244, 255);

	SDL_Rect TBBB = {0, 40, 960, 10};
	SDL_Rect TBSBL = {0, 10, 10, 30};
	SDL_Rect TBSBR = {950, 10, 10, 30};
	SDL_Rect TBTB = {0, 0, 960, 10};

	SDL_RenderFillRect(renderer, &TBBB);
	SDL_RenderFillRect(renderer, &TBSBL);
	SDL_RenderFillRect(renderer, &TBSBR);
	SDL_RenderFillRect(renderer, &TBTB);

	SDL_SetRenderDrawColor(renderer, 135, 135, 237, 255);
	SDL_Rect TBBF = {10, 10, 940, 30};
	SDL_RenderFillRect(renderer, &TBBF);

  //Draw Health
	SDL_SetRenderDrawColor(renderer, (200 - block->Health), block->Health, 0, 200);
	SDL_Rect health = {10, 10, (block->Health * 2), 10};
	SDL_RenderFillRect(renderer, &health);
  //Draw Power
	SDL_SetRenderDrawColor(renderer, (250 - block->Power), (block->Power), 0, 200);
	SDL_Rect power = {10, 30, (block->Power), 10};
	SDL_RenderFillRect(renderer, &power);

  //Room bounding box
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 150);

	SDL_Rect PBBB = {0, 534, 960, 10};
	SDL_Rect PBBT = {0, 50, 960, 10};
	SDL_Rect PBBL = {0, 60, 10, 474};
	SDL_Rect PBBR = {950, 60, 10, 474};
	SDL_RenderFillRect(renderer, &PBBB);
	SDL_RenderFillRect(renderer, &PBBT);
	SDL_RenderFillRect(renderer, &PBBL);
	SDL_RenderFillRect(renderer, &PBBR);



	//Collision stuff

	//Check for collision with SDL bools
	SDL_bool TopCol = SDL_HasIntersection(&Player, &PBBT);
	SDL_bool LeftCol = SDL_HasIntersection(&Player, &PBBL);
	SDL_bool RightCol = SDL_HasIntersection(&Player, &PBBR);
	SDL_bool BottomCol = SDL_HasIntersection(&Player, &PBBB);

	SDL_bool EnemyCol = SDL_HasIntersection(&Player, &EnemyR);

	if(EnemyCol){
		if(enemy->x > block->x){
			enemy->x += 30;
		}

		if(enemy->x < block->x){
			enemy->x -= 30;
		}

		if(enemy->y > block->y){
			enemy->y += 30;
		}

		if(enemy->y < block->y){
			enemy->y -= 30;
		}
	}


	//Collision events
	if(ctrl.buttons & (SCE_CTRL_CROSS)){
		if(TopCol){
			block->y += 5.7;
		}

		if(LeftCol){
			block->x += 5.7;

		}

		if(RightCol){
			block->x -= 5.7;
		}

		if(BottomCol){
			block->y -= 5.7;
		}
	}
	else{
		if(TopCol){
			block->Spd = 0;
			block->y += 5.5;
		}

		if(LeftCol){
			block->Spd = 0;
			block->x += 5.3;

		}

		if(RightCol){
			block->Spd = 0;
			block->x -= 3.9;
		}

		if(BottomCol){
			block->Spd = 0;
			block->y -= 4.1;
		}
		else{
			block->Spd = 6.5;
		}
	}


	SDL_RenderPresent(renderer);
}

void Controls(Block *block){

	int XP;
	int XN;
	int YP;
	int YN;
	int Spd;

	Spd = block->Spd;
	XN = 0;
	XP = 0;
	YN = 0;
	YP = 0;
	
    //Setup
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	SceCtrlData ctrl;
	sceCtrlPeekBufferPositive(0, &ctrl, 1);

	//Specialu powarr!
	if(ctrl.buttons & (SCE_CTRL_CROSS)){
		if (block->Power > 5){
			Spd += 2.5;
			block->Power -= 1.5;
		}
	}
	else{
		if(block->Power < 248.5){ 
			if(ctrl.buttons & (SCE_CTRL_CROSS)){
			}
			else{
				block->Power += 1.5;
			}
		}
	}

	//Control switcher
	if(ctrl.buttons & (SCE_CTRL_TRIANGLE)){
		SDL_Delay(300);
		if(block->CTRLVAR == 1){
			block->CTRLVAR = 0;
		}
		else{
			block->CTRLVAR = 1;
		}
	}
	
    //Analogue controls
	if(block->CTRLVAR == 1){
		if(ctrl.lx > 177.5){
			XP += 1;
		}

		if(ctrl.lx < 77.5){
			XN += 1;
		}

		if(ctrl.ly > 177.5){
			YP += 1;
		}

		if(ctrl.ly < 77.5){
			YN += 1;
		}
	}
	

	//TO DO TOMORROW ~ MOVE CONTROL STUFF AFTER BOOL CHECKS FOR BETTER COLLISION DETECTION!

	
    //D-PAD controls

	if(block->CTRLVAR == 0){
		if(ctrl.buttons & (SCE_CTRL_UP)){
			YN += 1;
		}

		if(ctrl.buttons & (SCE_CTRL_DOWN)){
			YP += 1;
		}

		if(ctrl.buttons & (SCE_CTRL_LEFT)){
			XN += 1;
		}

		if(ctrl.buttons & (SCE_CTRL_RIGHT)){
			XP += 1;
		}
	}
	


	if(XP >= 1){
		block->x += Spd;
	}

	if(XN >= 1){
		block->x -= Spd;
	}

	if(YP >= 1){
		block->y += Spd;
	}

	if(YN >= 1){
		block->y -= Spd;
	}
}

void EnemyFollow(Block *block, Enemy *enemy){
	int ESpd;
	ESpd = enemy->Spd;
	if(enemy->x > block->x){
		enemy->x -= ESpd;
	}

	if(enemy->x < block->x){
		enemy->x += ESpd;
	}

	if(enemy->y > block->y){
		enemy->y -= ESpd;
	}

	if(enemy->y < block->y){
		enemy->y += ESpd;
	}
}

int main(int argc, char *argv[]) {

    SDL_Window *window;                
    SDL_Renderer *renderer;                
  
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
  
    window = SDL_CreateWindow("Window1",                
                              SDL_WINDOW_FULLSCREEN,
                              SDL_WINDOW_FULLSCREEN,      
                              960,                            
                              544,                       
                              0                              
                              );
    renderer = SDL_CreateRenderer(window, -1, SDL_WINDOW_OPENGL);

    
    
    

    Block block;
    block.x = 480;
    block.y = 272;
    block.CTRLVAR = 0;
    block.Health = 200;
    block.Power = 250;
    block.Spd = 6.5;
    
    Enemy enemy;
    enemy.x = 400;
    enemy.y = 200;
    enemy.Health =300;
    enemy.Spd = 4; 


    SDL_Surface* PlayerImg = IMG_Load("app0:/assets/Player.png");
    block.PlayerTex = SDL_CreateTextureFromSurface(renderer, PlayerImg);
    SDL_FreeSurface(PlayerImg);
    
    

    int done = 0;

    while(!done){

    	done = processEvents(window, &block);
    	EnemyFollow(&block, &enemy);
    	Render(renderer, &block, &enemy);
    	Controls(&block);
    	SDL_Delay(1000/45);
    }


    IMG_Quit();
    SDL_DestroyTexture(block.PlayerTex);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}