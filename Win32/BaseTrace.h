#ifndef BASETRACE_H__

#define BASETRACE_H__

#ifdef _DEBUG
#include <stdio.h>
#include <assert.h>

#define ZY_ASSERT(x)		assert(x)
#define ZY_VERIFY(x)		assert(x)
#define ZY_TRACE			ZYDebugTrace
#define ZY_TRACEA			ZYDebugTraceA

void __inline __cdecl ZYDebugTraceA(char* pszFormat, ...)
{
	char szBuffer[1024];
	va_list arglist;
	va_start(arglist, pszFormat);
	ZY_VERIFY(vsprintf(szBuffer, pszFormat, arglist) < _countof(szBuffer));
	OutputDebugStringA(szBuffer);
	va_end(arglist);
}

void __inline __cdecl ZYDebugTrace(LPCTSTR pszFormat, ...)
{
	TCHAR szBuffer[1024];
	va_list arglist;
	va_start(arglist, pszFormat);
#ifdef WIN32
	ZY_VERIFY(wvsprintf(szBuffer, pszFormat, arglist) < _countof(szBuffer));
	OutputDebugString(szBuffer);
#else
	ZY_VERIFY(vsprintf(szBuffer, pszFormat, arglist) < _countof(szBuffer));
	fprintf(stderr, szBuffer);
#endif
	va_end(arglist);
}

#else // !_DEBUG

#define ZY_ASSERT(x)
#define ZY_VERIFY(x)		(x)
#define ZY_TRACE			(void)
#define ZY_TRACEA			(void)

#endif // _DEBUG





#endif  //BASETRACE_H__