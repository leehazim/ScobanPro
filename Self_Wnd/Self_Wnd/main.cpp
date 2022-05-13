#include "Bullet.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MenuProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND g_hWnd;
LPCTSTR lpszClass = TEXT("Follow Rect");
LPCTSTR MenuClass = TEXT("Menu");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nCmdParam, int nCmdShow) {

	HWND hWnd;
	WNDCLASS WndClass;
	MSG Msg;
	g_hInst = hInstance;

	WndClass.style = CS_VREDRAW | CS_HREDRAW;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	RegisterClass(&WndClass);

	WndClass.lpfnWndProc = MenuProc;
	WndClass.lpszClassName = MenuClass;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 640,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Msg, NULL, 0, 0)){
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}
const int MAXENEMY = 20;
Player P;
AutoMove* A[30];

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	HDC hdc;
	PAINTSTRUCT ps;
	HWND MenuWnd;
	int i;
	int key;

	switch (msg) {
	case WM_CREATE:
		/* 게임 시작시 난수발생기 생성*/
		srand(time(NULL)); 
		/* 게임 시작과 동시에 적군 만들어질 메모리 확보*/
		for (i = 0; i < 30; i++) {
			if (i < 10) A[i] = new Bullet(P);
			else A[i] = new Enemy(&P);
		}
		SetTimer(hwnd, 1, 20, NULL);
		return 0;

	case WM_TIMER:
		/* 1000분의 1 확률로 적군 생성*/
		for (i = 10; i < MAXENEMY + 10; i++) if (!(rand() % 1000)) A[i]->SetExist(true);
		for (i = 0; i < 30; i++) {
			if (!A[i]->GetExist()) continue;
			A[i]->Move();
		}
		/* 플레이어와 적군 충돌감지*/
		for (i = 10; i < MAXENEMY + 10; i++) {
			if (dynamic_cast<Enemy*>(A[i])->IsCrash(&P)) {  SendMessage(hwnd, WM_DESTROY, 0, 0); break;	}
			for (int j = 0; j < 10; j++) {
				if (!A[j]->GetExist()) continue;
				dynamic_cast<Bullet*>(A[j])->IsCrash(dynamic_cast<Enemy*>(A[i]));
			}
		}
		
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_KEYDOWN:
		/* 키 입력으로 플레이어 이동및 총알 방향 설정*/
		P.Move(LOWORD(wParam));
		for (i = 0; i < 10; i++) 
			if (!A[i]->GetExist()) { 
				dynamic_cast<Bullet*>(A[i])->Make(LOWORD(wParam), P); 
				break; 
			}
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		/* 키 입력시마다 좌표 다시 검사해서 플레이어 그리기*/
		P.Draw(hdc);
		/* wm_timer 발생시마다 자동객체들 다시그리기*/
		for (i = 0; i < 30; i++) {
			if (!A[i]->GetExist()) continue;
			A[i]->Draw(hdc);
		}
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		/* 확보해 두었던 적군 총알들을 메모리 해제*/
		for (i = 0; i < 30; i++) delete A[i];
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK MenuProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	
	switch (msg) {
	case WM_CREATE:
		CreateWindow(TEXT("button"), TEXT("Close"), WS_CHILD | WS_VISIBLE  | BS_PUSHBUTTON,
					 200, 100, 50, 20, hwnd, (HMENU)1, g_hInst, NULL);
		return 0;

	case WM_COMMAND:
		switch (wParam) {
		case 1: 
			SendMessage(hwnd, WM_DESTROY, 0, 0);
			break;
		}
		return 0;

	case WM_DESTROY: DestroyWindow(hwnd); return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}