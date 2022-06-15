#pragma once
#include "childWindow.h"

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

private:
	static MainWindow* _Instance;
	static LPCTSTR lpszClass;
	static HWND Tiles[MAX_HEIGHT][MAX_WIDTH];

	int countBox;
	int countGoal;
	int countStage;
	int nowStage;
	int selectTile;
	ChildWindow* ChildInfo;
};