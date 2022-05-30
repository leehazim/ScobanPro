#pragma once
#include "BaseWindow.h"
#include "GameManager.h"

class MainWindow
	:public BaseWindow {
public:
	MainWindow();
	MainWindow(HINSTANCE hInstance);
	~MainWindow();

	bool InitWindow();
	void Active(int nCmdShow);
	static MainWindow* GetSingleInstance();
	static BitmapManager* GetBitmapManager();
	static GameManager* GetGameInstance();

private:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static void InitBitManager();
	static void InitGameManager();

private:
	static GameManager* Game_Instance;
	static BitmapManager* Bit_Instance;
	static MainWindow* _Instance;
	LPCTSTR lpszClass;
};