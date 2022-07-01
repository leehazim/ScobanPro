#pragma once
#include <Windows.h>
#include "resource1.h"

class MainWindow;
class BitmapManager;

struct tag_map {
	int Map[MAX_HEIGHT][MAX_WIDTH];
	int stage;
	int cntBox, cntGoal;
};

class FileMgr {
public:
	static bool Save();
	static bool Load();

	static FileMgr* GetInstance();

private:
	static tag_map m_WriteBuffer;
	static tag_map m_LoadBuffer[10];
	static HANDLE m_hFile;
	static DWORD m_dwWrite, m_dwRead;
	static WCHAR m_FileName[100];

	static FileMgr* _Instance;
};

