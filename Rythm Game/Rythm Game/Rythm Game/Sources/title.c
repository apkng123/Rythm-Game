#include "title.h"



void main_title() {

	system("cls");

	setColor(BLUE);
	for (int i = 0; i < Hei; i++) {
		gotoxy(SCREEN_WIDTH/2 - Wid/2, 4+i);
		puts(titlename[i]);
	}
	setColor(RED);
	const wchar_t title[] = L"이시온";
	gotoxy((SCREEN_WIDTH - (int)wcslen(title)*2) / 2, 4+Hei+1);
	wprintf(L"%s", title);

	setColor(GRAY);
	const char anykey[] = "아무 버튼이나 눌러주세요.";
	gotoxy(SCREEN_WIDTH/2 - (int)strlen(anykey)/2, 4+Hei+4);
	puts(anykey);

	gotoxy(SCREEN_WIDTH/2, 4+Hei+5);
	setCursor(UNDERBAR);
	clearBuffer();

	int _ = _getch();
}
