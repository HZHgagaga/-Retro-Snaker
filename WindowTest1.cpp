// WindowTest1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WindowProc.h"


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	// TODO: Place code here.
	WNDCLASS wc = {0};
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = "MainFrame";
	wc.style = CS_HREDRAW | CS_VREDRAW;
	
	if (!RegisterClass(&wc))
	{
		return -1;
	}

	HWND hWnd = CreateWindow("MainFrame", "haha", WS_OVERLAPPEDWINDOW^WS_THICKFRAME, 400, 100, 655, 650, NULL, NULL, hInstance, NULL);
	
	if (NULL == hWnd)
	{
		return -1;
	}
	
	ShowWindow(hWnd, SW_SHOW);
	
	MSG msg = {0};
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return 0;
}


