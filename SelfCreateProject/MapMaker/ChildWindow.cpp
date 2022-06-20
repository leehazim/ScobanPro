#include "ChildWindow.h"
#include "MainWindow.h"

LPCTSTR ChildWindow::lpszClass = L"TileWnd";
HWND ChildWindow::Tiles[MAX_HEIGHT][MAX_WIDTH];

ChildWindow::ChildWindow() {
	WndClass.hInstance = MainWindow::GetSingleInstance()->GetInstance();
	WndClass.cbWndExtra = sizeof(int);
	WndClass.lpszClassName = lpszClass;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszMenuName = nullptr;
}

ChildWindow::~ChildWindow() {}


LRESULT ChildWindow::WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_PAINT:

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMessage, wParam, lParam);
}

void ChildWindow::MakeTile(int x, int y, int width, int height) {
	Tiles[y][x] =  CreateWindow(lpszClass, nullptr, WS_CHILD | WS_BORDER | WS_VISIBLE,
							 x * width, y * height, width, height,
							 MainWindow::GetSingleInstance()->GetHandleWnd(), (HMENU)NULL,
							 MainWindow::GetSingleInstance()->GetInstance(), nullptr);
}

bool ChildWindow::InitWindow(HINSTANCE hInstance, int) {
	RegisterClass(&WndClass);
	return true;
}

void ChildWindow::Show() {
	for (int i = 0; i < MAX_HEIGHT; i++) {
		for (int j = 0; j < MAX_WIDTH; j++)
			ShowWindow(Tiles[i][j], SW_SHOW);
	}
}
