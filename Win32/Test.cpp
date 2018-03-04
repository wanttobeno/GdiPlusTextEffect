#include <Windows.h>
#include <GdiPlus.h>
#include "Win32App.h"
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
	int nRet = -1;
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_pGdiToken, &m_gdiplusStartupInput, NULL);
	CWin32App win32app;
	if (win32app.Create(hInstance))
		nRet = win32app.Run();
	GdiplusShutdown(m_pGdiToken);
	return nRet;
}