#ifndef TOKENIZE_C
#define TOKENIZE_C
#include"tokenize.h"
#include<string.h>
#include<Arduino.h>

enum Tokens{BAD,NONE,D13,ON,OFF,BLINK,GREEN,RED,RGB,SET,STATUS,LEDS,VERSION,HELP};
#define STREAM_SIZE 16

typedef void*(*TokenState)(Tokens);

bool blinking = true;

void* Zero_State(Tokens);
void* D13_State(Tokens);
void D13_ON_State();
void D13_OFF_State();
void D13_BLINK_State();

Tokens getNumericalToken(char*stringToken){
	Tokens result = BAD;
	if(strcmp(stringToken,"D13") == 0){
		result = D13;
	}
	if(strcmp(stringToken, "ON") == 0){
		result = ON;
	}
	if(strcmp(stringToken , "OFF") == 0){
		result = OFF;
	}
	if(strcmp(stringToken , "BLINK") == 0){
		result = BLINK;
	}
	if(strcmp(stringToken , "GREEN") == 0){
		result = GREEN;
	}
	if(strcmp(stringToken , "RED") == 0){
		result = RED;
	}
	if(strcmp(stringToken , "RGB") == 0){
		result = RGB;
	}
	if(strcmp(stringToken , "SET") == 0){
		result = SET;
	}
	if(strcmp(stringToken , "STATUS") == 0){
		result = STATUS;
	}
	if(strcmp(stringToken , "LEDS") == 0){
		result = LEDS;
	}
	if(strcmp(stringToken , "VERSION") == 0){
		result = VERSION; 
	}
	if(strcmp(stringToken , "HELP") == 0){
		result =  HELP; 
	}
	if(strcmp(stringToken , "") == 0){
		result = NONE;
	}
	return result;
}

Tokens getToken(){
	char buffer[8];
	int i;
	bool terminated = false;
	for(i = 0; i < 8 && Serial.available() > 0; i++){
		char incomingByte = Serial.read();
		Serial.print(incomingByte);
		Serial.println("");
		if(incomingByte == '\r' 
		|| incomingByte == '\t' 
		|| incomingByte == ' ' 
		|| incomingByte == '\n'){
			buffer[i] = '\0';
			break;
		}
	}
	if(!terminated){
		buffer[0] = 'B';
		buffer[1] = '\0';
	}
	return getNumericalToken(buffer);
}

void* Zero_State(Tokens input){
	if(input == D13){
		return (void*)D13_State;
	} else {
		return (void*)Zero_State;
	}
}
void* D13_State(Tokens input){
	if(input ==  BAD){
		return (void*)Zero_State;
	} else if(input == ON){
		D13_ON_State();
	} else if(input == OFF){
		D13_OFF_State();
	} else if(input == BLINK){
		D13_BLINK_State();
	}
	return (void*)D13_State;
}
void D13_ON_State(){
	blinking = false;
	digitalWrite(13, HIGH);
}
void D13_OFF_State(){
	blinking = false;
	digitalWrite(13, LOW);
}
void D13_BLINK_State(){
	blinking = true;
}

void setup(){
	pinMode(13,OUTPUT);
	Serial.begin(9600);
}
void loop(){
	static TokenState currentState = Zero_State;
	Tokens transition  = getToken();
	currentState = (TokenState)(currentState(transition));
	} 
	if(blinking){
		digitalWrite(13,HIGH);
		delay(500);
		digitalWrite(13,LOW);
		delay(500);
	}
}
#endif

