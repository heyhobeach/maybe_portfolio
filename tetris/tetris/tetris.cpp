// tetris.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//예전에 c 로 구현했던 코드

#include "pch.h"
#include <iostream>



#include<stdio.h>
#include<stdlib.h>
#include<windows.h>		//커서의 위치를 조정하는 gotoxy()함수를 위해 필요
#include<conio.h>		//키보드 입력을 얻는 _getch() 포함
#include<time.h>		//Sleep 함수로 시간이 천천히 흐르도록 설정


//키보드의 아스키 코드 값 정리
#define LEFT 75			//방향키 왼쪽
#define RIGHT 77		//방향키 오른쪽
#define UP 72			//방향키 위쪽
#define DOWN 80			//방향키 아래쪽
#define SPACE 32		//스페이스바
#define ENTER 13		//엔터키
#define ESC 27			//ESC키

//참 거짓
#define FAUSE 0
#define TURE 1

//화면에 표시될 블럭
#define EMPTY 0			//빈공간
#define M_BLOCK 1		//움직이고 있는 블럭
#define S_BLOCK 2		//움직임이 완료된 블럭
#define CEILING 3		//테트리스의 천장
#define WALL 4			//벽

//게임 판의 크기
#define BOARD_WIDTH 12	//가로
#define BOARD_HEIGHT 24	//세로




struct Block {
	int b_type;		//블록 종류를 저장
	int b_rotation;	//블록 회전값 저장
	int b_next;		//다음 생성될 블록값 저장
	int bx;			//블록의 x 좌표를 저장
	int by;			//블록의 y 좌표를 저장
};

//테트리스에 사용되는 7가지 블럭들의 회전 상태에 따른 4가지 모습을 담은 4차원 배열, [블럭 종류], [블럭의 회전 상태], [y축], [x축]을 의미
int blocks[7][4][4][4] = {
	{
		{0,0,0,0,
		 0,1,1,0,
		 0,1,1,0,
		 0,0,0,0 },
		{0,0,0,0,
		 0,1,1,0,
		 0,1,1,0,
		 0,0,0,0 },
		{0,0,0,0,
		 0,1,1,0,
		 0,1,1,0,
		 0,0,0,0 },
		{0,0,0,0,
		 0,1,1,0,
		 0,1,1,0,
		 0,0,0,0 } },
	{
		{0,0,0,0,
		 0,0,0,0,
		 1,1,1,1,
		 0,0,0,0 },
		{0,1,0,0,
		 0,1,0,0,
		 0,1,0,0,
		 0,1,0,0 },
		{0,0,0,0,
		 0,0,0,0,
		 1,1,1,1,
		 0,0,0,0 },
		{0,1,0,0,
		 0,1,0,0,
		 0,1,0,0,
		 0,1,0,0 } },
	{
		{0,0,0,0,
		 1,1,0,0,
		 0,1,1,0,
		 0,0,0,0 },
		{0,0,0,0,
		 0,0,1,0,
		 0,1,1,0,
		 0,1,0,0 },
		{0,0,0,0,
		 1,1,0,0,
		 0,1,1,0,
		 0,0,0,0 },
		{0,0,0,0,
		 0,0,1,0,
		 0,1,1,0,
		 0,1,0,0 } },
	{
		{0,0,0,0,
		 0,1,1,0,
		 1,1,0,0,
		 0,0,0,0 },
		{0,0,0,0,
		 1,0,0,0,
		 1,1,0,0,
		 0,1,0,0 },
		{0,0,0,0,
		 0,1,1,0,
		 1,1,0,0,
		 0,0,0,0 },
		{0,0,0,0,
		 1,0,0,0,
		 1,1,0,0,
		 0,1,0,0 } },
	{
		{0,0,0,0,
		 0,0,1,0,
		 1,1,1,0,
		 0,0,0,0 },
		{0,0,0,0,
		 1,1,0,0,
		 0,1,0,0,
		 0,1,0,0 },
		{0,0,0,0,
		 0,0,0,0,
		 1,1,1,0,
		 1,0,0,0 },
		{0,0,0,0,
		 0,1,0,0,
		 0,1,0,0,
		 0,1,1,0 } },
	{
		{0,0,0,0,
		 1,0,0,0,
		 1,1,1,0,
		 0,0,0,0 },
		{0,0,0,0,
		 0,1,0,0,
		 0,1,0,0,
		 1,1,0,0 },
		{0,0,0,0,
		 0,0,0,0,
		 1,1,1,0,
		 0,0,1,0 },
		{0,0,0,0,
		 0,1,1,0,
		 0,1,0,0,
		 0,1,0,0 } },
	{
		{0,0,0,0,
		 0,1,0,0,
		 1,1,1,0,
		 0,0,0,0 },
		{0,0,0,0,
		 0,1,0,0,
		 0,1,1,0,
		 0,1,0,0 },
		{0,0,0,0,
		 0,0,0,0,
		 1,1,1,0,
		 0,1,0,0 },
		{0,0,0,0,
		 0,1,0,0,
		 1,1,0,0,
		 0,1,0,0 } }
};

//게임 판의 정보를 담은 2차원 배열
int board[BOARD_HEIGHT][BOARD_WIDTH];
int board_pre[BOARD_HEIGHT][BOARD_WIDTH];

int key = 0;			//키보드로 입력받은 값
int speed = 100;		//게임의 속도
int score = 0;			//지운 줄의 갯수
int score_up = 1;		//방금 스코어가 올랐는지 확인
int touch_flag = 0;		//바닥에 닿였는지 알려주는 플래그
int hard_drop_flag = 0;	//하드 드롭을 실행 했다는 플래그
int start = 1;			//재시작을 위한 플래그
int level = 1;

CONSOLE_CURSOR_INFO CursorInfo;


int title();																		//시작화면 출력
void reset();																		//게임 시작전 내용 초기화
void menu();																		//메뉴 선택 화면
void play();																		//게임 시작 화면
struct Block check_input(struct Block block);										//키보드 입력을 인식하는 함수

void draw_board(int next);															//게임판 배열을 기반으로 화면을 그림
void make_wall();																	//게임판 배열에 벽을 추가

struct Block init_block(struct Block block);										//시작시 구조체 Block의 내용을 초기화
struct Block new_block(struct Block block);											//블럭을 새로 생성

void insert_block(int x, int y, int rotation, int type, int derection, int rotate);	//게임판 배열에 블록을 삽입
void delete_block(int x, int y, int rotation, int type);							//게임판 배열에 블록을 삭제

int move_block(int x, int y, int rotation, int type, int derection, int rotate);	//블럭의 위치를 이동
int rotate_block(int x, int y, int rotation, int type);								//블럭을 회전
int drop_block(int x, int y, int rotation, int type, int derection, int rotate);	//블럭을 한칸 내림

int check_crash(int x, int y, int rotation, int type);								//벽이나 바닥에 부딫혔는지 파악
void check_line();																	//한줄이 완성되었는지 체크하고 완성되면 지우는 함수

int check_game_over();																//블럭이 천장을 넘어 게임 오버됐는지를 확인
void gotoxy(int x, int y);															//커서를 원하는 위치로 이동
int kb_input();																		//확장키의 아스키 코드값을 온전하게 받아 줌


/*****************************************************		메인문 시작		**********************************************************/

void main() {
	srand((unsigned)time(NULL));	//랜덤 블럭 생성을 위한 난수 생성

	//깜빡이는 커서 삭제
	CursorInfo.dwSize = 1;
	CursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CursorInfo);

	menu();
}

int title() {	//타이틀 화면 출력
	int menu = 0;	//어느 메뉴를 선택하고 있는지 표시 
	int x = 5;		//타이틀화면이 시작되는 x좌표
	int y = 4;		//타이틀화면이 시작되는 y좌표

	gotoxy(x, y + 0);		printf("■■■■■  ■■■■■  ■■■■■  ■■■■  ■■■  ■■■■■"); Sleep(100);
	gotoxy(x, y + 1);		printf("    ■      ■              ■      ■    ■    ■    ■        "); Sleep(100);
	gotoxy(x, y + 2);		printf("    ■      ■■■■■      ■      ■■■      ■    ■■■■■"); Sleep(100);
	gotoxy(x, y + 3);		printf("    ■      ■              ■      ■■        ■            ■"); Sleep(100);
	gotoxy(x, y + 4);		printf("    ■      ■■■■■      ■      ■  ■■  ■■■  ■■■■■"); Sleep(100);
	gotoxy(x + 11, y + 8);	printf("【　게　임　시　작  】");
	gotoxy(x + 11, y + 12);	printf("　　종　　　　　료　　");

	while (1)
	{
		if (_kbhit())
		{
			key = kb_input();

			if (key == UP && menu > 0)
			{
				switch (menu)								//나중에 새로운 메뉴 추가를 위해 스위치문으로 구성
				{
				case 0:
					gotoxy(x + 11, y + 8);
					printf("　　게　임　시　작　　");
					break;
				case 1:
					gotoxy(x + 11, y + 12);
					printf("　　종　　　　　료　　");
					break;
				}

				menu--;

				switch (menu)
				{
				case 0:
					gotoxy(x + 11, y + 8);
					printf("【　게　임　시　작　】");
					break;
				case 1:
					gotoxy(x + 11, y + 12);
					printf("【　종　　　　　료　】");
					break;
				}
			}
			else if (key == DOWN && menu < 1)
			{
				switch (menu)
				{
				case 0:
					gotoxy(x + 11, y + 8);
					printf("　　게　임　시　작　　");
					break;
				case 1:
					gotoxy(x + 11, y + 12);
					printf("　　종　　　　　료　　");
					break;
				}

				menu++;

				switch (menu)
				{
				case 0:
					gotoxy(x + 11, y + 8);
					printf("【　게　임　시　작　】");
					break;
				case 1:
					gotoxy(x + 11, y + 12);
					printf("【　종　　　　　료　】");
					break;
				}
			}
			else if (key == ENTER)
			{
				int trash;
				while (_kbhit())
					trash = _getch();	//혹시 추가적으로 버퍼에 값이 남아있으면 시작하자말자 입력되기에 버퍼를 비움
				return menu;
			}
		}
	}
}

void reset() {	//게임 정보를 리셋함
	int i, j;

	key = 0;
	touch_flag = 0;
	hard_drop_flag = 0;
	start = 1;
	score = 0;
	score_up = 1;
	speed = 100;
	level = 1;

	for (i = 0; i < BOARD_HEIGHT; i++)
		for (j = 0; j < BOARD_WIDTH; j++)
			board[i][j] = EMPTY;

	make_wall();
	for (i = 1; i < BOARD_WIDTH - 1; i++) {
		if (board[2][i] != M_BLOCK)
			board[2][i] = CEILING;
	}
	system("cls");
}

void menu() {	//메뉴 선택
	while (start) {
		switch (title()) {
		case 0:	//게임 시작
			play();
			break;
		case 1: //종료
			exit(0);
			break;
		}
	}
}

void play() {		//게임 실행 함수
	struct Block blockInfo = { 0,0,0,0,0 };

	int time_cont = 0;


	reset();
	draw_board(blockInfo.b_next);
	blockInfo = init_block(blockInfo);

	while (1) {
		if (time_cont != speed) {
			blockInfo = check_input(blockInfo);
			if (hard_drop_flag == 1) {
				while (touch_flag != 1)
					blockInfo.by = drop_block(blockInfo.bx, blockInfo.by, blockInfo.b_rotation, blockInfo.b_type, 0, 1);
				hard_drop_flag = 0;
			}
			draw_board(blockInfo.b_next);
			time_cont++;
			Sleep(1);
		}
		else {
			blockInfo.by = drop_block(blockInfo.bx, blockInfo.by, blockInfo.b_rotation, blockInfo.b_type, 0, 1);
			time_cont = 0;
		}
		if (score / 10 == level && score_up == 0) {
			speed /= 2;
			if (speed == 0)
				speed = 1;
			score_up = 1;
			level++;
		}
		if (touch_flag == 1) {
			blockInfo = new_block(blockInfo);
			touch_flag = 0;
		}
		if (check_game_over()) {
			system("cls");
			start = 1;
			break;
		}
	}
}

struct Block check_input(struct Block block) {	//키보드 입력 값을 분석하여 해당하는 작업 수행
	int type = block.b_type;
	int rotation = block.b_rotation;
	int x = block.bx;
	int y = block.by;

	if (_kbhit()) {
		switch (kb_input()) {
		case LEFT:
			if (check_crash(x - 1, y, rotation, type)) {
				block.bx = move_block(x, y, rotation, type, -1, 0);
			}
			break;
		case RIGHT:
			if (check_crash(x + 1, y, rotation, type)) {
				block.bx = move_block(x, y, rotation, type, 1, 0);
			}
			break;
		case UP:
			if (check_crash(x, y, rotation + 1, type)) {
				rotation = rotate_block(x, y, rotation, type);
				block.b_rotation = rotation;
			}
			break;
		case DOWN:
			block.by = drop_block(x, y, rotation, type, 0, 1);
			break;
		case SPACE:
			hard_drop_flag = 1;
			break;
		}
	}
	return block;
}

void draw_board(int next) {		//board 배열 및 부가정보를 화면에 출력
	int i, j;
	int x = 2;
	int y = 2;

	for (i = 1; i < BOARD_WIDTH - 1; i++) {
		if (board[2][i] != M_BLOCK)
			board[2][i] = CEILING;
	}

	for (i = 0; i < BOARD_HEIGHT; i++) {
		for (j = 0; j < BOARD_WIDTH; j++) {
			switch (board[i][j]) {
			case EMPTY:
				gotoxy(x + j, y + i);
				printf("  ");
				break;
			case S_BLOCK:
				gotoxy(x + j, y + i);
				printf("□");
				break;
			case M_BLOCK:
				gotoxy(x + j, y + i);
				printf("■");
				break;
			case CEILING:
				gotoxy(x + j, y + i);
				printf("..");
				break;
			case WALL:
				gotoxy(x + j, y + i);
				printf("▣");
				break;
			}
		}
	}

	gotoxy(x + BOARD_WIDTH + 1, 4);		printf("N E X T");
	gotoxy(x + BOARD_WIDTH + 7, 4);		printf("score : %d", score);
	gotoxy(x + BOARD_WIDTH + 7, 6);		printf("level : %d", level);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			gotoxy(x + BOARD_WIDTH + 1 + j, 5 + i);
			if (blocks[next][0][i][j] == 1)
				printf("■");
			else
				printf("  ");
		}
	}

}

void make_wall() {	//board 배열에 테트리스 벽을 삽입
	int i;

	for (i = 0; i < BOARD_HEIGHT; i++) {
		board[i][0] = WALL;
		board[i][BOARD_WIDTH - 1] = WALL;
	}
	for (i = 0; i < BOARD_WIDTH; i++)
		board[BOARD_HEIGHT - 1][i] = WALL;
}

struct Block init_block(struct Block block) {		//시작할때 블럭 정보를 생성함
	int i, j;

	block.b_type = rand() % 7;
	block.b_rotation = 0;
	block.b_next = rand() % 7;
	block.bx = BOARD_WIDTH / 2 - 2;
	block.by = 0;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			board[i][j + block.bx] = blocks[block.b_type][block.b_rotation][i][j];

	return block;
}

struct Block new_block(struct Block block) {		//새로운 블럭 생성
	int i, j;

	block.b_type = block.b_next;
	block.b_rotation = 0;
	block.b_next = 1;
	block.bx = BOARD_WIDTH / 2 - 2;
	block.by = 0;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			board[i][j + block.bx] = blocks[block.b_type][block.b_rotation][i][j];

	return block;
}

void delete_block(int x, int y, int rotation, int type) {		//board 배열의 블럭을 삭제
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (blocks[type][rotation][i][j] == 1)
				board[y + i][x + j] = EMPTY;
		}
	}
}

void insert_block(int x, int y, int rotation, int type, int derection, int rotate) {	//board 배열에 블럭을 삽입
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (blocks[type][rotation][i][j] == 1)
				board[y + i + rotate][x + j + derection] = M_BLOCK;
		}
	}
}

int move_block(int x, int y, int rotation, int type, int derection, int rotate) {	//블럭의 정보를 받아와 이동 시킴.

	delete_block(x, y, rotation, type);
	insert_block(x, y, rotation, type, derection, rotate);
	make_wall();

	if (derection) {
		if (derection == 1)
			return ++x;
		else
			return --x;
	}
	else if (rotate)
		return ++y;
}

int rotate_block(int x, int y, int rotation, int type) {	//블럭 회전

	delete_block(x, y, rotation, type);
	rotation = (rotation + 1) % 4;
	insert_block(x, y, rotation, type, 0, 0);

	return rotation;
}

int drop_block(int x, int y, int rotation, int type, int derection, int rotate) {	//블럭을 내릴 수 있으면 내리고, 아니면 S_BLOCK으로 굳힘
	int i, j;

	y = move_block(x, y, rotation, type, 0, 1);
	if (check_crash(x, y + 1, rotation, type))
		return y;
	else {
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (board[y + i][x + j] == 1)
					board[y + i][x + j] = S_BLOCK;
			}
		}
		check_line();
	}
}

int check_crash(int x, int y, int rotation, int type) {	//블럭의 x, y 좌표, 회전정보, 블럭 종류를 받아와 충돌하는지 확인
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (blocks[type][rotation][i][j] == M_BLOCK && ((board[y + i][x + j] == S_BLOCK) || (board[y + i][x + j] == WALL))) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

void check_line() {	//맞추는데 성공한 줄이 있는지 확인
	int i, j, k, l;
	int block_cont;

	for (i = BOARD_HEIGHT - 2; i > 3;) {
		block_cont = 0;
		for (j = 0; j < BOARD_WIDTH; j++) {
			if (board[i][j] == S_BLOCK)
				block_cont++;
		}
		if (block_cont == BOARD_WIDTH - 2) {
			for (k = i; k > 1; k--) { //윗줄을 한칸씩 모두 내림(윗줄이 천장이 아닌 경우에만) 
				for (l = 1; l < BOARD_WIDTH - 1; l++) {
					if (board[k - 1][l] != CEILING)
						board[k][l] = board[k - 1][l];
					if (board[k - 1][l] == CEILING)
						board[k][l] = EMPTY;
					//윗줄이 천장인 경우에는 천장을 한칸 내리면 안되니까 빈칸을 넣음 
				}
			}
			score++;
			if (score_up == 1)
				score_up = 0;
		}
		else
			i--;
	}
	touch_flag = 1;		//움직이던 블럭이 바닥에 부딫혔다.
}

int check_game_over() { //잠시 동안 game over화면 보여주고 시작화면으로 돌아감
	int i;

	for (i = 1; i < BOARD_WIDTH - 2; i++) {
		if (board[4][i] == S_BLOCK) {
			gotoxy(15, 15);	printf("===================================");
			gotoxy(15, 16);	printf("                                   ");
			gotoxy(15, 17);	printf("                                   ");
			gotoxy(15, 18);	printf("       g a m e       o v e r       ");
			gotoxy(15, 19);	printf("                                   ");
			gotoxy(15, 20);	printf("                                   ");
			gotoxy(15, 21);	printf("===================================");
			Sleep(1000);
			return TURE;
		}
	}
	return FALSE;
}

void gotoxy(int x, int y) { //콘솔의 좌상단 기준으로 x, y만큼 축 이동하여 커서를 위치시킴
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int kb_input() {
	int ch = _getch();

	//방향키의 아스키 코드 값은 224를 리턴한 다음에 각각 72 75 77 80을 반환함. 이를 받아주는 함수
	if (ch == 224) {
		ch = _getch();
	}
	return ch;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
