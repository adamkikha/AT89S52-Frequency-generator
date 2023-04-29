#include <reg52.h>
#define SEGMENTS P2

sbit R0 = P1^0;
sbit R1 = P1^1;
sbit R2 = P1^2;
sbit R3 = P1^3;
sbit C0 = P1^4;
sbit C1 = P1^5;
sbit C2 = P1^6;
sbit C3 = P1^7;

sbit D0 = P0^0;
sbit D1 = P0^1;
sbit D2 = P0^2;
sbit D3 = P0^3;
sbit SIGNAL = P3^7;

code const unsigned char numbers[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
code const unsigned char segments[10] = {192,249,164,176,153,146,130,248,128,144};
unsigned char mode2 , done , high , low , digit , digits[4] = {0,0,0,0};
unsigned short int oc  , counter , input = 0;
int remainder;
float time;
void display(void);
void debounce(void);

//begin function calculates time period form the entered frequency
void begin(void) interrupt 0
{
	if(input){
		time = 1.0 / (2*input);
		counter = time / 0.071111;
		oc = counter;
		remainder = (time - counter * 0.071111)* 1000000 / 1.085069;
		remainder = ~remainder + 1;
		high = remainder >> 8;
		low = remainder & 0x00FF;
		SIGNAL = 1;
		ET1 = 1;
		TMOD = ( TMOD & 0x0F ) | 0x10;
		if(counter == 0){
			if(high == 0xFF){			
				TMOD = ( TMOD & 0x0F ) | 0x20;
				TH1 = low;
				mode2 = 1;
			}
			else{
				TH1 = high;
				TL1 = low;
			}
		}
		else{
			TH1 = 0;
			TL1 = 0;
			done = 0;
		}
		TR1 = 1;
	}
}

// toggles the signal
void flip(void) interrupt 3{
	if(mode2) SIGNAL = ~SIGNAL;
	else{ 
		if(oc == 0){
			SIGNAL = ~SIGNAL;
			TH1 = high;
			TL1 = low;
		}
		else{
			if(done){
				SIGNAL = ~SIGNAL;
				counter = oc;
				done = 0;
			}
			if(counter == 0){
				TH1 = high;
				TL1 = low;
				done = 1;
			}
			else{
				counter--;
				TH1 = 0;
				TL1 = 0;
			}
		}
	}
}

// keypad scanning
void main(){
	unsigned char i,j;
	EX0 = 1;
	IT0	= 1;
	ET0 = 1;
	EA = 1;
	PT1 = 1;
	TMOD = 0x01;
	TH0 = 0xBF;
	TL0 = 0;
	TR0 = 1;
	while(1){
		digit=0xFF;
		P1 = 0x0F;
		while (P1 == 0x0F);
		if(R3 == 0) digit = 0;
		else if(R2 == 0) i=2;
		else if(R1 == 0) i=1;
		else if(R0 == 0) i=0;
		
		P1 = 0xF0;
		if(digit != 0){
			if(C2 == 0) j=2;
			else if(C1 == 0) j=1;
			else if(C0 == 0) j=0;
			digit = numbers[i][j];
		}
		if(C3 != 0){
			debounce();
			digits[0] = digits[1];
			digits[1] = digits[2];
			digits[2] = digits[3];
			digits[3] = digit;
			input = digits[0]*1000 + digits[1]*100 + digits[2]*10 + digits[3];
		}
	}
}

void display(void) interrupt 1{
	unsigned char i;
	SEGMENTS = segments[digits[0]];
	D0 = 0;
	for(i = 255;i>0;i--);
	D0 = 1;
	SEGMENTS = segments[digits[1]];
	D1 = 0;
	for(i = 255;i>0;i--);
	D1 = 1;
	SEGMENTS = segments[digits[2]];
	D2 = 0;
	for(i = 255;i>0;i--);
	D2 = 1;
	SEGMENTS = segments[digits[3]];
	D3 = 0;
	for(i = 255;i>0;i--);
	D3 = 1;
	TH0 = 0xBF;
	TL0 = 0;
	TR0 = 1;
}

void debounce(void)
{
	unsigned char i ;
	while( P1 != 0xF0 );		
	for( i = 255; i > 0; i--)
		if ( P1 != 0xF0 )
				i = 255;	
}