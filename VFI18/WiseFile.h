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

#include "tcodepage.h"

// file states
enum FileStates
{
	FWFS_VALID =		0x0000,	// constructed only
	FWFS_ATTACHED =		0x0001,	// attached to a file
	FWFS_VERSION =		0x0002,	// version information
	FWFS_CRC_PENDING =	0x0004,	// unused
	FWFS_CRC_WORKING =	0x0008,	// working on CRC
	FWFS_CRC_COMPLETE = 0x0010,	// CRC complete
	FWFS_CRC_ERROR =	0x0020,	// error generating CRC
	FWFS_SIZE =			0x0040,
	FWFS_DELETE =		0x0080,	// pending deletion
	FWFS_LANGUAGE =		0x0100,
	FWFS_INVALID =		0x1000		// invalid state
};

#define	FWF_SUCCESS				0
#define	FWF_ERR_NONE			0
#define	FWF_ERR_INVALID			-1;
#define FWF_ERR_BADPARAM		-2;
#define FWF_ERR_FOLDER			-3;
#define FWF_ERR_WILDCARD		-4;
#define FWF_ERR_SPECIALPATH		-5;
#define FWF_ERR_NOVERSION		-6
#define FWF_ERR_LOWMEMORY		-7;
#define FWF_ERR_OTHER			-64;

class CWiseFile
{

public:
	// construction, destruction
	CWiseFile();
	CWiseFile(const CWiseFile& rwf);
	CWiseFile(LPCWSTR pszFileSpec);
	~CWiseFile();

	const CWiseFile& Copy(const CWiseFile& rwf);
	const CWiseFile& operator=(const CWiseFile& rwf);
	bool operator==(const CWiseFile& rwf);

	// initialization, release
	int Attach(LPCWSTR pszFileName);
	int Detach();

	// handlers for listview
	LPWSTR GetFieldString(int iField, bool fOptions);

	int ReadVersionInfoEx();

	LPWSTR GetFullPath()
	{
		if (CheckState(FWFS_ATTACHED))
		{
			return m_szFullPath;
		}
		return L"\0";
	}

	LPWSTR GetPath()
	{
		if (CheckState(FWFS_ATTACHED))
		{
			return m_szPath;
		}
		return L"\0";
	}

	LPWSTR GetSize()
	{
		if (CheckState(FWFS_SIZE))
		{
			return m_szSize;
		}
		SetSize(false);
		return L"\0";
	}

	LPWSTR GetCRC()
	{
		if (CheckState(FWFS_CRC_COMPLETE))
		{
			SetCRC(true);
		}
		return m_szCRC;
	}

	LPWSTR GetOS()
	{
		if (CheckState(FWFS_VERSION))
		{
			SetOS();
		}
		return m_szOS;
	}

	LPWSTR GetType()
	{
		if (CheckState(FWFS_VERSION))
		{
			SetType();
		}
		return m_szType;
	}

	LPWSTR GetDateLastAccess()
	{
		if (CheckState(FWFS_ATTACHED))
		{
			return m_szDateLastAccess;
		}
		return L"\0";
	}

	LPWSTR GetTimeLastAccess()
	{
		if (CheckState(FWFS_ATTACHED))
		{
			return m_szTimeLastAccess;
		}
		return L"\0";
	}

	LPWSTR GetDateCreated()
	{
		if (CheckState(FWFS_ATTACHED))
		{
			return m_szDateCreated;
		}
		return L"\0";
	}

	LPWSTR GetTimeCreated()
	{
		if (CheckState(FWFS_ATTACHED))
		{
			return m_szTimeCreated;
		}
		return L"\0";
	}

	LPWSTR GetDateLastWrite()
	{
		if (CheckState(FWFS_ATTACHED))
		{
			return m_szDateLastWrite;
		}
		return L"\0";
	}

	LPWSTR GetTimeLastWrite()
	{
		if (CheckState(FWFS_ATTACHED))
		{
			return m_szTimeLastWrite;
		}
		return L"\0";
	}

	LPWSTR GetAttribs()
	{
		if (CheckState(FWFS_VERSION))
		{
			return m_szAttribs;
		}
		SetAttribs();
		return m_szAttribs;
	}

	LPWSTR GetFileVersion()
	{
		if (CheckState(FWFS_VERSION))
		{
			return m_szFileVersion;
		}

		if (FWF_SUCCESS == SetProductVersion() && FWF_SUCCESS == SetFileVersion())
		{
			OrState(FWFS_VERSION);
		}
		return m_szFileVersion;
	}

	LPWSTR GetProductVersion()
	{
		if (CheckState(FWFS_VERSION))
		{
			return m_szProductVersion;
		}

		if (FWF_SUCCESS == SetProductVersion() && FWF_SUCCESS == SetFileVersion())
		{
			OrState(FWFS_VERSION);
		}
		return m_szProductVersion;
	}

	LPWSTR GetLanguage()
	{
		if (CheckState(FWFS_LANGUAGE))
		{
			return m_szLanguage;
		}
		SetLanguage(m_wLanguage);
		return m_szLanguage;
	}

	LPWSTR GetCodePage()
	{
		if (CheckState(FWFS_VERSION))
		{
			return m_szCodePage;
		}
		SetCodePage(m_CodePage);
		return m_szCodePage;
	}

	LPWSTR GetFlags()
	{
		if (CheckState(FWFS_VERSION))
		{
			return m_szFlags;
			SetFlags();
		}
		SetFlags();
		return m_szFlags;
	}

	LPWSTR GetName()
	{
		if (CheckState(FWFS_ATTACHED))
		{
			return m_szName;
		}
		return L"\0";
	}

	LPWSTR GetExt()
	{
		if (CheckState(FWFS_ATTACHED))
		{
			return m_szExt;
		}
		return L"\0";
	}

	LPWSTR GetShortName()
	{
		if (CheckState(FWFS_ATTACHED))
		{
			return m_szShortName;
		}
		return L"\0";
	}

	int SetSize(bool bHex = false);
	int SetAttribs();

	int SetDateCreation(bool fLocal = true);
	int SetDateLastAccess(bool fLocal = true);
	int SetDateLastWrite(bool fLocal = true);

	int SetTimeCreation(bool fLocal = true);
	int SetTimeLastAccess(bool fLocal = true);
	int SetTimeLastWrite(bool fLocal = true);

	int SetFileVersion();
	int GetFileVersion(LPDWORD pdwHigh, LPDWORD pdwLow);
	int GetFileVersion(LPWORD pwHighMS, LPWORD pwLowMS, LPWORD pwHighLS, LPWORD pwLowLS);

	int SetProductVersion();
	int GetProductVersion(LPDWORD pdwMS, LPDWORD pdwLS);
	int GetProductVersion(LPWORD pwHighMS, LPWORD pwLowMS, LPWORD pwHighLS, LPWORD pwLowLS);

	bool SetLanguage(UINT Language);
	int SetCodePage(bool bNumeric = false);

	void SetOS();
	void SetType();
	int SetFlags();

	int SetCRC(bool bHex = true);

	void SetState(WORD wState)
	{
		m_wFileState = wState;
	}

	void OrState(WORD wState)
	{
		m_wFileState |= wState;
	}

	WORD GetState()
	{
		return m_wFileState;
	}

	bool CheckState(WORD wCheck)
	{
		return (0 != (m_wFileState & wCheck));
	}

	int TouchFileTime(FILETIME* lpTime);

	void _SetCRC(DWORD dwCRC)
	{
		m_dwCRC = dwCRC;
	}
	// execute helpers
	int ReadVersionInfo();

private:
	bool Init();
	// Member Variables
	// Set when a file is added
	wchar_t		m_szFullPath[_MAX_PATH];
	wchar_t		m_szPath[_MAX_PATH];
	wchar_t		m_szName[_MAX_PATH];
	wchar_t		m_szExt[_MAX_PATH];
	wchar_t		m_szShortName[14];

	// Set when information requested, if not set
	wchar_t		m_szAttribs[10];
	wchar_t		m_szFlags[256];
	wchar_t		m_szOS[64];
	wchar_t		m_szType[64];
	wchar_t		m_szSize[64];
	wchar_t		m_szDateCreated[64];
	wchar_t		m_szTimeCreated[64];
	wchar_t		m_szDateLastAccess[64];
	wchar_t		m_szTimeLastAccess[64];
	wchar_t		m_szDateLastWrite[64];
	wchar_t		m_szTimeLastWrite[64];
	wchar_t		m_szLanguage[64];
	wchar_t		m_szCodePage[64];
	wchar_t		m_szCRC[64];
	wchar_t		m_szFileVersion[64];
	wchar_t		m_szProductVersion[64];

	// from WIN32_FIND_DATA
	DWORD		m_dwAttribs;
	SYSTEMTIME	m_stUTCCreation;
	SYSTEMTIME	m_stUTCLastAccess;
	SYSTEMTIME	m_stUTCLastWrite;

	// set when requested, if not set
	SYSTEMTIME	m_stLocalCreation;
	SYSTEMTIME	m_stLocalLastAccess;
	SYSTEMTIME	m_stLocalLastWrite;

	QWORD		m_qwSize;
	QWORD		m_qwFileVersion;
	QWORD		m_qwProductVersion;
	WORD		m_wLanguage;
	TCodePage	m_CodePage;
	DWORD		m_dwOS;
	DWORD		m_dwType;
	DWORD		m_dwFlags;

	DWORD		m_dwCRC;
	WORD		m_wFileState;

	// Flags
	bool m_fDebugStripped;
	bool m_fHasVersion;
	bool m_fszOS;
	bool m_fszType;
	bool m_fszFlags;
	bool m_fszAttribs;
};