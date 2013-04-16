#include "mylib.h"
#include "life.h"
#include "debugging.h"
#include "stimulate.h"
#include "mypalette.h"
//Jonathan Suit
u16 * videoBuffer = BACKBUFFER;
u16 * readBuffer = FRONTBUFFER;
u32 cRow = 45;
u32 cCol = 100;
int main(){
	REG_DISPCTL = BG2_ENABLE | MODE4;
	STATE state;
	setPalette(life_palette, 255, palette_index, MAXWIDTH);
	clearScreens();
	state = TITLESCREEN;
	while(1){

		if(state == TITLESCREEN){
			state = titleScreen(life+4);
			//call function to DRAW title screen
			// function should return edit or simulate
		}
		if(state == EDIT){
			state = edit();
		}
		if(state == SIMULATE){
			state = simulate();
		}
	}
return 0;
}
