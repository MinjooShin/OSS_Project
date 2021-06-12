#include <windows.h>

void Clear(void);

void GotoXY(short x, short y);

COORD GetXY(void);

void SetTitle(const char* szConsoleName);

void SetColor(unsigned char _BgColor, unsigned char _TextColor);

void SetCursor(BOOL _bShow);

void SetConsoleSize(int _col, int _lines);