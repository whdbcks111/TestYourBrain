#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include "console.h"
#include "game.h"

int main();
void mainScreen(int selected);
void gameStart();
void rankingScreen();
void printInformation();
void lostLife();
void stageClear();
void gameOver();
void registInRanking(long long score);


int main() {
	srand(time(NULL));
	mainScreen(0);
	return 0;
}

void mainScreen(int selected) {
	//콘솔 기본설정
	setTitle(TITLE);
	cursorView(0);
	setConsoleSize(CONSOLE_WIDTH, CONSOLE_HEIGHT);
	setColor(0xf, 0x0);

	//게임 기본설정
	stage = 1;
	life = 3;
	turn = 0;
	visibleTime = 7;
	typeTime = 20;

	//화면 출력
	printCenter(-2, 0, TITLE, 0x6);

	char btn[10][50];
	int key, select = selected, max_select = 2;
	char str[50];
	strcpy(btn[0], "게임 시작");
	strcpy(btn[1], "랭킹 보기");
	strcpy(btn[2], "게임 종료");

	for (int i = 0; i <= max_select; i++) {
		sprintf(str, "    %s    ", btn[i]);
		printField(i, str, 0xf);
	}

	sprintf(str, "  > %s <  ", btn[select]);
	printField(select, str, 0xe);

	while (1) {
		if (_kbhit()) {
			cursorView(0);
			key = _getch();
			if (key == ARROW) {
				key = _getch();
				if (key == UP) {
					sprintf(str, "    %s    ", btn[select]);
					printField(select, str, 0xf);
					if (select == 0) select = max_select;
					else select--;
					sprintf(str, "  > %s <  ", btn[select]);
					printField(select, str, 0xe);
				}
				else if (key == DOWN) {
					sprintf(str, "    %s    ", btn[select]);
					printField(select, str, 0xf);
					if (select == max_select) select = 0;
					else select++;
					sprintf(str, "  > %s <  ", btn[select]);
					printField(select, str, 0xe);
				}
			}
			else if (key == ENTER) {
				switch (select) {
				case 0:
					gameStart();
					break;
				case 1:
					rankingScreen();
					break;
				case 2:
					exit(0);
					break;
				default:
					break;
				}
				return;
			}
		}
	}
}

void gameStart() {
	cls();
	score = 0;
	while (1) {
		printInformation();
		if (turn == 0) { // 차례 : 알파벳 순서 외우는 시간
			int colors[] = { 0xb, 0xf }, i;
			char str[10];
			for (i = 0; i < 5; i++) {
				keybind[i] = 'a' + rand() % ('z' - 'a' + 1);
			}
			for (int t = 0; t < (int)(visibleTime * 2); t++) {
				for (i = 0; i < 5; i++) {
					sprintf(str, "%d -> %c", i + 1, keybind[i]);
					printCenter(-4, i + 2, str, colors[t % 2]);
				}
				sprintf(str, "%d", (int)visibleTime - t / 2);
				printCenter(-4, i + 2, str, 0xc);
				Sleep(500);
			}
			for (i = 0; i <= 5; i++) {
				printCenter(-4, i + 2, "        ", 0x0);
			}
			for (i = 0; i < answerLen; i++) {
				answer[i] = '1' + rand() % 5;
			}
			answer[i] = '\0';
			printCenter(-2, 0, answer, 0xe);
			printCenter(-2, 1, "에 알맞는 알파벳을 입력하세요.", 0xf);
			printCenter(-2, 2, "(한번 입력 시 지우기 불가)", 0x8);
			typeStartTime = time(NULL);
			//제한시간이 끝나면 정답 입력 차례로 이동
			for (int i = 0; i < 11; i++) {
				input[i] = 0;
			}
			inputLen = 0;
			turn = 1;
			while (_kbhit()) { // 입력 차례 전에 입력된 불필요한 입력이 있으면 입력을 지움 
				_getch();
			}
			continue;
		}
		else if (turn == 1) { // 차례 : 정답 입력
			int t = ((int)typeTime) - (time(NULL) - typeStartTime); //남은 제한시간
			char tStr[6]; sprintf(tStr, " %d ", t);
			printCenter(-2, 3, tStr, 0xc);
			if (t <= 0) { //제한시간이 모두 끝났다면
				turn = 2; //게임 오버로 이동
				cls();
				continue;
			}
			if (_kbhit()) { //키보드를 입력했다면
				char key = _getch();
				if ('a' <= key && key <= 'z') { // 알파벳을 입력할 시
					input[inputLen++] = key; // 입력된 값을 저장하는 배열에 값 저장
				}
				input[inputLen] = '\0';
				printCenter(-1, 0, input, 0x7); //그리고 화면에 지금까지 입력된 값 출력
			}
			for (int i = 0; i < inputLen; i++) { //답 체크
				char k = input[i];
				int check = 0;
				for (int j = 0; j < 5; j++) {
					if (k == keybind[j] && answer[i] - '1' == j) { //k(입력한 글자)가 정해진 키에 포함되고, 순서 또한 같다면
						check = 1; // 이 글자 1개에 한해서 정답 처리
						break;
					}
				}
				if (!check) { //만약 이 글자가 틀렸다면
					turn = 2;
					break;
				}
			}
			if (turn == 2) { // 만약 지금 차례가 2라면 (입력한 답이 틀렸다면)
				cls();
				continue; //화면 지우고 반복의 시작으로 이동
			}
			else if(inputLen == answerLen) { //만약 그게 아니고 틀리지도 않았고 모두 입력했다면
				cls();
				turn = 4;
				continue; // 화면을 지운 후 '차례 : 스테이지 클리어'로 이동
			}
		}
		else if (turn == 2) { // 차례 : 틀림
			stageUp();
			lostLife();
		}
		else if (turn == 3) { // 차례 : 게임 오버
			gameOver();
			return;
		}
		else if (turn == 4) { // 차례 : 스테이지 클리어
			stageClear();
			stageUp();
		}
	}
}

void printInformation() { // 남은 생명과 스테이지 출력
	gotoxy(2, 1);
	for (int i = 0; i < 3; i++) {
		if (i < life) setColor(0xc, 0x0);
		else setColor(0x8, 0x0);
		printf("♥");
	}
	setColor(0xf, 0x0);
	printf("   %d 스테이지   %lld점", stage, score);
}

void registInRanking(long long score) {
	long long _ranking[RANKING] = { 0 };
	int addIndex = 0;
	for (int i = 0; i < RANKING; i++) {
		if ((i == 0 && ranking[0] < score)
			|| (i > 0 && ranking[i - 1] >= score && ranking[i] < score)
			|| (i == RANKING - 1 && ranking[RANKING - 1] >= score)) {
			_ranking[i] = score;
			addIndex = 1;
		}
		_ranking[i + addIndex] = ranking[i];
	}

	for (int i = 0; i < RANKING; i++) {
		ranking[i] = _ranking[i];
	}
}

void rankingScreen() {
	cls();
	printCenter(-2, 0, "점수 순위", 0x9);
	for (int i = 0; i < RANKING; i++) {
		char str[CONSOLE_WIDTH];
		sprintf(str, "%2d등  > %6lld점", i + 1, ranking[i]);
		printCenter(0, i, str, 0xf);
	}
	while (1) {
		if (_kbhit()) {
			if (_getch() == ENTER) {
				break;
			}
		}
	}
	mainScreen(1);
}

void lostLife() {
	score = (long long)(score * 0.9);
	printCenter(0, 0, "잘못 입력하여 생명이 1 깎였습니다..", 0xc);
	gotoxy(2, 1);
	life--;
	printInformation();
	if (life > 0) {
		Sleep(1500);
		cls();
		turn = 0;
		return;
	}
	else {
		Sleep(1000);
		cls();
		turn = 3;
		return;
	}
}

void stageClear() {
	cls();
	int t = ((int)typeTime) - (time(NULL) - typeStartTime);
	char str[40], asw[11] = { 0 };
	for (int i = 0; i < answerLen; i++) {
		asw[i] = keybind[answer[i] - '1'];
	}
	sprintf(str, "정답 : %s", asw);

	printCenter(-1, 0, "정답입니다!", 0xa);
	printCenter(-1, 2, str, 0x7);

	Sleep(1800);
	cls();
	turn = 0;
	score += (long long)stage * 161L + 1015L + (long long)t * 128L;
}

void gameOver() {
	cls();
	registInRanking(score);

	printCenter(-1, 0, "게임 오버!", 0x4);

	Sleep(1800);
	mainScreen(0);
}