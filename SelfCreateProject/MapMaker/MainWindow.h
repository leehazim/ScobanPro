#pragma once
#include "BaseWindow.h"

class BaseWindow;
class FileManager;
class BitmapManager;

class MainWindow
	:public BaseWindow, public BitmapManager {
public:
	MainWindow();
	virtual ~MainWindow();

	bool InitWindow(HINSTANCE hInstance, int nCmdShow);
	MSG Run();
	static MainWindow* GetSingleInstance();

private:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);

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
};