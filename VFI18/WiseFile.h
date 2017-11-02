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

// strings
static const std::wstring strVOS_UNKNOWN(L"VOS_UNKNOWN: 0x%08x");
static const std::wstring strVOS_DOS(L"VOS_DOS");
static const std::wstring strVOS_OS216(L"VOS_OS216");
static const std::wstring strVOS_OS232(L"VOS_OS232");
static const std::wstring strVOS_NT(L"VOS_NT");
static const std::wstring strVOS__WINDOWS16(L"VOS__WINDOWS16");
static const std::wstring strVOS__PM16(L"VOS__PM16");
static const std::wstring strVOS__PM32(L"VOS__PM32");
static const std::wstring strVOS__WINDOWS32(L"VOS__WINDOWS32");
static const std::wstring strVOS_OS216_PM16(L"VOS_OS216_PM16");
static const std::wstring strVOS_OS232_PM32(L"VOS_OS232_PM32");
static const std::wstring strVOS_DOS_WINDOWS16(L"VOS_DOS_WINDOWS16");
static const std::wstring strVOS_DOS_WINDOWS32(L"VOS_DOS_WINDOWS32");
static const std::wstring strVOS_NT_WINDOWS32(L"VOS_NT_WINDOWS32");
static const std::wstring strVOS_RESERVED(L"Reserved: 0x%08x");

//
static const std::wstring strVFT_UNKNOWN(L"VFT_UNKNOWN: 0x%08x");
static const std::wstring strVFT_APP(L"VFT_APP");
static const std::wstring strVFT_DLL(L"VFT_DLL");
static const std::wstring strVFT_DRV(L"VFT_DRV");
static const std::wstring strVFT_FONT(L"VFT_FONT");
static const std::wstring strVFT_VXD(L"VFT_VXD");
static const std::wstring strVFT_STATIC_LIB(L"VFT_STATIC_LIB");
static const std::wstring strVFT_RESERVED(L"Reserved: 0x%08x");

class CWiseFile
{
public:
	// construction, destruction
	CWiseFile();
	CWiseFile(const CWiseFile& rwf);
	CWiseFile(std::wstring& strFile);

	~CWiseFile();

	// initialization, release
	int Attach();
	bool Attach(std::wstring& strFile);
	int Detach();

	int ReadVersionInfoEx();
	bool GetShortPath(std::wstring& strDest);

	bool GetFieldString(std::wstring& strDest, int iField, bool fOptions);

	const CWiseFile& Copy(const CWiseFile& rwf);
	const CWiseFile& operator=(const CWiseFile& rwf);
	bool operator==(const CWiseFile& rwf);

	// access members
	void GetFullPath(std::wstring& strDest)
	{
		strDest = _strFullPath;
	}

	std::wstring GetFullPath()
	{
		return _strFullPath;
	}

	int GetPath(std::wstring& strDest)
	{
		strDest = _strPath;
		return FWF_SUCCESS;
	}

	std::wstring GetPath()
	{
		return _strPath;
	}

	int GetName(std::wstring strDest)
	{
		strDest = _strName;
		return FWF_SUCCESS;
	}

	std::wstring GetName()
	{
		return _strName;
	}

	int GetExt(std::wstring strDest)
	{
		strDest = _strExt;
		return FWF_SUCCESS;
	}

	std::wstring GetExt()
	{
		return _strExt;
	}

	void GetShortName(std::wstring strDest)
	{
		strDest = _strShortName;
	}
	
	std::wstring GetShortName()
	{
		return _strShortName;
	}

	int GetSize(std::wstring& strDest, bool bHex = false);

	DWORD GetSize()
	{
		return LODWORD(m_qwSize);
	}

	int GetSize64(std::wstring& strDest, bool bHex = false);

	QWORD GetSize64()
	{
		return m_qwSize;
	}

	int GetAttribs(std::wstring& strDest);
	DWORD GetAttribs()
	{
		return m_dwAttribs;
	}

	int GetDateCreation(std::wstring& strDest, bool fLocal = true);
	LPSYSTEMTIME GetDateCreation(bool fLocal = true);
	int GetDateLastAccess(std::wstring& strDest, bool fLocal = true);
	LPSYSTEMTIME GetDateLastAccess(bool fLocal = true);
	int GetDateLastWrite(std::wstring& strDest, bool fLocal = true);
	LPSYSTEMTIME GetDateLastWrite(bool fLocal = true);

	int GetTimeCreation(std::wstring& strDest, bool fLocal = true);
	LPSYSTEMTIME GetTimeCreation(bool fLocal = true);
	int GetTimeLastAccess(std::wstring& strDest, bool fLocal = true);
	LPSYSTEMTIME GetTimeLastAccess(bool fLocal = true);
	int GetTimeLastWrite(std::wstring& strDest, bool fLocal = true);
	LPSYSTEMTIME GetTimeLastWrite(bool fLocal = true);

	int GetFileVersion(std::wstring& strDest);
	QWORD GetFileVersion()
	{
		return m_qwFileVersion;
	}
	int GetFileVersion(LPDWORD pdwHigh, LPDWORD pdwLow);
	int GetFileVersion(LPWORD pwHighMS, LPWORD pwLowMS, LPWORD pwHighLS, LPWORD pwLowLS);

	int GetProductVersion(std::wstring& strDest);
	QWORD GetProductVersion()
	{
		return m_qwProductVersion;
	}
	int GetProductVersion(LPDWORD pdwMS, LPDWORD pdwLS);
	int GetProductVersion(LPWORD pwHighMS, LPWORD pwLowMS, LPWORD pwHighLS, LPWORD pwLowLS);

	int GetLanguage(std::wstring& strDest, bool bNumeric = false);
	WORD GetLanguage()
	{
		return m_wLanguage;
	}

	int GetCodePage(std::wstring& strDest, bool bNumeric = false);
	const TCodePage& GetCodePage()
	{
		return m_CodePage;
	}

	int GetFullLanguage(std::wstring& strDest, bool bNumeric = false);

	int GetOSString(std::wstring& strDest);
	void GetOSString();
	DWORD GetOS()
	{
		return m_dwOS;
	}

	int GetTypeString(std::wstring& strDest);
	void GetTypeString();
	DWORD GetType()
	{
		return m_dwType;
	}

	int GetFlags(std::wstring& strDest);
	DWORD GetFlags()
	{
		return m_dwFlags;
	}

	int GetCRC(std::wstring& strDest, bool bHex = true);
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
	static bool GetLanguageName(std::wstring& strDest, UINT Language);

private:
	bool Attach(LPCWSTR pszFileName);
	bool Init();
	// Member Variables
	// Set when a file is added

	std::wstring _strFullPath;
	std::wstring _strPath;
	std::wstring _strName;
	std::wstring _strExt;
	std::wstring _strShortName;

	// Set when information requested, if not set
	std::wstring _strAttribs;
	std::wstring _strFlags;
	std::wstring _strOS;
	std::wstring _strType;

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