#include "BaseWindow.h"

HINSTANCE BaseWindow::m_hInst;
HWND BaseWindow::m_hWnd;

BaseWindow::BaseWindow() {
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.lpszMenuName = NULL;

	m_hInst = nullptr;
	m_hWnd = nullptr;
}

BaseWindow::~BaseWindow() {}

void BaseWindow::SetInstance(HINSTANCE hInstance) {
	m_hInst = hInstance;
}

HINSTANCE BaseWindow::GetInstance() {
	return m_hInst;
}

HWND BaseWindow::GetHandleWnd() {
	return m_hWnd;
}
