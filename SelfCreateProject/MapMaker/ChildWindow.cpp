#include "ChildWindow.h"
#include "MainWindow.h"

LPCTSTR ChildWindow::lpszClass = L"TileWnd";
ChildWindow::ChildWindow() {
	WndClass.hInstance = MainWindow::GetSingleInstance()->GetInstance();
	WndClass.cbWndExtra = sizeof(int);
	WndClass.lpszClassName = lpszClass;
	WndClass.lpfnWndProc = WndProc;
}

ChildWindow::~ChildWindow() {}


LRESULT ChildWindow::WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMessage, wParam, lParam);
}

HWND ChildWindow::MakeTile(int x, int y, int width, int height) {
	HWND hWnd = CreateWindow(lpszClass, NULL, WS_CHILD | WS_BORDER | WS_VISIBLE,
							 x, y, width, height,
							 MainWindow::GetSingleInstance()->GetHandleWnd(), (HMENU)NULL,
							 MainWindow::GetSingleInstance()->GetInstance(), nullptr);
	return hWnd;
}

bool ChildWindow::InitWindow(HINSTANCE, int) {
	return false;
}
