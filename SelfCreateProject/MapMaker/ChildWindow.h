#pragma once
#include "BaseWindow.h"
#include "resource1.h"

class BitmapManager;
class ChildWindow 
	: public BaseWindow {
public:
	ChildWindow();
	virtual ~ChildWindow();

	static BitmapManager* GetBitInsntance();
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static HWND MakeTile(int x, int y, int width, int height);

private:
	static  BitmapManager* Bit_Instance;
	static LPCTSTR lpszClass;
};

