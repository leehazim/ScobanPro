#include "FileMgr.h"
#include "MainWindow.h"
#include "BitmapManager.h"

tag_map FileMgr::m_WriteBuffer = {0};
tag_map FileMgr::m_LoadBuffer[10] = {0};
HANDLE FileMgr::m_hFile = nullptr;
DWORD FileMgr::m_dwWrite = 0;
DWORD FileMgr::m_dwRead = 0;
WCHAR FileMgr::m_FileName[100] = L"";

bool FileMgr::Save() {
	m_WriteBuffer = { 0 };
	m_WriteBuffer.stage = SendDlgItemMessage(MainWindow::GetSingleInstance()->GetHandleWnd(), ID_LIST, LB_GETCURSEL, 0, 0) + 1;
	int playerExist = 0;
	for (int i = 0; i < MAX_HEIGHT; i++) {
		for (int j = 0; j < MAX_WIDTH; j++) {
			m_WriteBuffer.Map[i][j] = GetWindowLongPtr(MainWindow::GetTile(i, j), ID_TILEBUFFER);
			if (GetWindowLongPtr(MainWindow::GetTile(j, i), ID_TILEBUFFER) == BitmapManager::tag_tile::BOX)
				m_WriteBuffer.cntBox++;
			if (GetWindowLongPtr(MainWindow::GetTile(j, i), ID_TILEBUFFER) == BitmapManager::tag_tile::GOAL)
				m_WriteBuffer.cntGoal++;
			if (GetWindowLongPtr(MainWindow::GetTile(j, i), ID_TILEBUFFER) == BitmapManager::tag_tile::MAN)
				playerExist++;
		}
	}

	/*if (playerExist != 1) {
		MessageBox(MainWindow::GetSingleInstance()->GetHandleWnd(), L"플레이어는 1개 존재해야합니다.", L"알림", MB_OK);
		return false;
	}
	if (m_WriteBuffer.cntBox != m_WriteBuffer.cntGoal) {
		MessageBox(MainWindow::GetSingleInstance()->GetHandleWnd(), L"목적지와 박스의 개수가 맞지 않습니다.", L"다시 시도!", MB_OK);
		return false;
	}*/

	wsprintf(m_FileName, L"C:\\Asset\\%s", MainWindow::GetSingleInstance()->GetStageName(m_WriteBuffer.stage - 1));
	m_hFile = CreateFile(m_FileName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE) {
		MessageBox(MainWindow::GetSingleInstance()->GetHandleWnd(), TEXT("파일 생성 실패"), TEXT("알림"), MB_OK);
		return false;
	}
	WriteFile(m_hFile, &m_WriteBuffer, sizeof(tag_map), &m_dwWrite, NULL);
	CloseHandle(m_hFile);
	m_hFile = nullptr;
	m_WriteBuffer = { 0 };

	return true;
}

bool FileMgr::Load() {
	for (int i = 0; i < 10; i++) {
		wsprintf(m_FileName, L"\\Asset\\%s", MainWindow::GetSingleInstance()->GetStageName(i + 1));
		m_hFile = CreateFile(m_FileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (m_hFile == INVALID_HANDLE_VALUE) {
			tag_map* sub = (tag_map*)calloc(1, sizeof(tag_map));
			m_LoadBuffer[i] = *sub;
			free(sub);
			return false;
		}
		ReadFile(m_hFile, &m_LoadBuffer[i], sizeof(tag_map), &m_dwRead, NULL);
		CloseHandle(m_hFile);
	}

	for (int i = 0; i < MainWindow::GetSingleInstance()->GetMaxStage(); i++) {
		MainWindow::GetSingleInstance()->GetMap(i) = m_LoadBuffer[i];
	}
	return true;
}
