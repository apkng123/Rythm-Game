
#include "selectmap.h"

char* main_selectmap() {
	setCursor(HIDE);
	system("cls");

	mapCount = 0;
	if (loadMaps() == -1) return NULL;
	
	if (mapCount == 0 || showMapList() == -1) {
		system("cls");
		setColor(GRAY);
		gotoxy(slp, stp); puts(":( 맵이 없네요..");
		gotoxy(slp, stp+3); puts("맵 제작법은 맵_제작법.md 파일을 참조하세요.");
		gotoxy(slp, stp+4); setCursor(UNDERBAR);

		while (1);
	}

	else {
		
	}

	int pointer = selecting(mapCount);
	blink(pointer);
	return mapList[pointer];
}



int loadMaps() {

	mapFolder = "maps";

	static char* dir = NULL;
	if (dir == NULL) {
		dir = malloc(strlen(mapFolder)+2+1);
		if (dir == NULL) return -1;
		sprintf_s(dir, strlen(mapFolder)+2+1, "%s/*", mapFolder);
	}

	
	struct _finddata_t fd;
	intptr_t handle = _findfirst(dir, &fd);
	if (handle == -1) {
		mapCount = 0;
		return 0;
	}

	while (_findnext(handle, &fd) != -1) {
		if (!strcmp(fd.name, "..") || !(fd.attrib & _A_SUBDIR)) 
			continue;
		mapCount++;
	}

	mapList = malloc(mapCount * sizeof(char*));
	if (mapList == NULL) return -1;

	handle = _findfirst(dir, &fd);
	for (int i = 0; i <= mapCount; i++) {
		if (_findnext(handle, &fd) == -1)
			break;
		if (!strcmp(fd.name, ".") || !strcmp(fd.name, "..") || !(fd.attrib & _A_SUBDIR)) { 
			i--; continue;
		}

		mapList[i] = malloc(strlen(fd.name)+1);
		if (mapList[i] == NULL) return -1;
		strcpy_s(mapList[i], strlen(fd.name)+1, fd.name);
	}

	_findclose(handle);

	return 0;
}


int showMapList() {

	// 안내
	setColor(GRAY);
	gotoxy(slp, stp); puts("곡을 선택하세요.");
	



	const char* highScoreFileName = "stats.dat";
	const int hsfPathSize = (int)strlen(mapFolder) + 1 + (int)strlen(highScoreFileName) + 1;
	statsPath = malloc(hsfPathSize);
	if (statsPath == NULL) return -1;
	sprintf_s(statsPath, hsfPathSize, "%s/%s", mapFolder, highScoreFileName);

	JSON_Value* rootValue = json_parse_file(statsPath);
	if (rootValue == NULL) {
		rootValue = json_value_init_object();
		json_serialize_to_file_pretty(rootValue, statsPath);
	}
	JSON_Object* highScore = json_value_get_object(rootValue);


	setColor(GRAY);
	gotoxy(slp+3, stp+3); printf("Music");
	gotoxy(slp+3+30, stp+3); printf("High Score!");
	setColor(DARK_GRAY);
	gotoxy(slp+2, stp+4); wprintf(L"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");

	for (int i = 0; i < mapCount; i++) {
		setColor(WHITE);
		gotoxy(slp+3, stp+5+i); puts(mapList[i]);
		setColor(GREEN);
		gotoxy(slp+3+30, stp+5+i); printf("%d", (int)json_object_get_number(highScore, mapList[i]));
	}

	return 0;
}



int selecting(int mapCount)
{
	int pointer = 0;
	setColor(WHITE);
	gotoxy(slp, stp+5+pointer); wprintf(L"->");


	while (GetAsyncKeyState(VK_RETURN) || GetAsyncKeyState(VK_SPACE));
	clearBuffer();

	int key;
	BOOL selected = FALSE;
	while (!selected) {

		key = _getch();
		if (key == 0xE0 || key == 0)
			key = _getch();

		switch (key) {

			
			case UP:
				gotoxy(slp, stp+5+pointer); wprintf(L"　");
				pointer--;
				if (pointer < 0)
					pointer = mapCount - 1;
				break;

			case DOWN:
				gotoxy(slp, stp+5+pointer); wprintf(L"　");
				pointer++;
				if (pointer >= mapCount)
					pointer = 0;
				break;

			case ENTER:
			case SPACE:
				gotoxy(slp, stp+5+pointer); wprintf(L" ▶");
				selected = TRUE;
				break;
		}

		if (!selected) {
			gotoxy(slp, stp+5+pointer); wprintf(L"->");
		}
	}

	return pointer;
}

void blink(int pointer) {
	gotoxy(slp+3, stp+5+pointer);
	_putch(' ');

	for (int i = 0; i < 2; i++) {
		gotoxy(slp+4, stp+5+pointer);
		for (int j = 0; j < strlen(mapList[pointer]); j++)
			_putch(' ');
		Sleep(150);

		gotoxy(slp+4, stp+5+pointer);
		printf("%s", mapList[pointer]);
		Sleep(150);
	}

	Sleep(300);
}