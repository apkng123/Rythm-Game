

#include "fun.h"


void gotoxy(int x, int y) {
	static COORD pos = {0,};
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void movexy(int x, int y) {
	gotoxy(curX() + x, curY() + y);
}

void setCursor(enum CURSOR_TYPE curType) {
	static CONSOLE_CURSOR_INFO CurInfo = {0,};

	switch (curType) {
		case HIDE:
			CurInfo.dwSize = 1;
			CurInfo.bVisible = FALSE;
			break;
		case UNDERBAR:
			CurInfo.dwSize = 1;
			CurInfo.bVisible = TRUE;
			break;
		case BLOCK:
			CurInfo.dwSize = 100;
			CurInfo.bVisible = TRUE;
			break;
	}

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

int curX() {
	static CONSOLE_SCREEN_BUFFER_INFO curInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	return curInfo.dwCursorPosition.X;
}


int curY() {
	static CONSOLE_SCREEN_BUFFER_INFO curInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	return curInfo.dwCursorPosition.Y;
}

void clearBuffer() {
	while (_kbhit()) {
		int _ = _getch();
	}
}


void setColor(enum Colors color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
