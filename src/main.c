//PS Vita imports
#include <SDL2/SDL.h>
#include <vita2d.h>
#include <psp2/ctrl.h>



//General imports
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//PS Vita stuff

//Input stuff

//Stuff stuff
typedef struct{
	int x, y;
	int CTRLVAR;
	int Health;
	int Power;
} Block;


//SDL Stuff
int processEvents(SDL_Window *window, Block *block){
	
	SDL_Event event;
	int done = 0;

	int XP;
	int XN;
	int YP;
	int YN;
	int Spd;

	Spd = 4;
	XN = 0;
	XP = 0;
	YN = 0;
	YP = 0;


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


    //PS Vita controller stuff

    //Vars

    //Setup
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	SceCtrlData ctrl;
	sceCtrlPeekBufferPositive(0, &ctrl, 1);

	//Specialu powarr!
	if(ctrl.buttons & (SCE_CTRL_CROSS)){
		Spd += 2.5;
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

	return done;
}



void Render(SDL_Renderer *renderer, Block *block)
{

	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	SceCtrlData ctrl;
	sceCtrlPeekBufferPositive(0, &ctrl, 1);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	SDL_RenderClear(renderer);


	
	SDL_Rect Player = {block->x, block->y, 25, 25};
	SDL_RenderClear(renderer);
	if(ctrl.buttons & (SCE_CTRL_CROSS)){
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	}
	else{
		SDL_SetRenderDrawColor(renderer, 255, 100, 100, 180);
		
	}
	SDL_RenderFillRect(renderer, &Player);

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
	SDL_Rect power = {10, 20, (block->Power), 10};
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

	SDL_bool TopCol = SDL_HasIntersection(&Player, &PBBT);
	SDL_bool LeftCol = SDL_HasIntersection(&Player, &PBBL);
	SDL_bool RightCol = SDL_HasIntersection(&Player, &PBBR);
	SDL_bool BottomCol = SDL_HasIntersection(&Player, &PBBB);

	if(TopCol){
		block->y += 5;
	}

	if(LeftCol){
		block->x += 5;

	}

	if(RightCol){
		block->x -= 5;
	}

	if(BottomCol){
		block->y -= 5;
	}

	SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {

    SDL_Window *window;                
    SDL_Renderer *renderer;                
  
    SDL_Init(SDL_INIT_VIDEO);             
  
    window = SDL_CreateWindow("Window1",                
                              SDL_WINDOW_FULLSCREEN,
                              SDL_WINDOW_FULLSCREEN,      
                              960,                            
                              544,                       
                              0                                
                              );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Block block;
    block.x = 480;
    block.y = 272;
    block.CTRLVAR = 0;
    block.Health = 200;
    block.Power = 250;
    


    int done = 0;
  

    while(!done){
    	done = processEvents(window, &block);
    	Render(renderer, &block);
    	SDL_Delay(10);
    }
  
  

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}