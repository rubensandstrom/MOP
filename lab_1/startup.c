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
	GPIO_D.moder &= 0x0000FFFF;
	GPIO_D.moder |= 0x55000000; // D [15-8]: ooooiiii
	GPIO_D.oTyper &= 0x00FF; // D [15-8]: push-pull
	GPIO_D.pupdr &= 0x0000FFFF;
	GPIO_D.pupdr |= 0xAAAA0000; // D [15-8]: pull down
}
void out7seg (char c) {
	GPIO_D.odrLow &= 0x00;
	switch c {
		case '1':
			GPIO_D.odrLow |= 0b00000110;
			break;
		case '2'
			GPIO_D.odrLow |= 0b01011011;
			break;
		case '3'
			GPIO_D.odrLow |= 0b01001111;
			break;
		case 'A'
			GPIO_D.odrLow |= 0b01110111;
			break;
		case '4'
			GPIO_D.odrLow |= 0b01100110;
			break;
		case '5'
			GPIO_D.odrLow |= 0b01101101;
			break;
		case '6'
			GPIO_D.odrLow |= 0b01111101;
			break;
		case 'B'
			GPIO_D.odrLow |= 0b01111100;
			break;
		case '7'
			GPIO_D.odrLow |= 0b00000111;
			break;
		case '8'
			GPIO_D.odrLow |= 0b01111111;
			break;
		case '9'
			GPIO_D.odrLow |= 0b01101111;
			break;
		case 'C'
			GPIO_D.odrLow |= 0b00111001;
			break;
		case '*'
			break;
		case '0'
			GPIO_D.odrLow |= 0b00111111;
			break;
		case '#'
			break;
		case 'D'
			GPIO_D.odrLow |= 0b01011110;
			break;
	}
}

char keyb () {
	GPIO_D.odrHigh &= 0x00;
	char key[16] = "123A456B789C*0#D";
	
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

