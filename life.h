/*
 * Exported with BrandonTools v0.9
 * Invocation command was BrandonTools -mode4 -resize=240,160 -start=4 -palette=251 life game-of-life.jpg 
 * Time-stamp: Wednesday 11/28/2012, 10:25:15
 * 
 * Image Information
 * -----------------
 * game-of-life.jpg 240@160
 * 
 * Quote/Fortune of the Day!
 * -------------------------
 * I plan on living forever.  So far, so good. ~Author Unknown
 * 
 * All bug reports / feature requests are to be sent to Brandon (brandon.whitehead@gatech.edu)
 */
//Jonathan Suit
#ifndef LIFE_BITMAP_H
#define LIFE_BITMAP_H

extern const unsigned short life[19200];
extern const unsigned short life_palette[255];
#define LIFE_WIDTH 240
#define LIFE_HEIGHT 160
#define LIFE_PALETTE_SIZE 255
#define LIFE_PALETTE_OFFSET 4

#endif
