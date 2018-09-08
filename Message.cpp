#include "StdAfx.h"
#include "Message.h"

#define STAY 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

HBITMAP hbmp;
BITMAP bmp;

BOOL g_bFlag = FALSE;
int nMove = STAY;
BOOL g_bFood = TRUE;
SnakeNode *g_pHead;
SnakeNode *g_pEnd;
SnakeNode *g_pFood;
int g_nSneakLength;
int g_nSpeed = 45;

void CreateSnake()
{
	SnakeNode *pNext = NULL;
	SnakeNode *pTemp = g_pHead;
	
	while (NULL != pTemp)
	{
		pNext = pTemp->m_pNext;
		delete pTemp;
		pTemp = pNext;
	}
	
	g_pHead = NULL;
	g_pEnd = NULL;
	
	nMove = STAY;
	g_nSneakLength = 0;

	SnakeNode *sHead = new SnakeNode;
	SnakeNode *sBody = new SnakeNode;
	SnakeNode *sEnd = new SnakeNode;
	
	sHead->m_nX = 20;
	sHead->m_nY = 0;
	sBody->m_nX = 10;
	sBody->m_nY = 0;
	sEnd->m_nX = 0;
	sEnd->m_nY = 0;
	
	sHead->m_pNext = sBody;
	sBody->m_pNext = sEnd;
	sEnd->m_pNext = NULL;
	g_pHead = sHead;
	g_pEnd = sEnd;
	g_nSneakLength += 3;
}

void Door(HWND hWnd, int nLeftRect1, int nTopRect1, int nRightRect1, int nBottomRect1,
		  int nLeftRect2, int nTopRect2, int nRightRect2, int nBottomRect2, int nW)
{
	BOOL bOpen = TRUE;
	int nX1 = 0;
	int nY1 = 0;
	int nX2 = 0;
	int nY2 = 0;
	
	srand(time(NULL));
	
	if (bOpen)
	{
		if (g_pHead->m_nX > (nLeftRect1 - 10) && g_pHead->m_nX < nRightRect1 && g_pHead->m_nY > (nTopRect1 - 10) && g_pHead->m_nY < nBottomRect1)
		{
			nX1 = g_pHead->m_nX;
			nY1 = g_pHead->m_nY;

			if (1 == nW)
			{
				g_pHead->m_nX = nX1 + 300;
				g_pHead->m_nY = nY1 + 300;
			}
			else
			{
				g_pHead->m_nX = nX1 - 300;
				g_pHead->m_nY = nY1 + 300;
			}
			
			bOpen = !bOpen;
			
			switch (nMove)
			{
			case UP:
				nMove = DOWN;
				break;
			case DOWN:
				nMove = UP;
				break;
			case LEFT:
				nMove = RIGHT;
				break;
			case RIGHT:
				nMove = LEFT;
				break;
			}
		}
	}
	
	if (bOpen)
	{
		if (g_pHead->m_nX > (nLeftRect2 - 10) && g_pHead->m_nX < nRightRect2 && g_pHead->m_nY > (nTopRect2 - 10) && g_pHead->m_nY < nBottomRect2)
		{
			nX2 = g_pHead->m_nX;
			nY2 = g_pHead->m_nY;

			if (1 == nW)
			{
				g_pHead->m_nX = nX2 - 300;
				g_pHead->m_nY = nY2 - 300;
			}
			else
			{
				g_pHead->m_nX = nX2 + 300;
				g_pHead->m_nY = nY2 - 300;
			}

			bOpen = !bOpen;
			
			switch (nMove)
			{
			case UP:
				nMove = DOWN;
				break;
			case DOWN:
				nMove = UP;
				break;
			case LEFT:
				nMove = RIGHT;
				break;
			case RIGHT:
				nMove = LEFT;
				break;
			}
		}
	}
}

void WallBoom(HWND hWnd)
{
	SnakeNode *pSnake = g_pHead;
	RECT reRect = {0};
	GetClientRect(hWnd, &reRect);
	RECT reRect1 = {270, 280, 370, 340};
	POINT pt = {g_pHead->m_nX, g_pHead->m_nY};

	if (0 > pSnake->m_nX || 0 > pSnake->m_nY || reRect.right < pSnake->m_nX || (reRect.bottom - 10) < pSnake->m_nY)
	{
		KillTimer(hWnd, 123);
		MessageBox(hWnd, "头撞碎了!", "吔屎丫你", MB_OK);
		g_nSpeed = 45;
		CreateSnake();
	}

	if (PtInRect(&reRect1, pt))
	{
		KillTimer(hWnd, 123);
		MessageBox(hWnd, "啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊!碎♂了!", "啊啊啊", MB_OK);
		g_nSpeed = 45;
		CreateSnake();
	}
}

void EatSelf(HWND hWnd)
{
	SnakeNode *pSnake = g_pHead;
	SnakeNode *pEat = g_pHead->m_pNext->m_pNext;

	while (NULL != pEat)
	{
		if (pSnake->m_nX == pEat->m_nX && pSnake->m_nY == pEat->m_nY)
		{
			if (g_nSneakLength > 4)
			{
				KillTimer(hWnd, 123);
				MessageBox(hWnd, "咬断了!", "啊♂", MB_OK);
				g_nSpeed = 45;
				CreateSnake();
				break;
			}
		}
		
		pEat = pEat->m_pNext;
	}
}

SnakeNode *CreateFood(HWND hWnd)
{
	RECT reRect = {0};
	GetClientRect(hWnd, &reRect);
	SnakeNode *Food = new SnakeNode;
	SnakeNode *pTemp = g_pHead; 
	RECT reRect1 = {100, 100, 220, 220};
	RECT reRect2 = {400, 400, 520, 520};
	RECT reRect3 = {400, 100, 520, 220};
	RECT reRect4 = {100, 400, 220, 520};
	RECT reRect5 = {0, 0, 20, 10};
	RECT reRect6 = {260, 270, 380, 350};
	RECT reRect7 = {170, 20, 250, 70};
	RECT reRect8 = {390, 20, 470, 70};


	HBITMAP hbbp = (HBITMAP)LoadImage(NULL, "bei.bmp", IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	GetObject(hbbp, sizeof(BITMAP), &bmp);
	SelectObject(hdcMem, hbbp);
	SetStretchBltMode(hdc, COLORONCOLOR);
	StretchBlt(hdc, 0, 0, reRect.right, reRect.bottom, hdcMem, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	
	hbmp = (HBITMAP)LoadImage(NULL, "xianyu.bmp", IMAGE_BITMAP, 50, 100, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	GetObject(hbmp, sizeof(BITMAP), &bmp);
	SelectObject(hdcMem, hbmp);
	BitBlt(hdc, FishKing.m_X, 520, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);		
	DeleteDC(hdcMem);


	while (1)
	{
		srand(time(NULL));
		Food->m_nX = (rand() % (((reRect.right - 10) - reRect.left) / 10)) * 10;
		Food->m_nY = (rand() % (((reRect.bottom - 10) - reRect.top) / 10)) * 10;
		POINT pt = {Food->m_nX, Food->m_nY};
		pTemp = g_pHead;

		if (!PtInRect(&reRect5, pt))
		{
			if (PtInRect(&reRect2, pt) || PtInRect(&reRect1, pt) || PtInRect(&reRect3, pt) || PtInRect(&reRect4, pt) || PtInRect(&reRect6, pt) || PtInRect(&reRect6, pt) || PtInRect(&reRect7, pt))
			{
				Food->m_nX += 100;
				Food->m_nY += 100;
			}

			while (NULL != pTemp)
			{
				if (pTemp->m_nX == Food->m_nX && pTemp->m_nY == Food->m_nY)
				{
					break;
				}

				if (pTemp == g_pEnd && pTemp->m_nX != Food->m_nX && pTemp->m_nY != Food->m_nY)
				{
					Food->m_pNext = NULL;
					return Food;
				}

				pTemp = pTemp->m_pNext;
			}
		}
	}
}

void SnakeMove(HWND hWnd)
{
	SnakeNode *pSnake = g_pHead;
	int nSnakeLen = 0;

	switch (nMove)
	{
		case UP:
				pSnake->m_nY -= 10;
				break;
		case DOWN:
				pSnake->m_nY += 10;
				break;
		case LEFT:
				pSnake->m_nX -= 10;
				break;
		case RIGHT:
				pSnake->m_nX += 10;
				break;
		default:
				break;
	}

	if (pSnake->m_nX == g_pFood->m_nX && pSnake->m_nY == g_pFood->m_nY)
	{
		g_pEnd->m_pNext = g_pFood;
		g_pEnd = g_pFood;
		g_pFood = NULL;
		g_bFood = TRUE;
		g_nSneakLength++;
	}

	Door(hWnd, 110, 110, 210, 210, 410, 410, 510, 510, 1);
	Door(hWnd, 410, 110, 510, 210, 110, 410, 210, 510, 0);
}

MSG_MAP g_msg[] = { {WM_CREATE, OnCREATE},
					{WM_LBUTTONDOWN, OnLBUTTONDOWN},
					{WM_KEYDOWN, OnKEYDOWN},
					{WM_PAINT, OnPAINT},
					{WM_CLOSE, OnCLOSE},
					{WM_DESTROY, OnDESTROY},
					{WM_NULL, NULL}};



void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	SnakeNode *pSnake = g_pHead;
	int nLastX1 = 0;
	int nLastY1 = 0;
	int nLastX = pSnake->m_nX;
	int nLastY = pSnake->m_nY;
	SnakeMove(hWnd);
	InvalidateRect(hWnd, NULL, TRUE);
	pSnake = pSnake->m_pNext;

	while (NULL != pSnake)
	{
		if (g_pEnd == pSnake)
		{
			pSnake->m_nX = nLastX;
			pSnake->m_nY = nLastY;
			break;
		}
		nLastX1 = pSnake->m_nX;
		nLastY1 = pSnake->m_nY;
		pSnake->m_nX = nLastX;
		pSnake->m_nY = nLastY;
		nLastX = nLastX1;
		nLastY = nLastY1;
		pSnake = pSnake->m_pNext;
	}

	WallBoom(hWnd);
	EatSelf(hWnd);

	InvalidateRect(hWnd, NULL, TRUE);
}

LRESULT OnCREATE(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (g_nSneakLength == 0)
	{
		CreateSnake();
	}

	return 0;
}

LRESULT OnKEYDOWN(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int nKey = (int)wParam;
	SnakeNode *pSnake = g_pHead;
	
	switch (nKey)
	{
		case VK_UP:
			{
				if (STAY == nMove)
				{
					nMove = UP;
				}
				else if (UP == nMove || DOWN == nMove || g_pHead->m_nY == (g_pHead->m_pNext->m_nY + 10))
				{
					break;
				}

				nMove = UP;

				SetTimer(hWnd, 123, g_nSpeed, TimerProc);
			}
			break;
		case VK_DOWN:
			{
				if (STAY == nMove)
				{
					nMove = DOWN;
				}
				else if (DOWN == nMove || UP == nMove || g_pHead->m_nY == (g_pHead->m_pNext->m_nY - 10))
				{
					break;
				}

				nMove = DOWN;

				SetTimer(hWnd, 123, g_nSpeed, TimerProc);
			}
			break;
		case VK_LEFT:
			{
				if (STAY == nMove)
				{
					if (g_pHead->m_nX == (g_pHead->m_pNext->m_nX + 10))
					{
						break;
					}
					
					nMove = LEFT;
				}
				else if (LEFT == nMove || RIGHT == nMove || g_pHead->m_nX == (g_pHead->m_pNext->m_nX + 10))
				{
					break;
				}

				nMove = LEFT;

				SetTimer(hWnd, 123, g_nSpeed, TimerProc);
			}
			break;
		case VK_RIGHT:
			{
				if (STAY == nMove)
				{
					nMove = RIGHT;
				}
				else if (LEFT == nMove || RIGHT == nMove || g_pHead->m_nX == (g_pHead->m_pNext->m_nX - 10))
				{
					break;
				}

				nMove = RIGHT;

				SetTimer(hWnd, 123, g_nSpeed, TimerProc);
			}
			break;
		default:
			break;
	}

	return 0;
}

LRESULT OnPAINT(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	SnakeNode *pSnake = g_pHead;
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	TCHAR str[255];
	wsprintf(str, "分数: %d   ♂", g_nSneakLength);

	HBRUSH hBrush = CreateSolidBrush(RGB(rand(), rand(), rand()));
	HBRUSH hBrushF = CreateSolidBrush(RGB(255, 255, 0));
	HBRUSH hBrushD = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH hBrushDL = CreateSolidBrush(RGB(96, 96, 96));
	SelectObject(hdc, hBrush);

	while (NULL != pSnake)
	{
		if (pSnake == g_pHead)
		{
			HDC hdcMem  = CreateCompatibleDC(hdc);	//创建一个与指定设备兼容的内存设备上下文环境
			hbmp = (HBITMAP)LoadImage(NULL, "xianyu.bmp", IMAGE_BITMAP, 50, 100, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
			GetObject(hbmp, sizeof(BITMAP), &bmp);	//得到一个位图对象
			
			SelectObject(hdcMem, hbmp);
			BitBlt(hdc, pSnake->m_nX, pSnake->m_nY, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);		//显示位图
			
			DeleteDC(hdcMem);
			DeleteObject(hbmp);

			pSnake = pSnake->m_pNext;
			continue;
		}
		Rectangle(hdc, pSnake->m_nX, pSnake->m_nY, pSnake->m_nX + 10, pSnake->m_nY + 10);
		pSnake = pSnake->m_pNext;
	}

	SelectObject(hdc, hBrushD);
	Rectangle(hdc, 410, 110, 510, 210);
	Rectangle(hdc, 110, 410, 210, 510);
	Rectangle(hdc, 110, 110, 210, 210);
	Rectangle(hdc, 410, 410, 510, 510);
	SelectObject(hdc, hBrushDL);
	Ellipse(hdc, 130, 430, 190, 490);
	Ellipse(hdc, 430, 130, 490, 190);
	Ellipse(hdc, 130, 130, 190, 190);
	Ellipse(hdc, 430, 430, 490, 490);
	TextOut(hdc, 448, 150, "boy", strlen("boy"));
	TextOut(hdc, 147, 150, "hey", strlen("hey"));
	TextOut(hdc, 147, 450, "next", strlen("next"));
	TextOut(hdc, 447, 450, "door", strlen("door"));

	if (g_bFood)
	{
		g_pFood = CreateFood(hWnd);
		g_bFood = !g_bFood;
	}

	SelectObject(hdc, hBrushF);

	if (NULL != g_pFood)
	{
		Ellipse(hdc, g_pFood->m_nX, g_pFood->m_nY, g_pFood->m_nX + 10, g_pFood->m_nY + 10);
	}

	SelectObject(hdc, hBrush);
	Rectangle(hdc, 270, 280, 370, 340);
	TextOut(hdc, 284, 302, str, strlen(str));

	SelectObject(hdc, hBrushD);
	Rectangle(hdc, 180, 30, 240, 60);
	Rectangle(hdc, 400, 30, 460, 60);
	TextOut(hdc, 195, 36, "减速", strlen("减速"));
	TextOut(hdc, 415, 36, "加速", strlen("加速"));

	RECT reRect1 = {180, 30, 240, 60};
	RECT reRect2 = {410, 40, 460, 60};

	POINT pt1 = {g_pHead->m_nX, g_pHead->m_nY};

	if (PtInRect(&reRect1, pt1))
	{
		g_nSpeed += 5;
	}

	POINT pt2 = {g_pHead->m_nX, g_pHead->m_nY};

	if (PtInRect(&reRect2, pt2) && g_nSpeed >= 5)
	{
		g_nSpeed -= 5;
	}
	
	DeleteObject(hBrush);
	DeleteObject(hBrushF);
	DeleteObject(hBrushD);	
	DeleteObject(hBrushDL);
	EndPaint(hWnd, &ps);

	return 0;
}

LRESULT OnLBUTTONDOWN(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT OnCLOSE(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	DestroyWindow(hWnd);

	return 0;
}

LRESULT OnDESTROY(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);

	return 0;
}