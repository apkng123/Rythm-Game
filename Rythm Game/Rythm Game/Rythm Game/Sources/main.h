
#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "fun.h"

#define SCREEN_WIDTH 120 
#define SCREEN_HEIGHT 30 

#define LINE 4 


enum KeyCode { 
	BACKSPACE = 8,
	TAB = 9, 
	ENTER = 13, 
	ESC = 27,
	SPACE = 32, 
	UP = 72, 
	DOWN = 80 
};


int key[LINE];
wchar_t keyName[LINE][5]; 
const char* mapFolder; 


extern int mapCount;
extern char** mapList;


void main_keysetting();
void main_title();
char* main_selectmap();
void main_gameplay(char* _mapName);
