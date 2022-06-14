#pragma once
#include "BaseWindow.h"
#include "BitmapManager.h"

class MainWindow
	:public BaseWindow {
public:
	MainWindow();
	virtual ~MainWindow();

	bool InitWindow(HINSTANCE hInstance, int nCmdShow);
	MSG Run();
	static MainWindow* GetSingleInstance();

private:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static void InitBitManager();

private:
	static MainWindow* _Instance;
	static BitmapManager* Bit_Instance;
	static LPCTSTR lpszClass;

	int countBox;
	int countGoal;
	int countStage;
	int nowStage;
	int selectTile;
};