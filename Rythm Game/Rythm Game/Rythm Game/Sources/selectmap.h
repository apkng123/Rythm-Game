
#pragma once

#include <io.h>
#include "main.h"
#include "parson.h"


int mapCount; 
char** mapList; 

const int slp = 32; 
const int stp = 2; 

char* statsPath; 


char* main_selectmap();
int loadMaps();
int showMapList();
int selecting(int mapCount);
void blink(int pointer);
