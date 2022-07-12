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
	MSG Run();

	void OnCreate();
	void OnUpdate();
	void OnDestroy();
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

	HDC m_hDC;
	static bool isClear;
	static bool restart;
	static bool enemyInited;
};