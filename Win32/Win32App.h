#pragma once
#include <Windows.h>

class CWin32App
{
public:
	CWin32App(void);
	~CWin32App(void);
public:
	BOOL Create(HINSTANCE hinst);
	BOOL Run();
	BOOL InitializeObjects();
	void RenderScene();
	void Shutdown();

	LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	HWND		m_hWnd;
};
