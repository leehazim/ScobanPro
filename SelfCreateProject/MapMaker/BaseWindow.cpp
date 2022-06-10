#include "BaseWindow.h"

BaseWindow::BaseWindow() {
	WndClass = { 0, };
	m_hWnd = nullptr;
	m_hInst = nullptr;
}

BaseWindow::BaseWindow(HINSTANCE hInstance) 
	:m_hInst(hInstance) {
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hInstance = m_hInst;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.lpszMenuName = NULL;
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
