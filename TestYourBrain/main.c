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
	//�ܼ� �⺻����
	setTitle(TITLE);
	cursorView(0);
	setConsoleSize(CONSOLE_WIDTH, CONSOLE_HEIGHT);
	setColor(0xf, 0x0);

	//���� �⺻����
	stage = 1;
	life = 3;
	turn = 0;
	visibleTime = 7;
	typeTime = 20;

	//ȭ�� ���
	printCenter(-2, 0, TITLE, 0x6);

	char btn[10][50];
	int key, select = selected, max_select = 2;
	char str[50];
	strcpy(btn[0], "���� ����");
	strcpy(btn[1], "��ŷ ����");
	strcpy(btn[2], "���� ����");

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
		if (turn == 0) { // ���� : ���ĺ� ���� �ܿ�� �ð�
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
			printCenter(-2, 1, "�� �˸´� ���ĺ��� �Է��ϼ���.", 0xf);
			printCenter(-2, 2, "(�ѹ� �Է� �� ����� �Ұ�)", 0x8);
			typeStartTime = time(NULL);
			//���ѽð��� ������ ���� �Է� ���ʷ� �̵�
			for (int i = 0; i < 11; i++) {
				input[i] = 0;
			}
			inputLen = 0;
			turn = 1;
			while (_kbhit()) { // �Է� ���� ���� �Էµ� ���ʿ��� �Է��� ������ �Է��� ���� 
				_getch();
			}
			continue;
		}
		else if (turn == 1) { // ���� : ���� �Է�
			int t = ((int)typeTime) - (time(NULL) - typeStartTime); //���� ���ѽð�
			char tStr[6]; sprintf(tStr, " %d ", t);
			printCenter(-2, 3, tStr, 0xc);
			if (t <= 0) { //���ѽð��� ��� �����ٸ�
				turn = 2; //���� ������ �̵�
				cls();
				continue;
			}
			if (_kbhit()) { //Ű���带 �Է��ߴٸ�
				char key = _getch();
				if ('a' <= key && key <= 'z') { // ���ĺ��� �Է��� ��
					input[inputLen++] = key; // �Էµ� ���� �����ϴ� �迭�� �� ����
				}
				input[inputLen] = '\0';
				printCenter(-1, 0, input, 0x7); //�׸��� ȭ�鿡 ���ݱ��� �Էµ� �� ���
			}
			for (int i = 0; i < inputLen; i++) { //�� üũ
				char k = input[i];
				int check = 0;
				for (int j = 0; j < 5; j++) {
					if (k == keybind[j] && answer[i] - '1' == j) { //k(�Է��� ����)�� ������ Ű�� ���Եǰ�, ���� ���� ���ٸ�
						check = 1; // �� ���� 1���� ���ؼ� ���� ó��
						break;
					}
				}
				if (!check) { //���� �� ���ڰ� Ʋ�ȴٸ�
					turn = 2;
					break;
				}
			}
			if (turn == 2) { // ���� ���� ���ʰ� 2��� (�Է��� ���� Ʋ�ȴٸ�)
				cls();
				continue; //ȭ�� ����� �ݺ��� �������� �̵�
			}
			else if(inputLen == answerLen) { //���� �װ� �ƴϰ� Ʋ������ �ʾҰ� ��� �Է��ߴٸ�
				cls();
				turn = 4;
				continue; // ȭ���� ���� �� '���� : �������� Ŭ����'�� �̵�
			}
		}
		else if (turn == 2) { // ���� : Ʋ��
			stageUp();
			lostLife();
		}
		else if (turn == 3) { // ���� : ���� ����
			gameOver();
			return;
		}
		else if (turn == 4) { // ���� : �������� Ŭ����
			stageClear();
			stageUp();
		}
	}
}

void printInformation() { // ���� ����� �������� ���
	gotoxy(2, 1);
	for (int i = 0; i < 3; i++) {
		if (i < life) setColor(0xc, 0x0);
		else setColor(0x8, 0x0);
		printf("��");
	}
	setColor(0xf, 0x0);
	printf("   %d ��������   %lld��", stage, score);
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
	printCenter(-2, 0, "���� ����", 0x9);
	for (int i = 0; i < RANKING; i++) {
		char str[CONSOLE_WIDTH];
		sprintf(str, "%2d��  > %6lld��", i + 1, ranking[i]);
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
	printCenter(0, 0, "�߸� �Է��Ͽ� ������ 1 �𿴽��ϴ�..", 0xc);
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
	sprintf(str, "���� : %s", asw);

	printCenter(-1, 0, "�����Դϴ�!", 0xa);
	printCenter(-1, 2, str, 0x7);

	Sleep(1800);
	cls();
	turn = 0;
	score += (long long)stage * 161L + 1015L + (long long)t * 128L;
}

void gameOver() {
	cls();
	registInRanking(score);

	printCenter(-1, 0, "���� ����!", 0x4);

	Sleep(1800);
	mainScreen(0);
}