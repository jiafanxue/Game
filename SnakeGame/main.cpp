#include <Windows.h>
#include <string>
#include <time.h>

#include "table.h"
#include "resource.h"

using namespace std;

// ����������
const string ProgramTitle = "Snake";


// ��������
// �ص�����
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
// ���ƺ���
void DrawGame();
// ǽ�ڻ���
void WallDraw(HDC hdc);
// ��Ϸ������
void GameAreaDraw(HDC hdc);

// ��ʱ������
void OnTimer(UINT Timer_id);
void StartGame();
void EndGame();

bool checkOrNot();

// ȫ�ֱ���
Table table;

int iScores = 0;
int tableBlockWidth = 0;			// ���Ӹ��ӿ��
int tableBlockHeight = 0;			// ���Ӹ��Ӹ߶�

UINT uGameState = GAME_STATE_WAIT;	// ��Ϸ��ǰ״̬

// �����豸
HDC windowDC = NULL;				// ��������Ļ�豸
HDC bufferDC = NULL;				// ���ֻ����豸
HDC picDC = NULL;					// ��ϷͼƬ�豸
HDC endDC = NULL;					// ����ͼ���豸
HDC scoreDC = NULL;					// ����ͼ���豸

HWND hAppWnd = NULL;				// ��ǰ���ھ��

HBITMAP bufferBMP = NULL;
HBITMAP picBMP = NULL;				// ��Ϸͼ����
HBITMAP endBMP = NULL;				// ����ͼ����
HBITMAP scoreBMP = NULL;			// ����ͼ����
HBITMAP wallBMP = NULL;				// ǽ��ͼ����

HBRUSH wallBrush = NULL;			// ǽ�ڻ�ˢ


// ��ʱ��
UINT uSnakeMoveTimer;
UINT uFoodAddTimer;

RECT g_ClientRect;
int g_iClientWidth;
int g_iClientHeight;


// �����ں���
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevhInstance, PSTR lpszCmdLine, int nCmdShow)
{
	// create the window class structure
	WNDCLASS wndClass;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(101));
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszClassName = ProgramTitle.c_str();
	wndClass.lpszMenuName = NULL;
	// ע�ᴰ��
	RegisterClass(&wndClass);

	// ��ȡ��Ļ��С
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	// create a new window 
	HWND hWnd;
	hWnd = CreateWindow(
		ProgramTitle.c_str(),				// window class
		ProgramTitle.c_str(),				// title bar
		WS_POPUP,							// window style
		0, 0,								// window position
		width, height,						// window size
		NULL, NULL,							// parent window and menu
		hInstance,							// app instance
		NULL);								// window parameters

	hAppWnd = hWnd;

	//��ʾ����
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//��ȡ�����С
	GetClientRect(hAppWnd, &g_ClientRect);
	g_iClientWidth = g_ClientRect.right - g_ClientRect.left;
	g_iClientHeight = g_ClientRect.bottom - g_ClientRect.top;

	//����Ϸ����ֳ��ݣ����Ϊ20���С����
	//������ÿ������Ĵ�С
	tableBlockWidth = (g_iClientWidth - 2 * WALL_WIDTH) / 20;
	tableBlockHeight = (g_iClientHeight - 2 * WALL_HEIGHT) / 20;

	//��ȡ��ǰ�������豸��windowDC����
	windowDC = GetDC(NULL);
	//������windowDC���ݵ��ڴ��豸����
	bufferDC = CreateCompatibleDC(windowDC);
	picDC = CreateCompatibleDC(windowDC);
	endDC = CreateCompatibleDC(windowDC);
	scoreDC = CreateCompatibleDC(windowDC);

	//λͼ�ĳ�ʼ��������λͼ	
	bufferBMP = CreateCompatibleBitmap(windowDC, g_iClientWidth, g_iClientHeight);
	picBMP = (HBITMAP)LoadImage(NULL, "snake.bmp", IMAGE_BITMAP, 160, 80, LR_LOADFROMFILE);
	wallBMP = (HBITMAP)LoadImage(NULL, "brick.bmp", IMAGE_BITMAP, 16, 16, LR_LOADFROMFILE);
	endBMP = (HBITMAP)LoadImage(NULL, "end.bmp", IMAGE_BITMAP, 369, 300, LR_LOADFROMFILE);
	scoreBMP = (HBITMAP)LoadImage(NULL, "scoreboard.bmp", IMAGE_BITMAP, 265, 55, LR_LOADFROMFILE);

	//����λͼ���豸�����Ĺ���
	SelectObject(bufferDC, bufferBMP);
	SelectObject(picDC, picBMP);
	SelectObject(endDC, endBMP);
	SelectObject(scoreDC, scoreBMP);

	//������ˢ���������Ӧ��ͼ��Ĺ�����
	//�Ա���ˢ�ӽ�ǽˢ��������PatBlt()ʵ��
	wallBrush = CreatePatternBrush(wallBMP);

	StartGame();

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER:
		OnTimer((UINT)wParam);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			exit(0);
			break;
		case VK_UP:
			table.ChangeSnakeDirect(S_UP);
			break;
		case VK_DOWN:
			table.ChangeSnakeDirect(S_DOWN);
			break;
		case VK_LEFT:
			table.ChangeSnakeDirect(S_LEFT);
			break;
		case VK_RIGHT:
			table.ChangeSnakeDirect(S_RIGHT);
			break;
		case VK_SPACE:
			if (uGameState == GAME_STATE_END)
			{
				StartGame();
				break;
			}
		}
		return 0;
	case WM_SETCURSOR:
		SetCursor(NULL);
		return 0;
	case WM_DESTROY:
		ReleaseDC(hWnd, picDC);
		ReleaseDC(hWnd, bufferDC);
		ReleaseDC(hWnd, windowDC);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void DrawGame()
{
	//����������򵽻���
	WallDraw(bufferDC);

	//������Ϸ���򵽻���
	GameAreaDraw(bufferDC);

	//����������ӻ���DC��������Ļ
	BitBlt(windowDC, 0, 0, g_iClientWidth, g_iClientHeight, bufferDC, 0, 0, SRCCOPY);
}


void WallDraw(HDC hdc)
{
	//������ʾ��ʾ����
	char szText[30] = "Score: ";
	char szNum[20];

	int iNowScore = iScores * 100;
	_itoa_s(iNowScore, szNum, 10);
	strcat(szText, szNum);

	RECT rt, rect;
	GetClientRect(hAppWnd, &rt);
	//ǽ�Ļ���
	SelectObject(hdc, wallBrush);
	PatBlt(hdc, rt.left, rt.top, rt.right, rt.bottom, PATCOPY);
	//�ڲ���Ϸ���İ�ɫ��ƽ��
	rect.left = rt.left + WALL_WIDTH;
	rect.top = rt.top + WALL_HEIGHT;
	rect.right = rt.right - WALL_WIDTH;
	rect.bottom = rt.bottom - WALL_HEIGHT;
	FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

	//������ʾ��Ļ���
	BitBlt(hdc, GetSystemMetrics(SM_CXSCREEN) / 3,
		10, 256, 55, scoreDC, 0, 0, SRCCOPY);

	//�����Ĵ�ӡ����
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, GetSystemMetrics(SM_CXSCREEN) / 3 + 50, 30, szText, strlen(szText));
}

void GameAreaDraw(HDC hdc)
{
	int x, y, x_pos, y_pos;
	BitmapState  state;

	//����ˮ���� ����
	for (int i = 0; i<tableBlockHeight; i++)
		for (int j = 0; j<tableBlockWidth; j++)
		{
			x_pos = j * 20 + WALL_WIDTH;
			y_pos = i * 20 + WALL_HEIGHT;

			switch (table.GetData(j, i))
			{
			case TABLE_STATE_FOOD:
				BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 100, 0, SRCPAINT);
				BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 20, 0, SRCAND);
				break;
			case TABLE_STATE_BLOCK:
				BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 80, 0, SRCPAINT);
				BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 0, 0, SRCAND);
				break;
			}
		}

	//���ݵ�ǰ��״̬������ͷ		
	x = table.GetSnake()->GetPos()[0].x;
	y = table.GetSnake()->GetPos()[0].y;
	x_pos = x * 20 + WALL_WIDTH;
	y_pos = y * 20 + WALL_HEIGHT;
	state = table.GetSnake()->GetStateArray()[0];

	switch (state)
	{
	case M_UP_UP:
		BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 80, 20, SRCPAINT);
		BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 0, 20, SRCAND);
		break;
	case M_DOWN_DOWN:
		BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 140, 20, SRCPAINT);
		BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 60, 20, SRCAND);
		break;
	case M_LEFT_LEFT:
		BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 100, 20, SRCPAINT);
		BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 20, 20, SRCAND);
		break;
	case M_RIGHT_RIGHT:
		BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 120, 20, SRCPAINT);
		BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 40, 20, SRCAND);
		break;
	}

	//���ݸ�������ڵ��״̬�����������״
	for (int i = 1; i<table.GetSnake()->GetLength() - 1; i++)
	{
		x = table.GetSnake()->GetPos()[i].x;
		y = table.GetSnake()->GetPos()[i].y;
		x_pos = x * 20 + WALL_WIDTH;
		y_pos = y * 20 + WALL_HEIGHT;
		state = table.GetSnake()->GetStateArray()[i];
		switch (state)
		{
		case M_UP_UP:
		case M_DOWN_DOWN:
			BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 80, 40, SRCPAINT);
			BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 0, 40, SRCAND);
			break;
		case M_LEFT_LEFT:
		case M_RIGHT_RIGHT:
			BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 100, 40, SRCPAINT);
			BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 20, 40, SRCAND);
			break;
		case M_RIGHT_DOWN:
		case M_UP_LEFT:
			BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 100, 60, SRCPAINT);
			BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 20, 60, SRCAND);
			break;
		case M_LEFT_DOWN:
		case M_UP_RIGHT:
			BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 80, 60, SRCPAINT);
			BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 0, 60, SRCAND);
			break;
		case M_RIGHT_UP:
		case M_DOWN_LEFT:
			BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 140, 40, SRCPAINT);
			BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 60, 40, SRCAND);
			break;
		case M_LEFT_UP:
		case M_DOWN_RIGHT:
			BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 120, 40, SRCPAINT);
			BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 40, 40, SRCAND);
			break;

		}
	}

	//������β��
	x = table.GetSnake()->GetPos()[table.GetSnake()->GetLength() - 1].x;
	y = table.GetSnake()->GetPos()[table.GetSnake()->GetLength() - 1].y;
	x_pos = x * 20 + WALL_WIDTH;
	y_pos = y * 20 + WALL_HEIGHT;
	state = table.GetSnake()->GetStateArray()[table.GetSnake()->GetLength() - 1];
	switch (state)
	{
	case M_UP_UP:
		BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 120, 60, SRCPAINT);
		BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 40, 60, SRCAND);
		break;
	case M_DOWN_DOWN:
		BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 120, 0, SRCPAINT);
		BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 40, 0, SRCAND);
		break;
	case M_LEFT_LEFT:
		BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 140, 60, SRCPAINT);
		BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 60, 60, SRCAND);
		break;
	case M_RIGHT_RIGHT:
		BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 140, 0, SRCPAINT);
		BitBlt(hdc, x_pos, y_pos, 20, 20, picDC, 60, 0, SRCAND);
		break;
	}

	if (uGameState == GAME_STATE_END)
	{//������Ϸ����ͼ��
		int x_pos = g_iClientWidth / 3;
		int y_pos = g_iClientHeight / 4;

		BitBlt(hdc, x_pos, y_pos, 369, 300, endDC, 0, 0, SRCCOPY);
	}
}

void OnTimer(UINT Timer_id)
{
	if (Timer_id == uSnakeMoveTimer)
	{
		//�ƶ���
		table.SnakeMove();

		//����Ƿ���������(����)��������Ϸ
		if (table.GetSnake()->IsHeadTouchBody(table.GetSnake()->GetPos()[0].x, table.GetSnake()->GetPos()[0].y))
		{
			EndGame();
		}

		//������ͷ���ڵ�����������Ӧ�Ĵ���
		switch (table.GetData(table.GetSnake()->GetPos()[0].x, table.GetSnake()->GetPos()[0].y))
		{
		case TABLE_STATE_FOOD:
			table.ClearFood(table.GetSnake()->GetPos()[0].x, table.GetSnake()->GetPos()[0].y);
			//table.AddBlock((rand()) % tableBlockWidth, (rand()) % tableBlockHeight);
			table.GetSnake()->AddBody();
			++iScores;
			break;
		case TABLE_STATE_BLOCK:
		case TABLE_STATE_SBLOCK:
			//����Ƿ������ϰ��������Ϸ
			EndGame();
			break;
		}

		//��ʾ
		DrawGame();
	}
	else if (Timer_id == uFoodAddTimer)
	{
		//if (!checkOrNot)
		table.AddFood((rand()) % tableBlockWidth, (rand()) % tableBlockHeight);
	}
}

void StartGame()
{
	iScores = 0;

	//����ĳ�ʼ��
	table.InitalTable(tableBlockWidth, tableBlockHeight);
	table.GetSnake()->ChangeDirect(S_RIGHT);
	table.GetSnake()->SetHeadPos(tableBlockWidth / 2, tableBlockHeight / 2);

	//Ԥ�������������ʳ��
	// ��������
	srand((unsigned)time(NULL));
	for (int iFoodNum = 0; iFoodNum < 4; iFoodNum++)
	{
		table.AddFood((rand()) % tableBlockWidth, (rand()) % tableBlockHeight);
	}

	//�򿪶�ʱ��
	uSnakeMoveTimer = SetTimer(hAppWnd, 500, 100, NULL);
	uFoodAddTimer = SetTimer(hAppWnd, 600, 7000, NULL);

	uGameState = GAME_STATE_RUN;
}

void EndGame()
{
	// �رռ�ʱ��
	KillTimer(hAppWnd, uSnakeMoveTimer);
	KillTimer(hAppWnd, uFoodAddTimer);
	uGameState = GAME_STATE_END;
}

bool checkOrNot()
{
	for (int i = 0; i < table.GetHeight(); i++)
	{
		for (int j = 0; j < table.GetWidth(); j++)
		{
			if (table.GetData(i, j) == TABLE_STATE_FOOD)
			{
				return true;
			}
		}
	}
	return false;
}