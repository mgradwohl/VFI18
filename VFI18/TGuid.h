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

#include <objbase.h>
#include "trace.h"

#ifdef _DEBUG
	#ifdef DEBUG_NEW
		#define new DEBUG_NEW
		#ifndef THIS_FILE
			static WCHAR THIS_FILE[] = __FILE__;
		#endif
	#endif
#endif

class TGuid  
{
public:
	TGuid()
	{
		m_guid = GUID_NULL;
		::ZeroMemory(m_szGuid, 1024 * sizeof(WCHAR));
	}

	virtual ~TGuid()
	{
		::CoUninitialize();
	}

	bool Create()
	{
		if (::CoInitialize(NULL) != S_OK)
			return false;

		return (S_OK == ::CoCreateGuid(&m_guid));
	}

	void GetString(LPWSTR pszGuid)
	{
		if (m_szGuid[0] == NULL)
		{
			SetString();
		}

		lstrcpy(pszGuid, m_szGuid);
	}

	LPCWSTR GetString()
	{
		if (m_szGuid[0] == NULL)
		{
			SetString();
		}

		return m_szGuid;
	}

	GUID GetGuid()
	{
		return m_guid;
	}


private:
	GUID m_guid;
	WCHAR m_szGuid[1024 * sizeof(WCHAR)];

	void SetString()
	{
		WCHAR szFmt[104];
		lstrcpy(szFmt, (LPCWSTR) L"{%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}");
		wsprintf(m_szGuid, szFmt, 
			m_guid.Data1, m_guid.Data2, m_guid.Data3, 
			m_guid.Data4[0], m_guid.Data4[1], m_guid.Data4[2], m_guid.Data4[3],
			m_guid.Data4[4], m_guid.Data4[5], m_guid.Data4[6], m_guid.Data4[7]);
	}
};
