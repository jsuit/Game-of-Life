#include "mylib.h"
#include "debugging.h"
#include "stimulate.h"
//Jonathan Suit
int returnCount();
void drawGridSimulate2();
u8 getPixel2(int, int);
int simulate(){
	int timestep = 1;
	//TIMESTEP MODE
	//draw the grid without the cursor
	if(KEY_DOWN_NOW(BUTTON_SELECT)) return EDIT;	
	drawGridSimulate();
	FlipPage();
	drawGridSimulate();
	FlipPage();
	if(KEY_DOWN_NOW(BUTTON_SELECT)) return EDIT;			
	//display it so user can know we are in stimulation mode
	while(1){
		if(KEY_DOWN_NOW(BUTTON_SELECT)) return EDIT;	
		if(KEY_DOWN_NOW(BUTTON_B)){
			while(KEY_DOWN_NOW(BUTTON_B));
			timestep = 1;
		}
		if(timestep){	
			int value = timeStep();
			if(value == EDIT) return EDIT;
			else timestep = 0;	 		
		}else{
			if(KEY_DOWN_NOW(BUTTON_SELECT)) return EDIT;			
			applyRules();			
			FlipPage();
		}		
	}
}


int timeStep(){
	while(1){
		applyRules();	
		FlipPage();
		//DEBUG_PRINTF("PIXEL2 46,101= %d\n", getPixel(46,101));	
		
	//button A gets us out of time step mode
	// select gets out of edit mode
	// button b another tick
	while(!(KEY_DOWN_NOW(BUTTON_B) || KEY_DOWN_NOW(BUTTON_SELECT) || KEY_DOWN_NOW(BUTTON_A)));
	if(KEY_DOWN_NOW(BUTTON_B)){
		while(KEY_DOWN_NOW(BUTTON_B));
	}
	else if (KEY_DOWN_NOW(BUTTON_SELECT)) return EDIT;
	else if KEY_DOWN_NOW(BUTTON_A) return 0;
	}
}



//draws the grid without the cursor
void drawGridSimulate2(){
u8 pixel = 0;

for(int i = 0; i < MAXROW; i++){
		for(int j = 0; j < MAXCOL; j++){
			pixel = getPixel2(i,j);
			}
		}

	
}
void drawGridSimulate(){

	for(int i = 0; i < MAXROW; i++){
		for(int j = 0; j < MAXCOL; j++){
			u8 pixel = getPixel(i,j);
			if(pixel == CURSORALIVE) pixel = NOCURSORALIVE;
			else if(pixel == CURSORDEAD) pixel = NOCURSORDEAD;
			setPixel4(i,j,pixel);
		}
		
	}

}

void applyRules(){
	
	for(int i = 0; i < MAXROW; i++){
		for(int j = 0; j < MAXCOL; j++){
			int count = numNeighbors(i,j);
			u8 pixel = getPixel(i,j);
			//if(pixel)
			//DEBUG_PRINTF("pixel at %d, %d currently being displayed = %d\n", i, j, pixel);

			if((count == 2 || count == 3) && (pixel == NOCURSORALIVE)) setPixel4(i,j,NOCURSORALIVE); 				
			else if ((pixel == NOCURSORDEAD) && count == 3) setPixel4(i,j, NOCURSORALIVE);	
			else  setPixel4(i,j, NOCURSORDEAD);
		}
	}
}	



u8  getPixel2(int r, int c){
	int spot = (r * MAXCOL + c)/2;
	//odd col. insert into left side
	if(c & 1){
		return ((videoBuffer[spot] & 0xFF00) >> 8);
	}else{
		return (videoBuffer[spot] & 0x00FF); 
	}		
}
int numNeighbors(int row, int col){
	int count = 0;
	u8 pixel;
	
	if (row > 0){
		pixel = getPixel(row -1, col);
		if(pixel == NOCURSORALIVE){
			++count;
		}	
	}
	if( row < MAXROW){ 
		pixel = getPixel(row + 1, col);
		if(pixel == NOCURSORALIVE) ++count;
	}
	if(col > 0){
		pixel = getPixel(row, col -1);
		if(pixel == NOCURSORALIVE)++count;
	}
	if(col < MAXCOL){
		pixel = getPixel(row, col + 1);
		if(pixel == NOCURSORALIVE) ++count;
	}
	if(row > 0 && col > 0){
		pixel = getPixel(row -1, col -1);
		if(pixel == NOCURSORALIVE) ++count;	
	}	
	if(row < MAXROW && col > 0){
		pixel = getPixel(row +1, col -1);
		if(pixel == NOCURSORALIVE) ++count;
	}
	if(row > 0 && col < MAXCOL){
		pixel = getPixel(row -1, col + 1);
		if(pixel == NOCURSORALIVE) ++count;
	}
	if(row < MAXROW && col < MAXCOL){
			
		pixel = getPixel(row + 1, col + 1);
		
		if(pixel == NOCURSORALIVE) ++count;
	}

	return count;
			
}
