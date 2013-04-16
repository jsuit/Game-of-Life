typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char u8;

#define MAXCOL	240
#define	MAXROW 160
#define REG_DISPCTL *(u16 *)0x4000000
#define MODE3 3
#define MODE4 4

#define FRONTBUFFER (u16 *)0x6000000
#define BACKBUFFER (u16 *)0x600A000
#define BUFFERFLAG (1<<4)
#define PALETTE ((u16 *)0x5000000)

#define BG2_ENABLE (1<<10)
#define COLOR(r,g,b) ((r) | (g) << 5 | (b) << 10)
#define RED COLOR(31,0,0)
#define WHITE COLOR(31,31,31)
#define BLACK 0
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define CYAN COLOR(0,31,31)
#define MAGENTA COLOR(31,0,31)
#define YELLOW COLOR(31, 31, 0)
#define LTGRAY COLOR(20, 20, 20)
#define MODE4MAXSIZE 19200
#define OFFSET(r,c) ((r)*240+(c))

/* Buttons */

#define BUTTON_A      (1<<0)
#define BUTTON_B      (1<<1)
#define BUTTON_SELECT (1<<2)
#define BUTTON_START  (1<<3)
#define BUTTON_RIGHT  (1<<4)
#define BUTTON_LEFT   (1<<5)
#define BUTTON_UP     (1<<6)
#define BUTTON_DOWN   (1<<7)
#define BUTTON_R      (1<<8)
#define BUTTON_L      (1<<9)



#define BUTTONS (*( volatile unsigned int *)0x04000130)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

#define SCANLINECOUNTER (*(volatile unsigned short *)0x4000006)

/* DMA      NOTE!!! USES U32...don't forget to typedef it */

typedef struct 
{
	const volatile void *src;
	volatile void *dst;
	volatile u32 cnt;
} DMAREC;

#define DMA ((volatile DMAREC *)0x040000B0)

#define REG_DMA0SAD         *(u32*)0x40000B0  // source address
#define REG_DMA0DAD         *(u32*)0x40000B4  // destination address
#define REG_DMA0CNT         *(u32*)0x40000B8  // control register

/* DMA channel 1 register definitions */
#define REG_DMA1SAD         *(u32*)0x40000BC  // source address
#define REG_DMA1DAD         *(u32*)0x40000C0  // destination address
#define REG_DMA1CNT         *(u32*)0x40000C4  // control register

/* DMA channel 2 register definitions */
#define REG_DMA2SAD         *(u32*)0x40000C8  // source address
#define REG_DMA2DAD         *(u32*)0x40000CC  // destination address
#define REG_DMA2CNT         *(u32*)0x40000D0  // control register

/* DMA channel 3 register definitions */
#define REG_DMA3SAD         *(u32*)0x40000D4  // source address
#define REG_DMA3DAD         *(u32*)0x40000D8  // destination address
#define REG_DMA3CNT         *(u32*)0x40000DC  // control register

/* Defines */
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ENABLE (1 << 31)

#define START_ON_FIFO_EMPTY 0x30000000


#define DMA_MEM	((volatile DMAREC *) 0x040000B0)

#define NOCURSORDEAD	0
#define NOCURSORALIVE	1
#define CURSORALIVE	2
#define CURSORDEAD	3
typedef enum{CLEARSCREENS, TITLESCREEN, EDIT, SIMULATE} STATE; 
extern unsigned short *videoBuffer;
extern unsigned short *readBuffer;
extern u32	cRow;
extern u32	cCol;

// Prototypes
void setPixel(int row, int col, u16 color);
void drawRect(int row, int col, int height, int width, u16 color);
void waitForVblank();
void fillScreen(u16 color);
void drawFullScreen(const unsigned short bitmap[]);
void setPixel4(int row, int col, u8 index);
void FlipPage();
void fillScreen4(u8 index);
void drawRect4(int row, int col, int height, int width, u8);
void dma_memcopy4( void * dst, const volatile void * src, int count);
void dma_transfer(void * dst, volatile const void *src, int cnt, int ch, int mode);
void drawImage4(int r, int c, int width, int height, const  volatile u16 * image);
u8 getPixel(int r, int c);
void setPalette(const u16 * palette, int paletteSize, const u16 * palette2, int palettesize2);
int titleScreen(const u16 * image);
void clearScreens();
void readGridEdit();
int edit();
