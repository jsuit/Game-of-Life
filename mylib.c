
#include "mylib.h" 
#include "debugging.h"

void waitForVblank(){
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void setPixel4(int row, int col, u8 indexPalette){
	int spot = (row * MAXCOL + col)/2;
	//odd col. insert into left side
	if(col & 1){
		videoBuffer[spot] = (videoBuffer[spot] & 0x00FF) | (indexPalette << 8);
	}else{
		videoBuffer[spot] = (videoBuffer[spot] & 0xFF00) | indexPalette;
	}	
	
}

u8  getPixel(int r, int c){
	int spot = (r * MAXCOL + c)/2;
	//odd col. insert into left side
	if(c & 1){
		return ((readBuffer[spot] & 0xFF00) >> 8);
	}else{
		return (readBuffer[spot] & 0x00FF); 
	}		
}
void FlipPage(){

	//if bufferflag is on, we were displaying backbuffer
	// change it so that we can write to backbuffer

	if(REG_DISPCTL & BUFFERFLAG){
		waitForVblank();
		REG_DISPCTL &= ~BUFFERFLAG;
		videoBuffer = BACKBUFFER;	//write to backbuffer
		readBuffer = FRONTBUFFER;	//read from frontbuffer (what is being displayed)
	}else{	
		waitForVblank();
		REG_DISPCTL |= BUFFERFLAG;
		videoBuffer = FRONTBUFFER;
		readBuffer = BACKBUFFER;
	}
}
void fillScreen4(u8 index){
	volatile u16 indices = (index << 8) | index;
	dma_transfer(videoBuffer, &indices, MODE4MAXSIZE,3, DMA_ENABLE|DMA_SOURCE_FIXED);
}

void dma_memcopy4( void * dst, const volatile void * src, int count){
 
	dma_transfer(dst, src, count, 3, DMA_ENABLE);
}

void dma_transfer(void * dst, volatile const void *src, int cnt, int ch, int mode){
	
	DMA_MEM[ch].cnt = 0; 	//stop previous transfer
	DMA_MEM[ch].src = src;	
	DMA_MEM[ch].dst =  dst;
	DMA_MEM[ch].cnt = (cnt) | (mode);
}

void drawImage4(int r, int c, int width, int height, volatile const u16 * image)
{
		for(int i = 0; i < height; i++)
   			dma_memcopy4(videoBuffer + (OFFSET(i + r, c))/2, image+(width*i)/2, width/2);
  			
}



void setPalette(const u16 * palette, int paletteSize, const u16 * palette2, int paletteSize2){
	dma_memcopy4(PALETTE, palette, paletteSize);
	//dma_memcopy4(PALETTE, palette2, paletteSize2);
}

int titleScreen(const u16 * image){
	drawImage4(0, 0, 240, 160, image);
	FlipPage();
	while(1){
		if(KEY_DOWN_NOW(BUTTON_START)){
			while(KEY_DOWN_NOW(BUTTON_START));
			return EDIT;
		}

	}
	
	
	return 0;
}

void clearScreens(){
	fillScreen4(0);
	FlipPage();
	fillScreen4(0);
	FlipPage();
}

void readGridEdit() {
	//int max = MAXROW*MAXCOL/2;
	 u16 pixel;
	
/*
	for(int i = 1; i < 10; i++){
		for(int j = 0; j < 4; j++){
			setPixel4(i,j, CURSORALIVE);
		}
	}
*/
	
	
	for(int i = 0; i < MAXROW; i++){
		for(int j = 0; j < MAXCOL; j++){
			pixel = getPixel(i,j);
			if(i == cRow && j == cCol){
				if(pixel > 3)				
					setPixel4(i,j, CURSORDEAD);
				else if(pixel == NOCURSORALIVE) setPixel4(i,j, CURSORALIVE);
				else if(pixel == NOCURSORDEAD) setPixel4(i,j ,CURSORDEAD);	
				else setPixel4(i,j, pixel);			
			}
			else if(pixel == NOCURSORDEAD || pixel == NOCURSORALIVE){
				
				 setPixel4(i,j, pixel);			
			}
			else setPixel4(i,j, NOCURSORDEAD);			
		}
	}
	
}



int edit(){

	//draw the previous grid
	/*
	if(KEY_DOWN_NOW(BUTTON_SELECT)){
		while(KEY_DOWN_NOW(BUTTON_SELECT));
		return SIMULATE;
	}
	*/
	//FlipPage();	
	readGridEdit();
	FlipPage(); //display it (so we can see the cursor)
	readGridEdit(); // read the grid again, so we can manipulate it
	FlipPage();	
	while(1){

		if(KEY_DOWN_NOW(BUTTON_SELECT)){
			while(KEY_DOWN_NOW(BUTTON_SELECT));
			return SIMULATE;
		}	
		if(KEY_DOWN_NOW(BUTTON_R)){
			while(KEY_DOWN_NOW(BUTTON_R));
			clearScreens();
			setPixel4(cRow, cCol, CURSORDEAD);
			FlipPage();
		}
		if(KEY_DOWN_NOW(BUTTON_A)){
			while(KEY_DOWN_NOW(BUTTON_A));
			u8 pixel = getPixel(cRow, cCol);
			if(pixel == CURSORALIVE){
			 	setPixel4(cRow, cCol, CURSORDEAD);
			}else if(pixel == CURSORDEAD)
				setPixel4(cRow, cCol, CURSORALIVE);
			FlipPage();
			//get pixel from previous page. (should be either cursoralive or
 			//cursordead			
			pixel = getPixel(cRow,cCol);
			setPixel4(cRow, cCol, pixel);
		}
		if(KEY_DOWN_NOW(BUTTON_LEFT)){
			while(KEY_DOWN_NOW(BUTTON_LEFT));
			if(cCol - 1 >= 0){
				DEBUG_PRINTF("oldpixel1 = %d\n", 0);
				cCol--;
				u8 newpixel = getPixel(cRow,cCol);
				u8 oldpixel = getPixel(cRow,cCol+1);
				if(newpixel == NOCURSORDEAD) setPixel4(cRow,cCol, CURSORDEAD);
				else setPixel4(cRow,cCol, CURSORALIVE);
				if(oldpixel == CURSORALIVE) setPixel4(cRow,cCol + 1, NOCURSORALIVE);
				else setPixel4(cRow,cCol+1, NOCURSORDEAD);
				FlipPage();
				if(newpixel == NOCURSORDEAD) setPixel4(cRow, cCol, CURSORDEAD);
				else if(newpixel == NOCURSORALIVE) setPixel4(cRow, cCol, CURSORALIVE);
				if(oldpixel == CURSORALIVE) setPixel4(cRow, cCol+1, NOCURSORALIVE);
				else if(oldpixel == CURSORDEAD) setPixel4(cRow, cCol+1, NOCURSORDEAD);
			}
		}
		 if(KEY_DOWN_NOW(BUTTON_RIGHT)){
			while(KEY_DOWN_NOW(BUTTON_RIGHT));
			if(cCol + 1 < MAXCOL){
				cCol++;
				u8 newpixel = getPixel(cRow, cCol); 
				u8 oldpixel = getPixel(cRow, cCol -1);
				if(newpixel == NOCURSORDEAD) setPixel4(cRow, cCol, CURSORDEAD);
				else setPixel4(cRow, cCol, CURSORALIVE);
				if(oldpixel == CURSORALIVE) setPixel4(cRow, cCol-1, 			
								NOCURSORALIVE);
				else setPixel4(cRow, cCol -1, NOCURSORDEAD);
				FlipPage();	
				if(newpixel == NOCURSORDEAD) setPixel4(cRow, cCol, CURSORDEAD);
				else if(newpixel == NOCURSORALIVE) setPixel4(cRow, cCol, CURSORALIVE);
				if(oldpixel == CURSORALIVE) setPixel4(cRow, cCol-1, NOCURSORALIVE);
				else if(oldpixel == CURSORDEAD) setPixel4(cRow, cCol-1, NOCURSORDEAD);
			}
		}
		 if(KEY_DOWN_NOW(BUTTON_DOWN)){
			while(KEY_DOWN_NOW(BUTTON_DOWN));
			if(cRow + 1 < MAXROW){
				cRow++;
				u8 newpixel = getPixel(cRow, cCol); 
				u8 oldpixel = getPixel(cRow - 1, cCol);
				if(newpixel == NOCURSORDEAD) setPixel4(cRow, cCol, CURSORDEAD);
				else if(newpixel == NOCURSORALIVE) setPixel4(cRow, cCol, CURSORALIVE);
				if(oldpixel == CURSORALIVE) setPixel4(cRow - 1 , cCol, 			
								NOCURSORALIVE);
				else setPixel4(cRow-1, cCol, NOCURSORDEAD);
				FlipPage();	
				if(newpixel == NOCURSORDEAD) setPixel4(cRow, cCol, CURSORDEAD);
				else if(newpixel == NOCURSORALIVE) setPixel4(cRow, cCol, CURSORALIVE);
				if(oldpixel == CURSORALIVE) setPixel4(cRow-1, cCol, NOCURSORALIVE);
				else if(oldpixel == CURSORDEAD) setPixel4(cRow-1,cCol, NOCURSORDEAD);
			}
		}
		if(KEY_DOWN_NOW(BUTTON_UP)){
			if(cRow - 1 >= 0 ){
				while(KEY_DOWN_NOW(BUTTON_UP));
				cRow--;
				u8 newpixel = getPixel(cRow, cCol); 
				u8 oldpixel = getPixel(cRow + 1, cCol);
				if(newpixel == NOCURSORDEAD) setPixel4(cRow, cCol, CURSORDEAD);
				else if(newpixel == NOCURSORALIVE) setPixel4(cRow, cCol, CURSORALIVE);
				if(oldpixel == CURSORALIVE) setPixel4(cRow + 1 , cCol, 			
								NOCURSORALIVE);
				else setPixel4(cRow+1, cCol, NOCURSORDEAD);
				FlipPage();	
				if(newpixel == NOCURSORDEAD) setPixel4(cRow, cCol, CURSORDEAD);
				else if(newpixel == NOCURSORALIVE) setPixel4(cRow, cCol, CURSORALIVE);
				if(oldpixel == CURSORALIVE) setPixel4(cRow+1, cCol, NOCURSORALIVE);
				else if(oldpixel == CURSORDEAD) setPixel4(cRow+1,cCol, NOCURSORDEAD);
			}
		}
					

	}
}
