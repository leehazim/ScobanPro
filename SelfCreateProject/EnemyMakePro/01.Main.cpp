#include "Main.h"
#pragma comment(lib, "Main.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdParam, int nCmdShow) {
	MainWindow::GetInstance(hInstance, L"Zombie")->RegiWnd(WndProc);
	if (!MainWindow::GetInstance(hInstance, L"Zomebie")->InitWnd(nCmdShow))
		return 0;
	return MainWindow::GetInstance(hInstance, L"Zombie")->Running();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	switch (iMessage) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMessage, wParam, lParam);
}