#pragma once
#include "BaseWindow.h"
#include "FileMgr.h"

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
	static WCHAR* GetStageName(int idx);
	static const int GetMaxStage();
	static tag_map GetMap(int idx);

private:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);

	static void MakeChild(HWND hwnd);

	void Update();

private:
	static MainWindow* _Instance;
	static LPCTSTR lpszClass;
	static LPCTSTR lpszChild;
	static HWND Tiles[MAX_HEIGHT][MAX_WIDTH];
	static int ChangeSelect(WORD param);

	static int countBox;
	static int countGoal;
	static int countStage;
	static int countEnemy;
	static int nowStage;
	static int selectTile;

	HDC m_MainDC;
	static HDC m_ChildDC[MAX_HEIGHT][MAX_WIDTH];
	static const int Max_stage;
	static WCHAR* m_IdStage[10];
	static tag_map Map[10];

	static int m_ClientWidth;
	static int m_ClientHeight;
	
	static HWND m_hStatic;
	static HWND m_hListbox;
	static void InitMap();
};