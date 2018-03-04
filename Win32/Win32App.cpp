#include "Win32App.h"
#include <tchar.h>
#include <GdiPlus.h>
#include "MemDC.h"
#include "BaseTrace.h"
#include <string>
using namespace std;
using namespace Gdiplus;

#define WINDOW_CLASS    _T("Win32")
#define WINDOW_NAME     _T("Win32App")
#define WINDOW_WIDTH    450
#define WINDOW_HEIGHT   300

#define  ZY_TRANCEA  

CWin32App	*g_pApp=NULL;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return g_pApp->WinProc(hWnd,msg,wParam,lParam);
}

CWin32App::CWin32App(void)
{
	m_hWnd = NULL;
	g_pApp= this;
}

CWin32App::~CWin32App(void)
{
}

BOOL CWin32App::Run()
{
	// Show the window
	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hWnd);

	// Enter the message loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			RenderScene();
	}
	Shutdown();
	return 0;
}

void CWin32App::Shutdown()
{

}

void CWin32App::RenderScene()
{
	
}

BOOL CWin32App::Create(HINSTANCE hinst)
{
	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		WINDOW_CLASS, NULL };
	RegisterClassEx(&wc);

	// Create the application's window
	m_hWnd = CreateWindow(WINDOW_CLASS, WINDOW_NAME, WS_POPUPWINDOW,
		100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, GetDesktopWindow(),
		NULL, wc.hInstance, NULL);

	BOOL bRet = IsWindow(m_hWnd);
	return bRet;
}

//aardio
//参数：窗体对象,透明度,阴影大小,外圆角,内圆角,阻影颜色,插值位置,聚焦系数) = 
//除参数@1以外,其他所有参数可选  透明度默认为80,255为不透明  阴影大小默认为3像素  外圆角默认为阴影大小  
//内圆角默认为空  阴影颜色默认为0xFF000000,FF为透明度  插值位置默认为0.15  聚焦系数默认为15
int shadowSize = 3;
int roundRadius = 10;
double interpolation = 0.15;
Color shadowColor= Gdiplus::Color(255,255,0,0); //0xff880000;
int shadowAlpha = 80;
int focusRatio = 15;


// 添加圆角矩形
void addRoundRect(GraphicsPath &path,RECT rc,int r1,int r2=0,int r3=0,int r4=0)
{
	int x = rc.left;  
	int y = rc.top; 
	int w = rc.right - rc.left;  
	int h = rc.bottom - rc.top;  
	if( r2 == 0 ) r2 = r1;
	if( r3 == 0 ) r3 = r1;
	if( r4 == 0 ) r4 = r2; 
	path.AddBezier(x,y + r1,x,y,x + r1,y,x + r1,y);
	path.AddLine(x + r1,y,(x + w) - r2,y);
	path.AddBezier((x + w) - r2,y,x + w,y,x + w,y + r2,x + w,y + r2);  
	path.AddLine((x + w),(y + r2),(x + w),(y + h) - r3);  
	path.AddBezier(x + w,(y + h) - r3,x + w,y + h,(x + w) - r3,y + h,(x + w) - r3,y + h);  
	path.AddLine((x + w) - r3,y + h,x + r4,y + h);  
	path.AddBezier(x + r4,y + h,x,y + h,x,(y + h) - r4,x,(y + h) - r4);   
	path.AddLine(x,(y + h) - r4,x,y + r1); 
}

LRESULT WINAPI CWin32App::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_NCHITTEST:
		{
			POINT pt; pt.x = LOWORD(lParam); pt.y = HIWORD(lParam);
			::ScreenToClient(hWnd, &pt);


			ZY_TRACEA("%d %d\n",pt.x,pt.y);

			RECT rcClient;
			::GetClientRect(hWnd, &rcClient);

			if( !::IsZoomed(hWnd) )
			{
				int nborder = 5;
				int nLeft = nborder;
				int nTop = nborder;
				int nRight = nborder;
				int nBottom = nborder;
				RECT rcSizeBox = {nLeft,nTop,nRight,nBottom};
				if( pt.y < rcClient.top + rcSizeBox.top )
				{
					if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
					if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
					return HTTOP;
				}
				else if( pt.y > rcClient.bottom - rcSizeBox.bottom )
				{
					if( pt.x < rcClient.left + rcSizeBox.left ) 
						return HTBOTTOMLEFT;
					if( pt.x > rcClient.right - rcSizeBox.right ) 
						return HTBOTTOMRIGHT;
					return HTBOTTOM;
				}

				if( pt.x < rcClient.left + rcSizeBox.left ) 
					return HTLEFT;
				if( pt.x > rcClient.right - rcSizeBox.right ) 
					return HTRIGHT;
			}

			RECT rcCaption = {0,0,rcClient.right,30};
			if (PtInRect(&rcCaption,pt))
				return HTCAPTION;
			return HTCLIENT;
		}
		break;
	case WM_CREATE:
		{
			::SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE)|WS_EX_LAYERED);   
			SetLayeredWindowAttributes(hWnd,0,255/*(255*70)/150*/,LWA_ALPHA);  
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_PAINT:
		{
			RECT rtClient;
			GetClientRect(m_hWnd,&rtClient);

			HDC hdc = GetDC(m_hWnd);
			//SetBkMode(dc,TRANSPARENT);
			HDC Memdc=CreateCompatibleDC(hdc);
			HBITMAP hBitMap=CreateCompatibleBitmap(hdc,rtClient.right-rtClient.left,rtClient.bottom-rtClient.top);
			SelectObject(Memdc,hBitMap);
			SetBkMode(Memdc,TRANSPARENT);
			HBRUSH br = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
			FillRect(Memdc,&rtClient,br);
			DeleteObject(br);
			
			FontFamily fontFamily(L"Arial Black");
			StringFormat strformat;
			wchar_t pszbuf[][80]={
			{L"Hello everyone!"},
			{L"中文测试！123"}
			};

			GraphicsPath path;
			path.AddString(pszbuf[0],wcslen(pszbuf[0]),&fontFamily,
				FontStyleRegular,38,Point(10,10),&strformat);

			path.AddString(pszbuf[1],wcslen(pszbuf[1]),&fontFamily,
				FontStyleRegular,38,Point(100,60),&strformat);

			Pen pen(Color(200,215,0,0),3);

			// 绘制文字路径
			Graphics graphics(Memdc);
			graphics.SetSmoothingMode(SmoothingModeAntiAlias);
			graphics.DrawPath(&pen,&path);
			
			// 填充背景色
			SolidBrush brush(Color(200,228,228,228));
			graphics.FillRectangle(&brush,3,5,750,90);


			// 填充路径颜色，这里是文字的颜色
			LinearGradientBrush linGrBrush(
				Point(0,0),Point(0,90), 
				Color(255,0,255,255),
				Color(255,30,120,195));
			graphics.FillPath(&linGrBrush,&path);


			// 绘制圆角矩形
			GraphicsPath path2;
			RECT rt = {50,120,200,270};
			addRoundRect(path2,rt,40);

			HatchBrush myHatchBrush(
				HatchStyleWave,  // 50多种阴影
				Color(200, 0, 0, 255), // 阴影的颜色
				Color(200, 0, 255, 0)); // 填充背景色
			graphics.FillPath(&myHatchBrush,&path2);


			::BitBlt(hdc,0,0,rtClient.right-rtClient.left,rtClient.bottom-rtClient.top,
				Memdc,0,0,SRCCOPY);
			DeleteObject(hBitMap);
			DeleteDC(Memdc);
			ReleaseDC(m_hWnd,hdc);
		}
		break;
	case WM_KEYUP:
		if(wParam == VK_ESCAPE) PostQuitMessage(0);
		break;

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
