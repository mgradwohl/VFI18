// Visual File Information
// Copyright (c) Microsoft Corporation
// All rights reserved. 
// 
// MIT License
// 
// Permission is hereby granted, free of charge, to any person obtaining 
// a copy of this software and associated documentation files (the ""Software""), 
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom 
// the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included 
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS 
// OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR 
// IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#ifdef _DEBUG
	#ifdef DEBUG_NEW
		#define new DEBUG_NEW
	#endif
#endif

#include "trace.h"
#include "tguid.h"

class TEvent  
{
public:
	TEvent()
	{
		m_hEvent = NULL;
	}

	virtual ~TEvent()
	{
		if (NULL != m_hEvent)
		{
			TRACE (L"TEvent::~TEvent warning: Closing.\r\n");
			Close();
		}
	}

	operator HANDLE() const
	{
		return m_hEvent;
	}

	bool Create(LPCWSTR pszName)
	{
		if (NULL == pszName || L'\0' == *pszName)
		{
			return false;
		}

		if (NULL != m_hEvent)
		{
			TRACE(L"TEvent::Create warning: Event handle already used.\r\n");
			return false;
		}
		
		// no security
		// manual reset
		// initially off
		m_hEvent = ::CreateEvent(NULL, TRUE, FALSE, pszName);

		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			TRACE(L"TEvent::Create warning: Event already exists.\r\n");
		 	return false;
		}

		TRACE(L"TEvent::Create: Event created successfully.\r\n");
		return (NULL != m_hEvent);
	}

	bool Create()
	{
		TGuid guid;
		if (!guid.Create())
			return false;

		return Create(guid.GetString());
	}

	bool Close()
	{
		if (NULL == m_hEvent)
		{
			TRACE(L"TEvent::Close warning: NULL Event.\r\n");
			return false;
		}

		if (! ::CloseHandle (m_hEvent))
		{
			TRACE(L"TEvent::Close warning: CloseHandle failed.\r\n");
			return false;
		}
		TRACE(L"TEvent::Closed\r\n");

		m_hEvent = NULL;
		return true;
	}

	bool Signaled()
	{
		if (NULL == m_hEvent)
		{
			TRACE(L"TEvent::Signaled warning: NULL Event.\r\n");
			return false;
		}

		return (WAIT_OBJECT_0 == ::WaitForSingleObject(m_hEvent, 0));
	}

	bool Signal()
	{
		if (NULL == m_hEvent)
		{
			TRACE(L"TEvent::Signal warning: NULL Event.\r\n");
			return false;
		}

		return (TRUE == ::SetEvent(m_hEvent));
	}

	bool Reset()
	{
		if (NULL == m_hEvent)
		{
			TRACE(L"TEvent::Reset warning: NULL Event.\r\n");
			return false;
		}

		return (TRUE == ::ResetEvent(m_hEvent));
	}

	bool Signal(bool fSignal)
	{
		if (fSignal)
			return Signal();

		return Reset();
	}

	void Wait(DWORD dwMS)
	{
		if (NULL == m_hEvent)
		{
			TRACE(L"TEvent::Wait warning: NULL Event.\r\n");
			return;
		}

		::WaitForSingleObject(m_hEvent, dwMS);
	}

	void Wait()
	{
		Wait(INFINITE);
	}


private:
	HANDLE m_hEvent;
};