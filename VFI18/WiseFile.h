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
	FWFS_VALID = 0x0000,	// constructed only
	FWFS_ATTACHED = 0x0001,	// attached to a file
	FWFS_VERSION = 0x0002,	// version information
	FWFS_CRC_PENDING = 0x0004,	// unused
	FWFS_CRC_WORKING = 0x0008,	// working on CRC
	FWFS_CRC_COMPLETE = 0x0010,	// CRC complete
	FWFS_CRC_ERROR = 0x0020,	// error generating CRC
	FWFS_DELETE = 0x0040,	// pending deletion
	FWFS_INVALID = 0x0080		// invalid state
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
	int ReadVersionInfoEx();
	bool GetShortPath(LPWSTR pszBuf, int cchBuf);
	LPWSTR GetFieldString(int iField, bool fOptions);
	bool GetFieldString(LPWSTR pszBuf, int iField, bool fOptions);

	// construction, destruction
	CWiseFile();
	CWiseFile(const CWiseFile& rwf);
	CWiseFile(LPCWSTR pszFileSpec);
	~CWiseFile();

	// initialization, release
	int Attach(LPCWSTR pszFileName);
	int Detach();

	const CWiseFile& Copy(const CWiseFile& rwf);
	const CWiseFile& operator=(const CWiseFile& rwf);
	bool operator==(const CWiseFile& rwf);

	// access members
	int GetFullPath(LPWSTR pszText)
	{
		wcscpy_s(pszText, MAX_PATH, m_szFullPath);
		return FWF_SUCCESS;
	}

	LPWSTR szGetFullPath()
	{
		return m_szFullPath;
	}

	int GetPath(LPWSTR pszText)
	{
		pszText = m_szPath;
		return FWF_SUCCESS;
	}

	LPWSTR szGetPath()
	{
		return m_szPath;
	}

	LPWSTR szGetSize()
	{
		return m_szSize;
	}

	LPWSTR szGetCRC()
	{
		return m_szCRC;
	}

	LPWSTR szGetOS()
	{
		return m_szOS;
	}

	LPWSTR szGetType()
	{
		return m_szType;
	}

	LPWSTR szGetDateLastAccess()
	{
		return m_szDateLastAccess;
	}
	LPWSTR szGetTimeLastAccess()
	{
		return m_szTimeLastAccess;
	}
	LPWSTR szGetDateCreated()
	{
		return m_szDateCreated;
	}
	LPWSTR szGetTimeCreated()
	{
		return m_szTimeCreated;
	}
	LPWSTR szGetDateLastWrite()
	{
		return m_szDateLastWrite;
	}
	LPWSTR szGetTimeLastWrite()
	{
		return m_szTimeLastWrite;
	}
	LPWSTR szGetSize64()
	{
		return m_szSize;
	}
	LPWSTR szGetAttribs()
	{
		return m_szAttribs;
	}
	LPWSTR szGetFileVersion()
	{
		return m_szFileVersion;
	}
	LPWSTR szGetProductVersion()
	{
		return m_szProductVersion;
	}
	LPWSTR szGetLanguage()
	{
		return m_szLanguage;
	}
	LPWSTR szGetCodePage()
	{
		return m_szCodePage;
	}
	LPWSTR szGetFlags()
	{
		return m_szFlags;
	}

	int GetName(LPWSTR pszText)
	{
		wcscpy_s(pszText, MAX_PATH, m_szName);
		return FWF_SUCCESS;
	}

	LPWSTR szGetName()
	{
		return m_szName;
	}

	int GetExt(LPWSTR pszText)
	{
		wcscpy_s(pszText, MAX_PATH, m_szExt);
		return FWF_SUCCESS;
	}

	LPWSTR szGetExt()
	{
		return m_szExt;
	}

	int GetShortName(LPWSTR pszText)
	{
		wcscpy_s(pszText, MAX_PATH, m_szShortName);
		return FWF_SUCCESS;
	}

	LPWSTR szGetShortName()
	{
		return m_szShortName;
	}

	int GetSize(LPWSTR pszText, bool bHex = false);
	DWORD GetSize()
	{
		return LODWORD(m_qwSize);
	}

	int GetSize64(LPWSTR pszText, bool bHex = false);
	QWORD GetSize64()
	{
		return m_qwSize;
	}

	int GetAttribs(LPWSTR pszText);
	DWORD GetAttribs()
	{
		return m_dwAttribs;
	}

	int GetDateCreation(LPWSTR pszText, bool fLocal = true);
	LPSYSTEMTIME GetDateCreation(bool fLocal = true);
	int GetDateLastAccess(LPWSTR pszText, bool fLocal = true);
	LPSYSTEMTIME GetDateLastAccess(bool fLocal = true);
	int GetDateLastWrite(LPWSTR pszText, bool fLocal = true);
	LPSYSTEMTIME GetDateLastWrite(bool fLocal = true);

	int GetTimeCreation(LPWSTR pszText, bool fLocal = true);
	LPSYSTEMTIME GetTimeCreation(bool fLocal = true);
	int GetTimeLastAccess(LPWSTR pszText, bool fLocal = true);
	LPSYSTEMTIME GetTimeLastAccess(bool fLocal = true);
	int GetTimeLastWrite(LPWSTR pszText, bool fLocal = true);
	LPSYSTEMTIME GetTimeLastWrite(bool fLocal = true);

	int GetFileVersion(LPWSTR pszText);
	QWORD GetFileVersion()
	{
		return m_qwFileVersion;
	}
	int GetFileVersion(LPDWORD pdwHigh, LPDWORD pdwLow);
	int GetFileVersion(LPWORD pwHighMS, LPWORD pwLowMS, LPWORD pwHighLS, LPWORD pwLowLS);

	int GetProductVersion(LPWSTR pszText);
	QWORD GetProductVersion()
	{
		return m_qwProductVersion;
	}
	int GetProductVersion(LPDWORD pdwMS, LPDWORD pdwLS);
	int GetProductVersion(LPWORD pwHighMS, LPWORD pwLowMS, LPWORD pwHighLS, LPWORD pwLowLS);

	bool GetLanguageName(UINT Language, LPWSTR pszBuf);
	int GetLanguage(LPWSTR pszText, bool bNumeric = false);
	WORD GetLanguage()
	{
		return m_wLanguage;
	}

	int GetCodePage(LPWSTR pszText, bool bNumeric = false);
	const TCodePage& GetCodePage()
	{
		return m_CodePage;
	}

	int GetFullLanguage(LPWSTR pszText, bool bNumeric = false);

	int GetOSString(LPWSTR pszText);
	void GetOSString();
	DWORD GetOS()
	{
		return m_dwOS;
	}

	int GetTypeString(LPWSTR pszText);
	void GetTypeString();
	DWORD GetType()
	{
		return m_dwType;
	}

	int GetFlags(LPWSTR pszText);
	DWORD GetFlags()
	{
		return m_dwFlags;
	}

	int GetCRC(LPWSTR pszText, bool bHex = true);
	DWORD GetCRC()
	{
		return m_dwCRC;
	}

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