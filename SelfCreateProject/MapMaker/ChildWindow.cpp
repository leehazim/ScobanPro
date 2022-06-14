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

BitmapManager* ChildWindow::GetBitInsntance() {
	if (Bit_Instance == nullptr)
		Bit_Instance = new BitmapManager();
	return Bit_Instance;
}

LRESULT ChildWindow::WndProc(HWND, UINT, WPARAM, LPARAM) {
	return LRESULT();
}

HWND ChildWindow::MakeTile(int x, int y, int width, int height) {
	HWND hWnd = CreateWindow(lpszClass, NULL, WS_CHILD | WS_BORDER | WS_VISIBLE,
							 x, y, width, height,
							 MainWindow::GetSingleInstance()->GetHandleWnd(), (HMENU)NULL,
							 MainWindow::GetSingleInstance()->GetInstance(), nullptr);
	return hWnd;
}
