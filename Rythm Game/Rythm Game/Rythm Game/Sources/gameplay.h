
#pragma once
#pragma comment(lib,"winmm.lib")

#include <time.h>
#include <Windows.h>
#include <mmsystem.h>
#include <Digitalv.h>
#include <wchar.h>
#include <math.h>
#include "parson.h"
#include "main.h"


#define NOTETHK 6 
#define HEI 20 

#define N '@'
#define x ' ' 


const int glp = (SCREEN_WIDTH/2) - (LINE * NOTETHK / 2); 
const int gtp = 0; 


char* mapName;
char* mapDir; 
JSON_Object* mapInfo;
char** map; 
int mapLength; 
int FALLSPEED = 150; 

char note[HEI][LINE];

int mapIndex; 
int noteCount; 
clock_t pauseTimer;
int score;
int combo;
double accuracy;

BOOL paused;
BOOL songPlayed;
BOOL gameEnd;
BOOL quit;


int shouldRemove[LINE];
BOOL isPressed[LINE]; 



MCI_OPEN_PARMS openBgm;
int dwID;



extern char* statsPath;


void main_gameplay(char* _mapName);

void init();
int readMap();
void initBgm();

void drawScreen();
void countdown();

void fallingNote();
void showNotes();
void keyInput();
void press(int line);
void hitNote(int line, int judgement);
void removingJudgeTxt();

void pause();

void playBgm();
void updateUI(int comboPlus);

void showStats();
