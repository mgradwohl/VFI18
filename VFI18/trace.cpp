#include "stdafx.h"
#include "trace.h"

#ifdef _DEBUG
bool _trace(wchar_t *format, ...)
{
	wchar_t buffer[1000];
	
	va_list argptr;
	va_start(argptr, format);
	wvsprintf(buffer, format, argptr);
	va_end(argptr);

	OutputDebugString(buffer);

	return true;
}
#endif