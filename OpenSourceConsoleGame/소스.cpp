#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include "blockModel.h"
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACE 32
/*Size of Gameboard*/
#define GBOARD_HEIGHT 20
#define GBOARD_WIDTH 10

/*Size of Gameboard*/
#define GBOARD_ORIGIN_Y 2
#define GBOARD_ORIGIN_X 4

int GameBoard[GBOARD_HEIGHT + 1][GBOARD_WIDTH + 2];
int block_id;
int block_spin;
int curPosX, curPosY;
int speed = 15;
int score;


void SetCurrentCursorPos(int x, int y)
{
	COORD pos = { x, y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

COORD GetCurrentCursorPos()
{
	CONSOLE_SCREEN_BUFFER_INFO	curInfo;
	COORD curPoint;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}

void RemoveCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void showBlock(int blockInfo[4][4])
{
	COORD pos = GetCurrentCursorPos();

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			SetCurrentCursorPos(pos.X + x * 2, pos.Y + y);
			if (blockInfo[y][x] != 0)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), blockInfo[y][x]);
				printf("■");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
		}
		SetCurrentCursorPos(pos.X, pos.Y);
	}
}

void deleteBlock(int blockInfo[4][4])//블록을 지움
{
	COORD pos = GetCurrentCursorPos();

	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			SetCurrentCursorPos(pos.X + x * 2, pos.Y + y);
			if (blockInfo[y][x] != 0)
				printf("  ");
		}
		SetCurrentCursorPos(pos.X, pos.Y);
	}
}

bool IsCollision(int nextX, int nextY, int nextR)// 블록의 충돌 감지
{
	nextX = (nextX - GBOARD_ORIGIN_X) / 2;
	nextY = (nextY - GBOARD_ORIGIN_Y);

	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
		{
			if (GameBoard[nextY + y][nextX + x] != 0 && blockModel[block_id][nextR][y][x] != 0)
				return true;
		}
	return false;
}

void FillBoard()//배열보드를 블록으로 채움
{
	int arrX = (curPosX - GBOARD_ORIGIN_X) / 2;
	int arrY = (curPosY - GBOARD_ORIGIN_Y);

	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
		{
			if(GameBoard[arrY + y][arrX + x] == 0)
				GameBoard[arrY + y][arrX + x] = blockModel[block_id][block_spin][y][x];
		}
}

void ShiftLeft()//왼쪽으로 이동
{
	if (IsCollision(curPosX - 2, curPosY, block_spin))
		return;
	deleteBlock(blockModel[block_id][block_spin]);
	curPosX -= 2;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id][block_spin]);
}

void ShiftRight()//오른쪽으로 이동
{
	if (IsCollision(curPosX + 2, curPosY, block_spin))
		return;
	deleteBlock(blockModel[block_id][block_spin]);
	curPosX += 2;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id][block_spin]);
}

void BlockDown()//블록 떨어짐
{
	deleteBlock(blockModel[block_id][block_spin]);
	curPosY += 1;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id][block_spin]);
}

void BlockFastDown()//블록 빨리 떨어짐
{
	if (IsCollision(curPosX, curPosY + 1, block_spin))
		return;
	deleteBlock(blockModel[block_id][block_spin]);
	curPosY += 1;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id][block_spin]);
}

void BlockUp()//블록 위로
{
	deleteBlock(blockModel[block_id][block_spin]);
	curPosY -= 1;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id][block_spin]);
}

void BlockDrop()//블록을 가장 아래로 떨굼
{
	deleteBlock(blockModel[block_id][block_spin]);
	while (IsCollision(curPosX, curPosY + 1, block_spin) != true)
		curPosY += 1;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id][block_spin]);
}

void RotateBlock()//블록을 시계방향으로 회전
{
	if (IsCollision(curPosX, curPosY, block_spin + 1))
		return;
	deleteBlock(blockModel[block_id][block_spin]);
	block_spin = (block_spin + 1) % 4;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id][block_spin]);
}

void ProcessKeyInput()
{

	for (int i = 0; i < 20; i++)
	{
		int key;
		if (_kbhit() != 0) //키입력 있을시
		{
			key = _getch();

			switch (key)//키코드에 따라 행동
			{
			case LEFT:
				ShiftLeft();
				break;
			case RIGHT:
				ShiftRight();
				break;
			case UP:
				RotateBlock();
				break;
			case DOWN:
				BlockFastDown();
				break;
			case SPACE:
				BlockDrop();
				break;
			}
		}
		Sleep(speed);
	}
}

void drawBoard()
{
	int x, y;
	for (y = 0; y <= GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X, GBOARD_ORIGIN_Y + y);
		if (y == GBOARD_HEIGHT)
			printf("└");
		else
			printf("│");
	}

	for (x = 1; x <= GBOARD_WIDTH + 1; x++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (x * 2), GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
		if (x == GBOARD_WIDTH + 1)
			printf("┘");
		else
			printf("──");
	}

	for (y = 0; y < GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + ((GBOARD_WIDTH) * 2) + 2, GBOARD_ORIGIN_Y + y);
		printf("│");
	}
}

void ReDraw() //이차원 배열보고 게임판 그려줌
{
	for (int y = 0; y < GBOARD_HEIGHT; y++)
	{
		for (int x = 1; x < GBOARD_WIDTH+1; x++)
		{
			SetCurrentCursorPos(GBOARD_ORIGIN_X + (x * 2), GBOARD_ORIGIN_Y + y);
			if (GameBoard[y][x] == 0)
			{
				printf("  ");
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GameBoard[y][x]);
				printf("■");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
		}
	}
}

void completeLine() //한줄 완성시 제거하고 위에꺼 내림
{
	for (int y = GBOARD_HEIGHT - 1; y > 0; y--)
	{
		bool complete = true;
		for (int x = 1; x < GBOARD_WIDTH + 1; x++)
		{
			if (GameBoard[y][x] == 0)
			{
				complete = false;
				break;
			}
			
		}
		if (complete)
		{
			for (int i = y; i > 0; i--)
				memcpy(&GameBoard[i][1], &GameBoard[i - 1][1], 10 * sizeof(int));
			y++;
			score += 10;
		}

	}
}

COORD GetArrPos()// 현재 배열위치 반환
{
	COORD cur = GetCurrentCursorPos();
	COORD arr;
	arr.X = (cur.X - GBOARD_ORIGIN_X) / 2;
	arr.Y = cur.Y - GBOARD_ORIGIN_Y;

	return arr;
}



void initGameBoard()//게임보드 초기화
{
	for(int y=0;y<GBOARD_HEIGHT + 1; y++)
		for (int x = 0; x < GBOARD_WIDTH + 2; x++)
		{
			if (x == 0 || x == GBOARD_WIDTH + 1 || y == GBOARD_HEIGHT)
				GameBoard[y][x] = 1;
			else
				GameBoard[y][x] = 0;
		}
}
void printScore()//점수 출력
{
	SetCurrentCursorPos(30, 5);
	printf("[Score : %d]", score);
}

void gameover()//게임오버 띄움
{
	SetCurrentCursorPos(24, 11);
	printf("Game Over!");
	SetCurrentCursorPos(22, 14);
	printf("[Your Score : %d]", score);
}

int main()
{
	initGameBoard();
	block_spin = 0;
	srand(time(NULL));
	system("mode con: cols=60 lines=30");
	//각종 설정 초기화
	score = 0;
	RemoveCursor(); //커서제거
	printScore(); // 점수판 출력
	drawBoard(); //보드 경계 그리기

	while (1)
	{
		block_id = rand() % 7;//랜덤 블록 생성
		curPosX = 12;
		curPosY = 0;
		SetCurrentCursorPos(curPosX, curPosY);
		if (IsCollision(curPosX, curPosY, block_spin)) //시작위치에 이미 블록 존재시 게임오버
			break;

		while (IsCollision(curPosX, curPosY + 1, block_spin) == false)//아래에 충돌할때까지 반복
		{
			BlockDown();//블록 내림
			ProcessKeyInput(); //키보드 입력 받음
		}
		FillBoard(); //배열보드를 채움
		completeLine(); //한줄 완성시 제거
		ReDraw(); //보드판 다시그림
		printScore();//점수 출력
	}

	system("cls"); //화면 비움
	gameover();//게임오버 출력
	getchar();
	return 0;
}
