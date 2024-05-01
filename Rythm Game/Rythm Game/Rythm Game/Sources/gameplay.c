

#include "gameplay.h"



void main_gameplay(char* _mapName) {
	mapName = _mapName;

	init();

	if (readMap() == -1) {
		system("cls");
		gotoxy(1, 1);
		puts("맵 파일이 존재하지 않거나 맵을 열 수 없습니다.");
		Sleep(1000);
		return;
	}

	initBgm();
	drawScreen();
	countdown();
	updateUI(0);

	while (!gameEnd && !quit) {
		removingJudgeTxt();
		keyInput();
		fallingNote();
		
	}

	clearBuffer();
	if (!quit) {
		showStats();
		while (!_kbhit()) removingJudgeTxt();
	}

	playBgm(3);
	free(mapDir);
	for (int i = 0; i < mapLength; i++) free(map[i]); free(map);
}



void init()
{
	system("cls");
	gotoxy(glp + LINE*NOTETHK/2 - 5, gtp + HEI / 2 - 1);
	printf("Loading...");

	mapIndex = 0;
	paused = FALSE;
	songPlayed = FALSE;
	gameEnd = FALSE;
	quit = FALSE;
	score = 0;
	combo = 0;
	accuracy = 100;
	noteCount = 0;
	pauseTimer = 0;
	memset(note, x, sizeof(note));
	memset(shouldRemove, FALSE, LINE);
	memset(isPressed, FALSE, LINE);
}


int readMap() {
	FILE *f;

	
	const int mapDirSize = (int)strlen(mapFolder) + 1 + (int)strlen(mapName) + 1 + 1;
	mapDir = malloc(mapDirSize);
	sprintf_s(mapDir, mapDirSize, "%s/%s/", mapFolder, mapName);


	const char infoStr[] = "info.json";
	const int infoPathSize = mapDirSize + (int)strlen(infoStr);
	char* infoPath = malloc(infoPathSize);
	sprintf_s(infoPath, infoPathSize, "%s%s", mapDir, infoStr);


	JSON_Value *jsonValue = json_parse_file(infoPath);
	if (jsonValue == NULL) return -1;
	mapInfo = json_value_get_object(jsonValue);

	const char* noteFile = json_object_get_string(mapInfo, "mapFile");
	const int notePathSize = mapDirSize + (int)strlen(noteFile);
	char* notePath = malloc(notePathSize);
	if (notePath == NULL) return -1;
	sprintf_s(notePath, notePathSize, "%s%s", mapDir, noteFile);


	mapLength = (int)json_object_get_number(mapInfo, "mapLength");
	FALLSPEED = (int)json_object_get_number(mapInfo, "fallSpeed");

	if (FALLSPEED == 0) { 
		const double bpm = (int)json_object_get_number(mapInfo, "bpm");
		if (bpm == 0)
			FALLSPEED = 150;
		else
			FALLSPEED = (int)round(60 / bpm / 4 * 1000);
	}


	map = malloc(mapLength * sizeof(char*));
	if (map == NULL) return -1;
	fopen_s(&f, notePath, "r");
	if (f == NULL) return -1;

	char* line;
	for (int i = 0; i < mapLength; i++) {
		line = malloc(LINE * sizeof(char));
		if (line == NULL) return -1;
		for (int j = 0; j < LINE; j++) {
			line[j] = fgetc(f);
		}
		map[i] = line;

		fgetc(f); 
	}

	
	fclose(f);
	free(infoPath); free(notePath);
	return 0;
}


void initBgm() {
	
	const char* bgmName = json_object_get_string(mapInfo, "songFile");
	if (bgmName == NULL) bgmName = "";
	const int pathSize = (int)strlen(mapDir) + (int)strlen(bgmName) + 1;
	char* bgmPath = malloc(pathSize);
	if (bgmPath == NULL) return;
	sprintf_s(bgmPath, pathSize, "%s%s", mapDir, bgmName);

	
	const int bgmPathWSize = MultiByteToWideChar(CP_ACP, 0, bgmPath, -1, NULL, 0);
	wchar_t* bgmPathW = malloc(bgmPathWSize * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, bgmPath, (int)strlen(bgmPath)+1, bgmPathW, bgmPathWSize);

	openBgm.lpstrElementName = bgmPathW;
	openBgm.lpstrDeviceType = L"mpegvideo";
	mciSendCommandW(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD_PTR)(LPVOID)&openBgm); 
	dwID = openBgm.wDeviceID;
	mciSendCommandW(dwID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)(LPVOID)&openBgm);
	playBgm(1); 

	free(bgmPath);
}


void drawScreen() {

	
	setColor(GRAY);
	for (int i = 0; i <= HEI; i++) {
		gotoxy(glp - 1, gtp + i);

		_putch('|');

		for (int j = 0; j < LINE * NOTETHK; j++) {
			if (i == HEI-2) {
				setColor(WHITE);
				wprintf(L"□"); j++; 
				setColor(GRAY);
			}
			else if (i == HEI) {
				_putch('^');
			}
			else {
				_putch(' '); 
			}
		}

		_putch('|');
	}


	setColor(SKYBLUE);
	for (int i = 0; i < LINE; i++) {
		gotoxy(glp + i * NOTETHK + 1, gtp + HEI + 2);
		wprintf(L"%s", keyName[i]);
	}


	setColor(YELLOW);
	gotoxy(glp - 6 - (int)strlen(mapName), gtp + 1);
	printf("< %s >", mapName);
}

void countdown() {
	for (int i = 3; i >= 1; i--) {
		gotoxy(glp + LINE*NOTETHK/2 - 1, gtp + HEI/2-1); 
		wprintf(L"%c", i + '0' + 0xFEE0);
		Sleep(500);
	}

	gotoxy(glp + LINE*NOTETHK/2 - 3, gtp + HEI / 2 - 1);
	printf("Start!");
	Sleep(500);


	gotoxy(glp + 1, gtp + HEI + 2);
	for (int i = 0; i < LINE * NOTETHK; i++) {
		_putch(' ');
	}

	clearBuffer();
}


void fallingNote() {
	static clock_t timer = 0;
	if (timer == 0) timer = clock();
	static clock_t runtime = 0;
	if (runtime == 0) runtime = FALLSPEED;

	
	static clock_t endTimer = 0;
	static BOOL end = FALSE;
	
	if (quit) {
		timer = 0;
		runtime = 0;
		return;
	}

	if (clock() - (timer + pauseTimer) >= runtime) {

		
		for (int i = 0; i < LINE; i++) {
			if (note[HEI-1][i] == N) {
				hitNote(i, -1);
			}
		}


		for (int i = 1; i < HEI; i++) {
			for (int j = 0; j < LINE; j++) {
				note[HEI-i][j] = note[HEI-(i+1)][j];

				
				if (!songPlayed && i == 2 + json_object_get_number(mapInfo, "offset") && note[HEI-i][j] == N) {
					playBgm(0);
					songPlayed = TRUE;
				}
			}
		}


		if (mapIndex < mapLength) {
			for (int i = 0; i < LINE; i++) {
				note[0][i] = map[mapIndex][i];
			}
			mapIndex++;

			
			if (mapIndex == mapLength) {
				end = TRUE;
				endTimer = clock();
			}
		}

	
		else {
			for (int i = 0; i < LINE; i++) {
				note[0][i] = x;
			}
		}

	
		showNotes();

		runtime += FALLSPEED;
	}

	
	if (end) {
		if (clock() - (endTimer + pauseTimer) >= FALLSPEED * HEI + 1000) {
			timer = 0;
			runtime = 0;
			gameEnd = TRUE;
			end = FALSE;
		}
	}
}


void showNotes() {

	for (int i = 0; i < HEI; i++) {
		gotoxy(glp, gtp + i);

		for (int j = 0; j < LINE; j++) {
			for (int k = 0; k < NOTETHK / 2; k++) {

				if (note[i][j] == N) {
					setColor(GREEN);
					wprintf(L"■");
				}
				
				else {
					if (i == HEI-2) { 
						setColor(WHITE);
						wprintf(L"%c", isPressed[j] ? L'▣' : L'□');
					}
					
					else { 
						wprintf(L"　");
					}
				}
			}
		}
	}
}


void keyInput() {

	if (_kbhit()) {

		
		for (int i = 0; i < LINE; i++) {
			if (GetAsyncKeyState(key[i])) {
				if (!isPressed[i]) { 

					press(i);
					isPressed[i] = TRUE;

					setColor(WHITE);
					gotoxy(glp + i * NOTETHK, gtp + HEI - 2);
					for (int j = 0; j < NOTETHK / 2; j++)
						wprintf(L"▣");
				}
			}

			else if (isPressed[i]) { 

				setColor(WHITE);
				gotoxy(glp + i * NOTETHK, gtp + HEI - 2);
				for (int j = 0; j < NOTETHK / 2; j++)
					if (note[HEI-2][i] == x)
						wprintf(L"□");

				isPressed[i] = FALSE;
			}
		}

	
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			pause();
		}
	}
}


void press(int line) {
	for (int i = 1; i <= 4; i++) {
		if (note[HEI - i][line] == N) {
			hitNote(line, i);
			return;
		}
	}
}

void hitNote(int line, int judgement) {

	
	if (judgement != -1) note[HEI-judgement][line] = x;
	noteCount++;


	
	switch (judgement) {
		case 1: 
		case 3: 
			score += 200 + (int)round(200 * combo / 300.0);
			accuracy = (accuracy * (noteCount-1) + 200/3.0) / noteCount;
			updateUI(+1);
			break;
		case 2: 
			score += 300 + (int)round(300 * combo / 200.0);
			accuracy = (accuracy * (noteCount-1) + 100) / noteCount;
			updateUI(+1);
			break;
		case 4:
		case -1: 
			score += 0;
			accuracy = (accuracy * (noteCount - 1) + 0) / noteCount;
			updateUI(-1);
			break;
	}



	gotoxy(glp + line*NOTETHK+1, gtp + HEI+1);
	switch (judgement) {
		case 1:          setColor(DARK_YELLOW); puts("LATE"); break;
		case 2:          setColor(DARK_GREEN);  puts("GOOD"); break;
		case 3:          setColor(DARK_YELLOW); puts("FAST"); break;
		case 4: case -1: setColor(DARK_GRAY); puts("miss"); break;
	}
	shouldRemove[line] = 1;


	
	showNotes();
}


void removingJudgeTxt() {
	static clock_t timer[LINE] = {0,};

	for (int i = 0; i < LINE; i++) {
		if (shouldRemove[i] == 1) {
			timer[i] = clock();
			shouldRemove[i] = 2;
		}

		if (shouldRemove[i] == 2 && clock() - timer[i] >= 1000) {
			gotoxy(glp + i*NOTETHK+1, gtp + HEI+1);
			puts("    ");
			shouldRemove[i] = 0;
		}
	}
	
}



void pause() {
	
	if (!paused)  {
		paused = TRUE;
		clock_t pauseStart = clock();
		playBgm(1);

		
		drawScreen();
		setColor(GRAY);
		gotoxy(glp + LINE*NOTETHK/2 - 3, gtp + HEI/2-1); puts("Paused");
		gotoxy(glp, gtp + HEI/2+1); puts("ESC를 눌러서 계속 플레이");
		gotoxy(glp, gtp + HEI/2+2); puts("Q를 눌러서 맵 나가기");

		
		while (GetAsyncKeyState(VK_ESCAPE)) removingJudgeTxt();
		while (!GetAsyncKeyState(VK_ESCAPE)) {
			if (GetAsyncKeyState('Q')) {
				quit = TRUE;
				return;
			}
			removingJudgeTxt();
		}
		
		paused = FALSE;
		drawScreen();
		countdown();
		playBgm(2);
		pauseTimer += clock() - pauseStart;
	}
}



void playBgm(int action) {

	switch (action) {
		case 0:
			mciSendCommandW(dwID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)(LPVOID)&openBgm);
			break;

		case 1: 
			mciSendCommandW(dwID, MCI_PAUSE, MCI_NOTIFY, (DWORD_PTR)(LPVOID)&openBgm);
			break;

		case 2: 
			mciSendCommandW(dwID, MCI_RESUME, MCI_NOTIFY, (DWORD_PTR)(LPVOID)&openBgm);
			break;

		case 3: 
			mciSendCommandW(dwID, MCI_STOP, MCI_NOTIFY, (DWORD_PTR)(LPVOID)&openBgm); 
			mciSendCommandW(dwID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD_PTR)(LPVOID)NULL); 
			break;
	}
}

void updateUI(int comboPlus) {

	
	gotoxy(glp + LINE*NOTETHK + 2, gtp+1);
	setColor(GRAY);  printf("Score: ");
	setColor(GREEN); printf("%d", score);


	if (comboPlus == 1)
		combo++;
	else if (comboPlus == -1)
		combo = 0;
	gotoxy(glp + LINE*NOTETHK + 2, gtp+3);
	setColor(GRAY);  printf("Combo: ");
	setColor(WHITE); printf("%-4d", combo);

	gotoxy(glp + LINE*NOTETHK + 2, gtp+5);
	setColor(GRAY);   printf("Accuracy: ");
	setColor(YELLOW); printf("%-3.3f%% ", accuracy);
}



void showStats() {
	gotoxy(glp + 1, gtp + HEI/2-3);
	setColor(YELLOW); printf("%s", mapName);
	setColor(GRAY);   printf(" 플레이 결과");
	gotoxy(glp + 1, gtp + HEI/2-1);
	setColor(GRAY);  printf("점수: ");
	setColor(GREEN); printf("%d", score);
	gotoxy(glp + 1, gtp + HEI/2);
	setColor(GRAY);   printf("정확도: ");
	setColor(YELLOW); printf("%.3f%%", accuracy);

	setColor(GRAY);
	gotoxy(glp + 1, gtp + HEI/2+2);
	printf("메인 화면으로 돌아가려면");
	gotoxy(glp + 1, gtp + HEI/2+3);
	printf("아무 키나 누르세요");

	
	JSON_Value *rootValue = json_parse_file(statsPath);
	JSON_Object *highScore = json_value_get_object(rootValue);
	if (json_object_get_number(highScore, mapName) < score) {
		json_object_set_number(highScore, mapName, score);
		json_serialize_to_file_pretty(rootValue, statsPath);
	}
}
