#include <windows.h>
#include <stdio.h>

//Function to clear screen
void Clear(void)
{
    system("cls");
}

//Function that moves the cursor to that location
void GotoXY(short x, short y)
{
    COORD pos = { x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//Function to inform the position of the cursor X and Y
COORD GetXY(void)
{
    COORD pos;
    CONSOLE_SCREEN_BUFFER_INFO curInfor;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfor);
    pos.X = curInfor.dwCursorPosition.X;
    pos.Y = curInfor.dwCursorPosition.Y;

    return pos;
}
//Function to change the title name
void SetTitle(const char* _szConsoleName)
{
    SetConsoleTitle(_szConsoleName);
}

//Function to change the Pont color or background color
void SetColor(unsigned char _BgColor, unsigned char _TextColor)
{
    if(_BgColor > 15 || _TextColor > 15) return;

    unsigned short ColorNum= (_BgColor << 4) | _TextColor;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ColorNum);
}

//Functions that make the cursor visible or invisible
void SetCursor(BOOL _bShow)
{
    CONSOLE_CURSOR_INFO curInfor;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfor);
    curInfor.bVisible = _bShow;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfor);
}

void SetConsoleSize(int _col, int _lines)
{
    char setText[100];
    sprintf_s(setText, "mode con cols=%d lines=%d", _col, _lines);
    system(setText);
}

