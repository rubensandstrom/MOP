/*
 * 	startup.c
 *
 */
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

#include <stdint.h>
typedef struct {
	uint32_t moder;
	uint16_t oTyper;
	uint16_t oTReserved;
	uint32_t oSpeedr;
	uint32_t pupdr;
	uint8_t idrLow;
	uint8_t idrHigh;
	uint16_t idrReserved;
	uint8_t odrLow;
	uint8_t odrHigh;
	uint16_t bs;
	uint16_t br;
	uint32_t lckr;
	uint32_t afrl;
	uint32_t afrh;
} GPIO;

#define GPIO_D *((GPIO *) 0x40020C00)
#define GPIO_E *((GPIO *) 0x40021000)

void init () {
	GPIO_D.moder &= 0x00000000;
	GPIO_D.moder |= 0x55005555; // D [15-0]: ooiioooo
	GPIO_D.oTyper &= 0x0000; // D [15-8]: push-pull
	GPIO_D.pupdr &= 0x00000000;
	GPIO_D.pupdr |= 0xAAAA5555; // D [15-8]: pull down
}

void out7seg (unsigned char c) {
	GPIO_D.odrLow &= 0x00;
	 
	unsigned char key[14] = "123A456B789C0D";
	unsigned char code[14] = [0x06, 0x5B, 0x4F, 0x77, 0x66, 0x6D, 0x7D, 0x7C, 0x07, 0x7F, 0x67, 0x39, 0x3F, 0x5E];
	
	for (int i = 0; i <= 15; i++) {
		if c == key[i] {
			GPIO_D.odrLow |= code[i]
			break;
		}
	}
}

unsigned char keyb () {
	GPIO_D.odrHigh &= 0x00;
	unsigned char key[16] = "123A456B789C*0#D";
	
	for (int i = 0; i <= 3; i++) {
		GPIO_D.odrHigh |= (1<<(i+4));
		for (int j = 0; j <= 3; j++) {
			if GPIO_D.idrHigh && (1<<j) {
				return key[i*4 + j];
			}
		}
	}
}

void main (void)
{
	unsigned char c;
	init();
	while(1) {
		out7seg(keyb());
	}
}

