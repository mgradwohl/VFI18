#pragma once

#ifdef _DEBUG
	bool _trace(wchar_t *format, ...);
	#ifndef TRACE
		#define TRACE _trace
	#endif
#elif
	#define TRACE __noop
#endif
