#pragma once
#include "BaseWindow.h"

class FileManager;
class BitmapManager;

class MainWindow
	:public BaseWindow {
public:
	MainWindow();
	virtual ~MainWindow();

	bool InitWindow(HINSTANCE hInstance, int nCmdShow);
	MSG Run();
	static MainWindow* GetSingleInstance();
	static HWND GetTile(int x, int y);

private:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);

	void Update();

private:
	static MainWindow* _Instance;
	static LPCTSTR lpszClass;
	static LPCTSTR lpszChild;
	static HWND Tiles[MAX_HEIGHT][MAX_WIDTH];

	int countBox;
	int countGoal;
	int countStage;
	int nowStage;
	int selectTile;

	HDC m_MainDC;
	static HDC m_ChildDC[MAX_HEIGHT][MAX_WIDTH];
};