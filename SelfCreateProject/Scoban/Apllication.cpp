#include "MainWindow.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstace, LPSTR lpszCmdParam, int nCmdShow) {
	MainWindow::GetSingleInstance()->SetInstance(hInstance);
	if (!MainWindow::GetSingleInstance()->InitWindow()) {
		MessageBox(NULL, L"윈도우 생성 실패", L"오류", MB_OK);
		return 0;
	}
	MainWindow::GetSingleInstance()->Active(nCmdShow);

	MSG Msg = MainWindow::GetSingleInstance()->Run();

	return (int)Msg.wParam;
}