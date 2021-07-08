#define CONSOLE_WIDTH 90
#define CONSOLE_HEIGHT 40
#define ARROW 224
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ENTER 13
#define TITLE "TEST YOUR BRAIN"
#define RANKING 10

int life = 3;
int turn = 0;
double visibleTime = 7;
double typeTime = 20;
int stage = 1;
char keybind[5] = { 0, };
char answer[20], input[21];
int answerLen = 4, inputLen = 0;
long long typeStartTime = 0;
long long ranking[RANKING] = { 0 };
long long score = 0;


void printCenter(int field, int line, char* str, int color);
void printField(int line, char* str, int color);


void stageUp() {
	stage++;
	//스테이지별 난이도 조정
	if (stage % 3 == 0) {
		if (visibleTime > 3) visibleTime -= 0.5; //스테이지 올라갈 때 마다 보여주는 시간 -
		if (typeTime > 6) typeTime += 0.5; //스테이지 올라갈 때 마다 입력 제한시간 +
	}
	if (stage % 6 == 3) {
		answerLen++; //스테이지 올라갈 때 마다 정답 길이 +
	}
	for (int i = 0; i < 5; i++) {
		keybind[i] = 'a' + rand() % ('z' - 'a' + 1);
	}
}

void printCenter(int field, int line, char* str, int color) {
	setColor(0xf, 0x0);
	int strLen;
	for (strLen = 0; str[strLen]; strLen++);
	gotoxy(CONSOLE_WIDTH / 2 - strLen / 2, CONSOLE_HEIGHT / 2 + field * 4 + line);
	setColor(color, 0x0);
	printf(str);
}

void printField(int line, char* str, int color) {
	setColor(0xf, 0x0);
	int strLen;
	for (strLen = 0; str[strLen]; strLen++);
	gotoxy(CONSOLE_WIDTH / 2 - strLen / 2 - 4, CONSOLE_HEIGHT / 2 + line * 4);
	printf("┌─");
	for (int i = 0; i < strLen; i++) {
		printf("─");
	}
	printf("┐ ");
	gotoxy(CONSOLE_WIDTH / 2 - strLen / 2 - 4, CONSOLE_HEIGHT / 2 + line * 4 + 1);
	printf("│ ");
	setColor(color, 0x0);
	printf(str);
	setColor(0xf, 0x0);
	printf("│ ");
	gotoxy(CONSOLE_WIDTH / 2 - strLen / 2 - 4, CONSOLE_HEIGHT / 2 + line * 4 + 2);
	printf("└─");
	for (int i = 0; i < strLen; i++) {
		printf("─");
	}
	printf("┘ ");
}

