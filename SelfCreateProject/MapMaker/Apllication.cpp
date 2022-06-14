#include "MainWindow.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstace, LPSTR lpszCmdParam, int nCmdShow) {
	
	if (!MainWindow::GetSingleInstance()->InitWindow(hInstance, nCmdShow)) {
		MessageBox(NULL, L"������ ���� ����", L"����", MB_OK);
		return 0;
	}
	MSG Msg = MainWindow::GetSingleInstance()->Run();
	return (int)Msg.wParam;
}