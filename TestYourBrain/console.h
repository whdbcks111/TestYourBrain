#include <Windows.h>
#include <stdio.h>

void cursorView(char show)
{
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);
}

void gotoxy(int x, int y) {
	if (x < 0 || y < 0) return;
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void cls() {
	system("cls");
}

void setTitle(char* title) {
	char cmd[100] = "";
	sprintf(cmd, "title %s", title);
	system(cmd);
}

void setConsoleSize(int cols, int lines) {
	char cmd[100] = "";
	sprintf(cmd, "mode con cols=%d lines=%d", cols, lines);
	system(cmd);
}

void setColor(int text, int bg) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), bg * 16 + text);
}