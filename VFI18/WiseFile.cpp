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


#include "stdafx.h"
#include "resource.h"
#include "WiseFile.h"
#include "Imagehlp.h"

#include <cwchar>
#include <locale>
#include <codecvt>

// CWiseFile::CWiseFile
CWiseFile::CWiseFile()
{
	Init();

	SetState(FWFS_VALID);
}

CWiseFile::CWiseFile(const CWiseFile& rwf)
{
	Copy(rwf);
}

// CWiseFile::CWiseFile
// Param:   LPCWSTR pszFileSpec
CWiseFile::CWiseFile(std::wstring& strFile)
{
	Init();

	if (!Attach(strFile))
	{
		Init();
	}

	SetState(FWFS_VALID);
}

// CWiseFile::~CWiseFile
CWiseFile::~CWiseFile()
{
	SetState(FWFS_INVALID);
}

// CWiseFile::Attach
// Returns: bool 
// Param:   LPCWSTR pszFileSpec
bool CWiseFile::Attach(std::wstring& strFile)
{
	if (0 == _wcsnicmp(strFile.c_str(), L"\\\\?\\", 4))
	{
		return false;
	}

	WIN32_FIND_DATA fd;
	zero(fd);

	HANDLE hff = FindFirstFile(strFile.c_str(), &fd);
	if (INVALID_HANDLE_VALUE == hff)
	{
		return false;
	}

	m_dwAttribs = fd.dwFileAttributes;
	if (m_dwAttribs & FILE_ATTRIBUTE_DIRECTORY)
	{
		FindClose(hff);
		return false;
	}

	m_qwSize = GetFileSize64(strFile.c_str());

	FILETIME ft;
	::FileTimeToSystemTime( &(fd.ftCreationTime), &m_stUTCCreation );
	::FileTimeToLocalFileTime( &(fd.ftCreationTime), &ft );
	::FileTimeToSystemTime( &ft, &m_stLocalCreation );

	::FileTimeToSystemTime( &(fd.ftLastAccessTime), &m_stUTCLastAccess);
	::FileTimeToLocalFileTime( &(fd.ftLastAccessTime), &ft);
	::FileTimeToSystemTime( &ft, &m_stLocalLastAccess );

	::FileTimeToSystemTime( &(fd.ftLastWriteTime), &m_stUTCLastWrite);
	::FileTimeToLocalFileTime( &(fd.ftLastWriteTime), &ft);
	::FileTimeToSystemTime( &ft, &m_stLocalLastWrite );

	std::wstring strDrive;
	std::wstring strDir;
	std::wstring strExt;

	strDrive.resize(strFile.size());
	strDir.resize(strFile.size());
	strExt.resize(strFile.size());
	_strName.resize(strFile.size());

	_strFullPath = strFile;

	if (!_wsplitpath_s(_strFullPath.c_str(),
		&strDrive[0], strDrive.capacity(),
		&strDir[0], strDir.capacity(),
		&_strName[0], _strName.capacity(),
		&strExt[0], strExt.capacity()))
	{
		TRACE(L"splitpath failed\r\n");
	}

	_strExt = strExt.substr(1, strExt.length() - 1);
//	_strExt = strExt.substr(1, std::wstring::npos);
	
	_strPath.assign(strDrive);
	_strPath.append(strDir);
	_strShortName.assign(fd.cAlternateFileName);

	if (0 != FindNextFile(hff, &fd))
	{
		FindClose(hff);
		return false;
	}

	FindClose(hff);
	SetState(FWFS_ATTACHED);

	return true;
}

int CWiseFile::Attach()
{
	return Attach( GetFullPath() );
}

const CWiseFile& CWiseFile::Copy(const CWiseFile& rwf)
{
	m_dwAttribs = rwf.m_dwAttribs;
	m_dwCRC = rwf.m_dwCRC;
	m_dwFlags = rwf.m_dwFlags;
	m_dwOS = rwf.m_dwOS;
	m_dwType = rwf.m_dwType;
	m_qwFileVersion = rwf.m_qwFileVersion;
	m_qwProductVersion = rwf.m_qwProductVersion;
	m_qwSize = rwf.m_qwSize;
	m_stLocalCreation = rwf.m_stLocalCreation;
	m_stLocalLastAccess = rwf.m_stLocalLastAccess;
	m_stLocalLastWrite = rwf.m_stLocalLastWrite;
	m_stUTCCreation = rwf.m_stUTCCreation;
	m_stUTCLastAccess = rwf.m_stUTCLastAccess;
	m_stUTCLastWrite = rwf.m_stUTCLastWrite;
	_strAttribs = rwf._strAttribs;
	_strExt = rwf._strExt;
	_strFlags = rwf._strFlags;
	_strFullPath = rwf._strFullPath;
	_strShortName = rwf._strShortName;

	m_CodePage = rwf.m_CodePage;
	m_wFileState = rwf.m_wFileState;
	m_wLanguage = rwf.m_wLanguage;
	
	return *this;
}

const CWiseFile& CWiseFile::operator=(const CWiseFile& rwf)
{
	return Copy(rwf);
}

bool CWiseFile::operator==(const CWiseFile& rwf)
{
	if (CheckState(FWFS_CRC_COMPLETE))
	{
		return m_dwCRC == rwf.m_dwCRC;
	}

	if (CheckState(FWFS_VERSION))
	{
		return (0 == (_strFullPath == rwf._strFullPath) && (m_qwFileVersion == rwf.m_qwFileVersion ));
	}

	if (CheckState(FWFS_ATTACHED))
	{
		return (0 == (_strFullPath == rwf._strFullPath));
	}

	return false;
}

// CWiseFile::Detach
// Returns: int
int CWiseFile::Detach()
{
	// does nothing
	SetState(FWFS_INVALID);
	return FWF_SUCCESS;
}

int CWiseFile::GetSize(std::wstring& strDest, bool bHex)
{
	if (!CheckState(FWFS_ATTACHED))
	{
		strDest.clear();
		return FWF_ERR_INVALID;
	}

	if (bHex)
	{
		swprintf_s(&strDest[0], strDest.capacity(), L"%08x", LODWORD(m_qwSize));
		return FWF_SUCCESS;
	}
	else
	{
		// Not using StrFormatByteSize because it wordifies everything
		if (int2wstr(strDest, LODWORD(m_qwSize)))
		{
			return FWF_SUCCESS;
		}
		else
		{
			return FWF_ERR_OTHER;
		}
	}
}

int CWiseFile::GetSize64(std::wstring& strDest, bool bHex)
{
	if (!CheckState(FWFS_ATTACHED))
	{
		strDest.clear();
		return FWF_ERR_INVALID;
	}

	if (bHex)
	{
		swprintf_s(&strDest[0], strDest.capacity(), L"%16I64x", m_qwSize);
		return FWF_SUCCESS;
	}
	else
	{
		// Not using StrFormatByteSize because it wordifies everything
		if (int2wstr(strDest, m_qwSize))
		{
			return FWF_SUCCESS;
		}
		else
		{
			return FWF_ERR_OTHER;
		}
	}
}

int CWiseFile::GetCRC(std::wstring& strDest, bool bHex)
{
	if (!CheckState(FWFS_ATTACHED))
	{
		strDest.clear();
		return FWF_ERR_INVALID;
	}

	if (CheckState(FWFS_CRC_COMPLETE))
	{
		if (bHex)
		{
			swprintf_s(&strDest[0], strDest.capacity(), L"%08x", m_dwCRC);
		}
		else
		{
			int2wstr(strDest, m_dwCRC);
		}
		return FWF_SUCCESS;
	}

	if (CheckState(FWFS_CRC_ERROR))
	{
		LoadWstring(strDest, STR_CRC_ERROR);
		return FWF_SUCCESS;
	}

	if (CheckState(FWFS_CRC_PENDING))
	{
		LoadWstring(strDest, STR_CRC_PENDING);
		return FWF_SUCCESS;
	}

	if (CheckState(FWFS_CRC_WORKING))
	{
		LoadWstring(strDest, STR_CRC_WORKING);
		return FWF_SUCCESS;
	}

	strDest.clear();
	return FWF_ERR_OTHER;
}

int CWiseFile::GetFileVersion(std::wstring& strDest)
{
	if (!CheckState(FWFS_VERSION))
	{
		strDest.clear();
		return FWF_ERR_INVALID;
	}

	if (!m_fHasVersion)
	{
		strDest.clear();
		return FWF_SUCCESS;
	}

	std::wstring strDec;
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, &strDec[0], 2);

	swprintf_s(&strDest[0], strDest.capacity(), L"%d%s%02d%s%04d%s%04d",
		(int)HIWORD(HIDWORD(m_qwFileVersion)),
		strDec.c_str(),
		(int)LOWORD(HIDWORD(m_qwFileVersion)),
		strDec.c_str(),
		(int)HIWORD(LODWORD(m_qwFileVersion)),
		strDec.c_str(),
		(int)LOWORD(LODWORD(m_qwFileVersion)));

	return FWF_SUCCESS;
}

int CWiseFile::GetFileVersion(LPDWORD pdwMS, LPDWORD pdwLS)
{
	if (!CheckState(FWFS_VERSION))
		return FWF_ERR_INVALID;

	if(pdwMS)
		*pdwMS = HIDWORD(m_qwFileVersion);

	if(pdwLS)
		*pdwLS  = LODWORD(m_qwFileVersion);

	return FWF_SUCCESS;
}

int CWiseFile::GetFileVersion(LPWORD pwHighMS, LPWORD pwLowMS, LPWORD pwHighLS, LPWORD pwLowLS)
{
	if (!CheckState(FWFS_VERSION))
		return FWF_ERR_INVALID;

	if (pwHighMS)
		*pwHighMS = HIWORD(HIDWORD(m_qwFileVersion));

	if (pwLowMS)
		*pwLowMS  = LOWORD(HIDWORD(m_qwFileVersion));

	if (pwHighLS)
		*pwHighLS = HIWORD(LODWORD(m_qwFileVersion));

	if (pwLowLS)
		*pwLowLS  = LOWORD(LODWORD(m_qwFileVersion));

	return FWF_SUCCESS;
}

int CWiseFile::GetProductVersion(std::wstring& strDest)
{
	if (!CheckState(FWFS_VERSION))
	{
		strDest.clear();
		return FWF_ERR_INVALID;
	}

	if (!m_fHasVersion)
	{
		strDest.clear();
		return FWF_SUCCESS;
	}

	std::wstring strDec;
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, &strDec[0], 2);

	swprintf_s(&strDest[0], strDest.capacity(), L"%2d%s%02d%s%04d%s%04d",
		(int)HIWORD(HIDWORD(m_qwProductVersion)),
		strDec.c_str(),
		(int)LOWORD(HIDWORD(m_qwProductVersion)),
		strDec.c_str(),
		(int)HIWORD(LODWORD(m_qwProductVersion)),
		strDec.c_str(),
		(int)LOWORD(LODWORD(m_qwProductVersion)));

	return FWF_SUCCESS;
}

int CWiseFile::GetProductVersion(LPDWORD pdwMS, LPDWORD pdwLS)
{
	if (!CheckState(FWFS_VERSION))
		return FWF_ERR_INVALID;

	if(pdwMS)
		*pdwMS = HIDWORD(m_qwProductVersion);

	if(pdwLS)
		*pdwLS  = LODWORD(m_qwProductVersion);

	return FWF_SUCCESS;
}

int CWiseFile::GetProductVersion(LPWORD pwHighMS, LPWORD pwLowMS, LPWORD pwHighLS, LPWORD pwLowLS)
{
	if (!CheckState(FWFS_VERSION))
		return FWF_ERR_INVALID;

	if (pwHighMS)
		*pwHighMS = HIWORD(HIDWORD(m_qwProductVersion));

	if (pwLowMS)
		*pwLowMS  = LOWORD(HIDWORD(m_qwProductVersion));

	if (pwHighLS)
		*pwHighLS = HIWORD(LODWORD(m_qwProductVersion));

	if (pwLowLS)
		*pwLowLS  = LOWORD(LODWORD(m_qwProductVersion));

	return FWF_SUCCESS;
}

void CWiseFile::GetTypeString()
{
	if (!CheckState(FWFS_VERSION))
		return;
	
	if (!m_fHasVersion)
		return;

	switch (m_dwType)
	{
		case VFT_UNKNOWN: swprintf_s(&_strType[0], _strType.capacity(), strVFT_UNKNOWN.c_str(), m_dwType);
			break;
		case VFT_APP: _strType = strVFT_APP;
			break;
		case VFT_DLL: _strType = strVFT_DLL;
			break;
		case VFT_DRV: _strType = strVFT_DRV;
			break;
		case VFT_FONT: _strType = strVFT_FONT;
			break;
		case VFT_VXD: _strType = strVFT_VXD;
			break;
		case VFT_STATIC_LIB: _strType = strVFT_STATIC_LIB;
			break;
		default:
		{
			swprintf_s(&_strType[0], _strType.capacity(), strVFT_RESERVED.c_str(), m_dwType );
		}
	}
	m_fszType = true;
}

int CWiseFile::GetTypeString(std::wstring& strDest)
{
	if (!CheckState(FWFS_VERSION))
	{
		strDest.clear();
		return FWF_ERR_INVALID;
	}

	if (!m_fHasVersion)
		return FWF_ERR_INVALID;

	if (!m_fszType)
	{
		GetTypeString();
	}

	if (m_fszType)
	{
		strDest = _strType;
	}

	return FWF_SUCCESS;
}

void CWiseFile::GetOSString()
{
	if (!CheckState(FWFS_VERSION))
		return;

	if (!m_fHasVersion)
		return;

// turn these strings into const wstrings
// and use wcscpy
	switch(m_dwOS)
	{
		case VOS_UNKNOWN: swprintf_s(&_strOS[0], _strOS.capacity(), strVOS_UNKNOWN.c_str(), m_dwOS);
			break;
		case VOS_DOS: _strOS = strVOS_DOS;
			break;
		case VOS_OS216: _strOS = strVOS_OS216;
			break;
		case VOS_OS232: _strOS = strVOS_OS232;
			break;
		case VOS_NT: _strOS = strVOS_NT;
			break;
		case VOS__WINDOWS16: _strOS = strVOS__WINDOWS16;
			break;
		case VOS__PM16: _strOS = strVOS__PM16;
			break;
		case VOS__PM32: _strOS = strVOS__PM32;
			break;
		case VOS__WINDOWS32: _strOS = strVOS__WINDOWS32;
			break;
		case VOS_OS216_PM16: _strOS = strVOS_OS216_PM16;
			break;
		case VOS_OS232_PM32: _strOS = strVOS_OS232_PM32;
			break;
		case VOS_DOS_WINDOWS16: _strOS = strVOS_DOS_WINDOWS16;
			break;
		case VOS_DOS_WINDOWS32: _strOS = strVOS_DOS_WINDOWS32;
			break;
		case VOS_NT_WINDOWS32: _strOS = strVOS_NT_WINDOWS32;
			break;
		default:
		{
			swprintf_s(&_strOS[0], _strOS.capacity(), strVOS_RESERVED.c_str(), m_dwOS );
		}
	}
	m_fszOS = true;
}

int CWiseFile::GetOSString(std::wstring& strDest)
{
	if (!CheckState(FWFS_VERSION))
	{
		strDest.clear();
		return FWF_ERR_INVALID;
	}

	if (!m_fHasVersion)
	{
		strDest.clear();
		return FWF_SUCCESS;
	}

	if (!m_fszOS)
	{
		GetOSString();
	}

	if (m_fszOS)
	{
		strDest = _strOS;
	}

	return FWF_SUCCESS;
}

int CWiseFile::GetLanguage(std::wstring& strDest, bool bNumeric)
{
	if (!CheckState(FWFS_VERSION))
	{
		strDest.clear();
		return FWF_ERR_INVALID;
	}

	if (!m_fHasVersion)
	{
		strDest.clear();
		return FWF_SUCCESS;
	}

	if (bNumeric)
	{
		swprintf_s(&strDest[0], strDest.capacity(), L"%04x", (int)m_wLanguage);
	}
	else
	{
		GetLanguageName(strDest, m_wLanguage);
	}
	return FWF_SUCCESS;
}

int CWiseFile::GetCodePage(std::wstring& strDest, bool bNumeric)
{
	if (!CheckState(FWFS_VERSION))
	{
		strDest.clear();
		return FWF_ERR_INVALID;
	}

	if (!m_fHasVersion)
	{
		strDest.clear();
		return FWF_SUCCESS;
	}

	if (bNumeric)
	{
		swprintf_s(&strDest[0], strDest.capacity(), L"%04x", (WORD)m_CodePage);
	}
	else
	{
		m_CodePage.GetCodePageName(LANGIDFROMLCID(GetThreadLocale()), m_CodePage, &strDest[0]);
	}
	return FWF_SUCCESS;
}

int CWiseFile::GetFullLanguage(std::wstring& strDest, bool bNumeric)
{
	if (!CheckState(FWFS_VERSION))
	{
		strDest.clear();
		return FWF_ERR_INVALID;
	}

	if (!m_fHasVersion)
	{
		strDest.clear();
		return FWF_SUCCESS;
	}

	if (bNumeric)
	{
		swprintf_s(&strDest[0], strDest.capacity(), L"%04x, %04x", (int)m_wLanguage, (WORD)m_CodePage);
	}
	else
	{
		std::wstring strTemp;
		std::wstring strSep;

		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SLIST, &strSep[0], 2);
		strSep += L'\0';
		GetLanguageName(strTemp, m_wLanguage);

		swprintf_s(&strDest[0], strDest.capacity(), L"%s%s%s\0", strTemp.c_str(), strSep.c_str(), (LPCWSTR)m_CodePage);
	}
	return FWF_SUCCESS;
}

int CWiseFile::ReadVersionInfo()
{

	std::wstring wsPath(_strFullPath);

	//setup converter
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;

	//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
	std::string szPath = converter.to_bytes(wsPath);

	PLOADED_IMAGE pli;
	pli = ImageLoad(szPath.c_str(), NULL);
	if (NULL != pli)
	{
		if (pli->Characteristics & IMAGE_FILE_DEBUG_STRIPPED)
		{
			// debug info is stripped
			m_fDebugStripped = true;
		}
		else
		{
			m_fDebugStripped = false;
		}
		ImageUnload(pli);
	}

	DWORD	dwVerSize = 0;
	LPVOID	lpVerBuffer = NULL;
	LPVOID	lpVerData = NULL;
	UINT	cbVerData = 0;

	if (!GetShortPath(_strShortName))
	{
		return FWF_ERR_NOVERSION;
	}

	// this takes a long time to call, max size I've seen is 5476
	DWORD	dwVerHandle = 0;
	dwVerSize = ::GetFileVersionInfoSize(_strShortName.c_str(), &dwVerHandle);

	if (dwVerSize < 1)
		return FWF_ERR_NOVERSION;

	lpVerBuffer = (LPVOID) new byte[dwVerSize];

	if (NULL == lpVerBuffer )
	{
		delete [] lpVerBuffer;
		return FWF_ERR_LOWMEMORY;
	}

	ZeroMemory(lpVerBuffer, dwVerSize);
	if (! ::GetFileVersionInfo(_strShortName.c_str(), dwVerHandle, dwVerSize, lpVerBuffer))
	{
		delete [] lpVerBuffer;
		return FWF_ERR_NOVERSION;
	}

	if (! ::VerQueryValue(lpVerBuffer, L"\\", &lpVerData, &cbVerData))
	{
		delete [] lpVerBuffer;
		return FWF_ERR_NOVERSION;
	}

	m_fHasVersion = true;
	#define pVerFixedInfo ((VS_FIXEDFILEINFO FAR*)lpVerData)
	m_qwFileVersion = MAKEQWORD(pVerFixedInfo->dwFileVersionLS, pVerFixedInfo->dwFileVersionMS);
	m_qwProductVersion = MAKEQWORD(pVerFixedInfo->dwProductVersionLS, pVerFixedInfo->dwProductVersionMS);
	m_dwType = pVerFixedInfo->dwFileType;
	m_dwOS = pVerFixedInfo->dwFileOS;
	m_dwFlags = pVerFixedInfo->dwFileFlags;
	#undef pVerFixedInfo

	if (!VerQueryValue(lpVerBuffer, L"\\VarFileInfo\\Translation", &lpVerData, &cbVerData))
	{
		delete [] lpVerBuffer;
		return FWF_ERR_NOVERSION;
	}

	m_wLanguage = LOWORD(*(LPDWORD)lpVerData);
	m_CodePage = HIWORD(*(LPDWORD)lpVerData);
	delete [] lpVerBuffer;
	
	OrState(FWFS_VERSION);

	return FWF_SUCCESS;
}

int CWiseFile::GetDateCreation(std::wstring& strDest, bool fLocal)
{
	if (!CheckState(FWFS_ATTACHED))
	{
		strDest.clear();
		return FWF_ERR_INVALID;
	}

	SYSTEMTIME* pst = nullptr;
	if (fLocal)
	{
		pst = &m_stLocalCreation;
	}
	else
	{
		pst = &m_stUTCCreation;
	}

	if (0 == ::GetDateFormat(::GetThreadLocale(), DATE_SHORTDATE, pst, NULL, &strDest[0], 64))
	{
		return FWF_ERR_OTHER;
	}

	return FWF_SUCCESS;
}

int CWiseFile::GetDateLastAccess (std::wstring& strDest, bool fLocal)
{
	if (!CheckState(FWFS_ATTACHED))
	{
		strDest.clear();
		return FWF_ERR_INVALID;
	}

	SYSTEMTIME* pst = nullptr;
	if (fLocal)
	{
		pst = &m_stLocalLastAccess;
	}
	else
	{
		pst = &m_stUTCLastAccess;
	}

	if (0 == ::GetDateFormat(::GetThreadLocale(), DATE_SHORTDATE, pst, NULL, &strDest[0], 64 ))
	{
		return FWF_ERR_OTHER;
	}
	return FWF_SUCCESS;
}

int CWiseFile::GetDateLastWrite (std::wstring& strDest, bool fLocal)
{
	if (!CheckState(FWFS_ATTACHED))
	{
		strDest.clear();
		return FWF_ERR_INVALID;
	}

	SYSTEMTIME* pst = nullptr;
	if (fLocal)
	{
		pst = &m_stLocalLastWrite;
	}
	else
	{
		pst = &m_stUTCLastWrite;
	}

	if (0 == ::GetDateFormat(::GetThreadLocale(), DATE_SHORTDATE, pst, NULL, &strDest[0], 64 ))
	{
		return FWF_ERR_OTHER;
	}
	return FWF_SUCCESS;
}

LPSYSTEMTIME CWiseFile::GetDateCreation( bool fLocal )
{
	if (fLocal)
	{
		return &m_stLocalCreation;
	}
	return &m_stUTCCreation;
}

LPSYSTEMTIME CWiseFile::GetTimeCreation( bool fLocal )
{
	if (fLocal)
	{
		return &m_stLocalCreation;
	}
	return &m_stUTCCreation;
}

LPSYSTEMTIME CWiseFile::GetDateLastAccess(bool fLocal)
{
	if (fLocal)
	{
		return &m_stLocalLastAccess;
	}
	return &m_stUTCLastAccess;
}

LPSYSTEMTIME CWiseFile::GetTimeLastAccess(bool fLocal)
{
	if (fLocal)
	{
		return &m_stLocalLastAccess;
	}
	return &m_stUTCLastAccess;
}

LPSYSTEMTIME CWiseFile::GetDateLastWrite(bool fLocal)
{
	if (fLocal)
	{
		return &m_stLocalLastWrite;
	}
	return &m_stUTCLastWrite;
}

LPSYSTEMTIME CWiseFile::GetTimeLastWrite(bool fLocal)
{
	if (fLocal)
	{
		return &m_stLocalLastWrite;
	}
	return &m_stUTCLastWrite;
}

int CWiseFile::GetTimeCreation (std::wstring& strDest, bool fLocal)
{
	if (!CheckState(FWFS_ATTACHED))
	{
		strDest.clear();
		return FWF_ERR_INVALID;
	}

	SYSTEMTIME* pst = nullptr;
	if (fLocal)
	{
		pst = &m_stLocalCreation;
	}
	else
	{
		pst = &m_stUTCCreation;
	}

	if (0 == ::GetTimeFormat(::GetThreadLocale(), TIME_NOSECONDS, pst, NULL, &strDest[0], 64 ))
	{
			return FWF_ERR_OTHER;
	}
	return FWF_SUCCESS;
}

int CWiseFile::GetTimeLastAccess (std::wstring& strDest, bool fLocal)
{
	if (!CheckState(FWFS_ATTACHED))
	{
		strDest.clear();
		return FWF_ERR_INVALID;
	}

	SYSTEMTIME* pst = nullptr;
	if (fLocal)
	{
		pst = &m_stLocalLastAccess;
	}
	else
	{
		pst = &m_stUTCLastAccess;
	}

	if (0 == ::GetTimeFormat(::GetThreadLocale(), TIME_NOSECONDS, pst, NULL, &strDest[0], 64 ))
	{
		return FWF_ERR_OTHER;
	}
	return FWF_SUCCESS;
}

int CWiseFile::GetTimeLastWrite (std::wstring& strDest, bool fLocal)
{
	if (!CheckState(FWFS_ATTACHED))
	{
		strDest.clear();
		return FWF_ERR_INVALID;
	}

	SYSTEMTIME* pst = nullptr;
	if (fLocal)
	{
		pst = &m_stLocalLastWrite;
	}
	else
	{
		pst = &m_stUTCLastWrite;
	}

	if (0 == ::GetTimeFormat(::GetThreadLocale(), TIME_NOSECONDS, pst, NULL, &strDest[0], 64 ))
	{
		return FWF_ERR_OTHER;

	}
	return FWF_SUCCESS;
}

int CWiseFile::TouchFileTime(FILETIME* lpTime)
{
	if (!CheckState(FWFS_ATTACHED))
		return FWF_ERR_INVALID;

	if (NULL == lpTime)
		return FWF_ERR_BADPARAM;

	DWORD dwOldAttribs = ::GetFileAttributes(_strFullPath.c_str());
	if (FALSE == ::SetFileAttributes(_strFullPath.c_str(), FILE_ATTRIBUTE_NORMAL))
	{
		::SetFileAttributes(_strFullPath.c_str(), dwOldAttribs);
		return FWF_ERR_OTHER;
	}

	HANDLE hFile = ::CreateFile(_strFullPath.c_str(),
					   GENERIC_WRITE, 
					   FILE_SHARE_READ | FILE_SHARE_WRITE, 
					   NULL, 
					   OPEN_EXISTING, 
					   FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING, 
					   NULL);

	if(INVALID_HANDLE_VALUE == hFile)
	{
		::SetFileAttributes(_strFullPath.c_str(), dwOldAttribs);
		::CloseHandle(hFile);
		return FWF_ERR_OTHER;
	} 

	if (FALSE == ::SetFileTime(hFile, lpTime, lpTime, lpTime))
	{
		::SetFileAttributes(_strFullPath.c_str(), dwOldAttribs);
		::CloseHandle(hFile);
		return FWF_ERR_OTHER;
	}

	::SetFileAttributes(_strFullPath.c_str(), dwOldAttribs);
	::CloseHandle(hFile);

	FILETIME ft;
	::FileTimeToSystemTime( lpTime, &m_stUTCCreation );
	::FileTimeToLocalFileTime( lpTime, &ft );
	::FileTimeToSystemTime( &ft, &m_stLocalCreation );

	::FileTimeToSystemTime( lpTime, &m_stUTCLastAccess);
	::FileTimeToLocalFileTime( lpTime, &ft);
	::FileTimeToSystemTime( &ft, &m_stLocalLastAccess );

	::FileTimeToSystemTime( lpTime, &m_stUTCLastWrite);
	::FileTimeToLocalFileTime( lpTime, &ft);
	::FileTimeToSystemTime( &ft, &m_stLocalLastWrite );

	return true;
}

int CWiseFile::GetAttribs(std::wstring& strDest)
{
	if (!CheckState(FWFS_ATTACHED))
	{
		strDest = L"        ";
		return FWF_ERR_INVALID;
	}

	if (m_fszAttribs)
	{
		// the string is already built, return it
		strDest = _strAttribs;
		return FWF_SUCCESS;
	}

	if (m_dwAttribs & FILE_ATTRIBUTE_NORMAL)
	{
		_strAttribs = L"        ";
		m_fszAttribs = true;
		return FWF_SUCCESS;
	}

	swprintf_s(&_strAttribs[0], _strAttribs.capacity(),
		L"%c%c%c%c%c%c%c%c",
		((m_dwAttribs & FILE_ATTRIBUTE_ARCHIVE) ? 'A' : ' '),
		((m_dwAttribs & FILE_ATTRIBUTE_HIDDEN) ? 'H' : ' '),
		((m_dwAttribs & FILE_ATTRIBUTE_READONLY) ? 'R' : ' '),
		((m_dwAttribs & FILE_ATTRIBUTE_SYSTEM) ? 'S' : ' '),
		((m_dwAttribs & FILE_ATTRIBUTE_ENCRYPTED) ? 'E' : ' '),
		((m_dwAttribs & FILE_ATTRIBUTE_COMPRESSED) ? 'C' : ' '),
		((m_dwAttribs & FILE_ATTRIBUTE_TEMPORARY) ? 'T' : ' '),
		((m_dwAttribs & FILE_ATTRIBUTE_OFFLINE) ? 'O' : ' '));

	strDest = _strAttribs;
	m_fszAttribs = true;
	return FWF_SUCCESS;
}

int CWiseFile::GetFlags(std::wstring& strDest)
{
	if ( !CheckState(FWFS_VERSION) )
	{
		strDest.clear();
		return FWF_ERR_INVALID;
	}

	if (!m_fHasVersion)
	{
		strDest.clear();
		return FWF_SUCCESS;
	}

	if (m_fszFlags)
	{
		// the string is already built, return it
		strDest = _strFlags;
	}
	m_fszFlags = true;

	// list seperator
	std::wstring strSep;
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SLIST, &strSep[0], 2);
	strSep += L" ";

	std::wstring str;
	if (m_dwFlags & VS_FF_DEBUG)
	{
		if (m_fDebugStripped)
		{

			LoadWstring(str, STR_FLAG_DEBUG_STRIPPED);
		}
		else
		{

			LoadWstring(str, STR_FLAG_DEBUG);
		}

		_strFlags += str;
		_strFlags += strSep;
	}
	if (m_dwFlags & VS_FF_PRERELEASE)
	{

		LoadWstring(str, STR_FLAG_PRERELEASE);
		_strFlags += str;
		_strFlags += strSep;
	}
	if (m_dwFlags & VS_FF_PATCHED)
	{

		LoadWstring(str, STR_FLAG_PATCHED);
		_strFlags += str;
		_strFlags += strSep;
	}
	if (m_dwFlags & VS_FF_PRIVATEBUILD)
	{

		LoadWstring(str, STR_FLAG_PRIVATEBUILD);
		_strFlags += str;
		_strFlags += strSep;
	}
	if (m_dwFlags & VS_FF_INFOINFERRED)
	{
		LoadWstring(str, STR_FLAG_INFOINFERRED);
		_strFlags += str;
		_strFlags += strSep;
	}
	if (m_dwFlags & VS_FF_SPECIALBUILD)
	{
		LoadWstring(str, STR_FLAG_SPECIALBUILD);
		_strFlags += str;
		_strFlags += strSep;
	}

	size_t sep = strDest.find_last_of(strSep[0]);
	strDest.resize(sep);

	// if we've looked at it, and we still don't have a string for it
	// put a default one in (unless the flags are 00000000)
	if (_strFlags.length() < 3 && m_dwFlags != 0)
	{
		swprintf_s(&_strFlags[0], _strFlags.capacity(), L"%08x", m_dwFlags);
	}

	strDest = _strFlags;
	return FWF_SUCCESS;
}

bool CWiseFile::GetLanguageName(std::wstring& strDest, UINT Language)
{
	std::wstring strTemp;
	strTemp.reserve(255);

	if (0 == Language)
	{
		VerLanguageName(Language, &strTemp[0], 255);
		swprintf_s(&strDest[0], strDest.capacity(), L"%lu %s", Language, strTemp.c_str());
		return true;
	}

	if (0 == GetLocaleInfo(Language, LOCALE_SLANGUAGE, &strTemp[0], 255))
	{
		VerLanguageName(Language, &strTemp[0], 255);
	}

	swprintf_s(&strDest[0], strDest.capacity(), L"%lu %s", Language, strTemp.c_str());
	return true;
}

bool CWiseFile::GetFieldString(std::wstring& strDest, int iField, bool fOptions)
{
	switch (iField)
	{
		case  0: return (FWF_SUCCESS == GetPath(strDest));
		break;
		
		case  1: return (FWF_SUCCESS == GetName(strDest));
		break;
		
		case  2: return (FWF_SUCCESS == GetExt(strDest));
		break;
		
		case  3: return (FWF_SUCCESS == GetSize64(strDest));
		break;
		
		case  4: return (FWF_SUCCESS == GetDateCreation(strDest));
		break;
	
		case  5: return (FWF_SUCCESS == GetTimeCreation(strDest));
		break;
	
		case  6: return (FWF_SUCCESS == GetDateLastWrite(strDest));
		break;
	
		case  7: return (FWF_SUCCESS == GetTimeLastWrite(strDest));
		break;
	
		case  8: return (FWF_SUCCESS == GetDateLastAccess(strDest));
		break;

		case  9: return (FWF_SUCCESS == GetTimeLastAccess(strDest));
		break;

		case 10: return (FWF_SUCCESS == GetAttribs(strDest));
		break;

		case 11: return (FWF_SUCCESS == GetFileVersion(strDest));
		break;
	
		case 12: return (FWF_SUCCESS == GetProductVersion(strDest));
		break;
	
		case 13: return (FWF_SUCCESS == GetLanguage(strDest));
		break;
	
		case 14: return (FWF_SUCCESS == GetCodePage(strDest));
		break;

		case 15: return (FWF_SUCCESS == GetOSString(strDest));
		break;
	
		case 16: return (FWF_SUCCESS == GetTypeString(strDest));
		break;
	
		case 17: return (FWF_SUCCESS == GetFlags(strDest));
		break;

		case 18: return (FWF_SUCCESS == GetCRC(strDest));
		break;

		fOptions;
		default:	return false;
	}
}

bool CWiseFile::GetShortPath(std::wstring& strDest)
{
	if (0 == ::GetShortPathName(_strFullPath.c_str(), &strDest[0], (DWORD)strDest.capacity()))
	{
		return false;
	}
	return true;
}

int CWiseFile::ReadVersionInfoEx()
{
	HINSTANCE h = ::LoadLibraryEx(_strFullPath.c_str(), NULL, LOAD_LIBRARY_AS_DATAFILE);
	if (NULL == h)
	{
		return FWF_ERR_NOVERSION;
	}
	
	HRSRC hrsrc = ::FindResource(h, MAKEINTRESOURCE(1), RT_VERSION);
	if (NULL == hrsrc)
	{
		FreeLibrary(h);
		return FWF_ERR_NOVERSION;
	}

	UINT cb = ::SizeofResource(h, hrsrc);
	if (0 == cb)
	{
		FreeLibrary(h);
		return FWF_ERR_NOVERSION;
	}

	const UINT xcb = ::GetFileVersionInfoSize(_strFullPath.c_str(), 0);

	HGLOBAL hglobal = ::LoadResource(h, hrsrc);
	if (hglobal == NULL)
	{
		FreeLibrary(h);
		return FWF_ERR_NOVERSION;
	}

	LPBYTE pb = (BYTE*) ::LockResource(hglobal);
	if (NULL == pb)
	{
		FreeLibrary(h);
		return FWF_ERR_NOVERSION;
	}

	VS_FIXEDFILEINFO* pvi = new VS_FIXEDFILEINFO;
	LPBYTE pVerBuffer = pb;

	// Usually before you get here, you call GetFileVersionInfoSize to figure out cb
	// to get that size manually, you have to walk thru umpteen fields
	// then you call get file version info, which supposedly gives you VS_VERSION_INFO
	// but pb points to the VS_VERSION_INFO, so we'll VerQueryValue directly

	// next line causes access violation
	if (! ::VerQueryValue(pb, L"\\", (void**) &pvi, &cb))
	{
		return FWF_ERR_NOVERSION;
	}

	if (pvi->dwSignature != 0xFEEF04BD)
	{
		FreeLibrary(h);
		return FWF_ERR_NOVERSION;
	}

	m_qwFileVersion = MAKEQWORD(pvi->dwFileVersionLS, pvi->dwFileVersionMS);
	m_qwProductVersion = MAKEQWORD(pvi->dwProductVersionLS, pvi->dwProductVersionMS);
	m_dwType = pvi->dwFileType;
	m_dwOS = pvi->dwFileOS;
	m_dwFlags = pvi->dwFileFlags;

	LPVOID pVerData = NULL;
	UINT cbVerData = 0;
	if (!::VerQueryValue(pVerBuffer, L"\\VarFileInfo\\Translation", &pVerData, &cbVerData))
	{
		return FWF_ERR_NOVERSION;
	}

	m_wLanguage = LOWORD(*(LPDWORD)pVerBuffer);
	m_CodePage = HIWORD(*(LPDWORD)pVerBuffer);

	FreeLibrary(h);
	return FWF_SUCCESS;
}

bool CWiseFile::Init()
{
	zero(m_stLocalCreation);
	zero(m_stLocalLastAccess);
	zero(m_stLocalLastWrite);

	zero(m_stUTCCreation);
	zero(m_stUTCLastAccess);
	zero(m_stUTCLastWrite);
	
	m_dwAttribs = 0;
	m_qwSize = 0;
	m_qwFileVersion = 0;
	m_qwProductVersion = 0;
	m_wLanguage = 0;
	m_CodePage = 0;
	m_dwOS = 0;
	m_dwType = 0;
	m_dwFlags = 0;
	m_dwCRC = 0;

	m_fDebugStripped = false;
	m_fHasVersion = false;
	m_fszFlags = false;	
	m_fszAttribs = false;
	m_fszOS = false;
	m_fszType = false;

	return true;
}