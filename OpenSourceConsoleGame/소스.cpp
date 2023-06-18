#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include "blockModel.h"
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

/*Size of Gameboard*/
#define GBOARD_HEIGHT 20
#define GBOARD_WIDTH 10

/*Size of Gameboard*/
#define GBOARD_ORIGIN_Y 2
#define GBOARD_ORIGIN_X 4

int block_id;
int block_spin;
int curPosX, curPosY;
int speed = 30;

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
				printf("¡á");
		}
		SetCurrentCursorPos(pos.X, pos.Y);
	}
}

void deleteBlock(int blockInfo[4][4])
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

void ShiftLeft()
{
	deleteBlock(blockModel[block_id][block_spin]);
	curPosX -= 2;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id][block_spin]);
}

void ShiftRight()
{
	deleteBlock(blockModel[block_id][block_spin]);
	curPosX += 2;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id][block_spin]);
}

void BlockDown()
{
	deleteBlock(blockModel[block_id][block_spin]);
	curPosY += 1;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id][block_spin]);
}

void BlockUp()
{
	deleteBlock(blockModel[block_id][block_spin]);
	curPosY -= 1;
	SetCurrentCursorPos(curPosX, curPosY);
	showBlock(blockModel[block_id][block_spin]);
}

void RotateBlock()
{
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
		if (_kbhit() != 0)
		{
			key = _getch();

			switch (key)
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
			printf("¦¦");
		else
			printf("¦¢");
	}

	for (x = 1; x <= GBOARD_WIDTH + 1; x++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + (x * 2), GBOARD_ORIGIN_Y + GBOARD_HEIGHT);
		if (x == GBOARD_WIDTH + 1)
			printf("¦¥");
		else
			printf("¦¡¦¡");
	}

	for (y = 0; y < GBOARD_HEIGHT; y++)
	{
		SetCurrentCursorPos(GBOARD_ORIGIN_X + ((GBOARD_WIDTH) * 2) + 2, GBOARD_ORIGIN_Y + y);
		printf("¦¢");
	}
}

int main()
{
	block_spin = 0;
	srand(time(NULL));
	block_id = rand() % 7;
	curPosX = curPosY = 0;
	system("mode con: cols=60 lines=30");


	RemoveCursor();

	drawBoard();

	while (1)
	{
		BlockDown();
		ProcessKeyInput();
	}

	getchar();
	return 0;
}
