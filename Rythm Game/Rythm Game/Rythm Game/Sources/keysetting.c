#include "keysetting.h"


void main_keysetting() {
    const int klp = 8; 
    const int ktp = 4;
    const int gap = 6;
    const int boxWid = LINE * gap - 1; 

    const int fixedKeys[] = { 'D', 'F', 'J', 'K' };
    const wchar_t* fixedKeyNames[] = { L" D ", L" F ", L" J ", L" K " };

#define LINE (sizeof(fixedKeys) / sizeof(fixedKeys[0]))

    for (int i = 0; i < LINE; i++) {
        key[i] = fixedKeys[i];
        wcscpy_s(keyName[i], sizeof(keyName[i]) / sizeof(wchar_t), fixedKeyNames[i]);
    }

    setCursor(BLOCK);
    setColor(WHITE);
    gotoxy(klp + (boxWid - 7) / 2 - 1, ktp);
    puts("키 설정");

    
    for (int i = 0; i < LINE; i++) {
       
        gotoxy(klp + i * gap, ktp + 2); wprintf(L"▽");

        gotoxy(klp + i * gap - 1, ktp + 4);
        setColor(SKYBLUE);
        wprintf(L"%s", keyName[i]);

        setColor(WHITE);
        gotoxy(klp + i * gap, ktp + 2); wprintf(L"▼");
        gotoxy(klp + i * gap, ktp + 6); wprintf(L"↑");
    }
}
