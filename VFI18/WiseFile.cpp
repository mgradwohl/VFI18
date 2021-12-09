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

CWiseFile::CWiseFile()
{
	Init();
	SetState(FWFS_VALID);
}

CWiseFile::CWiseFile(const CWiseFile& rwf)
{
	Copy(rwf);
}

CWiseFile::CWiseFile(LPCWSTR pszFileSpec)
{
	if (FWF_SUCCESS != Attach(pszFileSpec))
	{
		Init();
	}

	OrState(FWFS_CRC_PENDING);
}

// CWiseFile::~CWiseFile
CWiseFile::~CWiseFile()
{
	SetState(FWFS_INVALID);
}

int CWiseFile::Attach(LPCWSTR pszFileSpec)
{
	if (wcslen(pszFileSpec) < 1 || (wcslen(pszFileSpec) > _MAX_PATH))
	{
		return FWF_ERR_BADPARAM;
	}

	if (0 == StrCmpN(pszFileSpec, L"\\\\?\\", 4))
	{
		return FWF_ERR_SPECIALPATH;
	}

	WIN32_FIND_DATA fd;
	zero(fd);

	HANDLE hff = FindFirstFile(pszFileSpec, &fd);
	if (INVALID_HANDLE_VALUE == hff)
	{
		//FindClose(hff);
		return FWF_ERR_BADPARAM;
	}

	m_dwAttribs = fd.dwFileAttributes;
	if (m_dwAttribs & FILE_ATTRIBUTE_DIRECTORY)
	{
		FindClose(hff);
		return FWF_ERR_FOLDER;
	}

	m_qwSize = GetFileSize64(pszFileSpec);

	FILETIME ft;
	::FileTimeToSystemTime(&(fd.ftCreationTime), &m_stUTCCreation);
	::FileTimeToLocalFileTime(&(fd.ftCreationTime), &ft);
	::FileTimeToSystemTime(&ft, &m_stLocalCreation);

	::FileTimeToSystemTime(&(fd.ftLastAccessTime), &m_stUTCLastAccess);
	::FileTimeToLocalFileTime(&(fd.ftLastAccessTime), &ft);
	::FileTimeToSystemTime(&ft, &m_stLocalLastAccess);

	::FileTimeToSystemTime(&(fd.ftLastWriteTime), &m_stUTCLastWrite);
	::FileTimeToLocalFileTime(&(fd.ftLastWriteTime), &ft);
	::FileTimeToSystemTime(&ft, &m_stLocalLastWrite);

	WCHAR szDrive[_MAX_DRIVE];
	WCHAR szDir[_MAX_DIR];
	LPWSTR pszDot;
	_wsplitpath_s(pszFileSpec, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, m_szName, _MAX_PATH, m_szExt, _MAX_PATH);

	pszDot = CharNext(m_szExt);
	wcscpy_s(m_szExt, pszDot);

	wcscpy_s(m_szFullPath, pszFileSpec);
	wcscpy_s(m_szPath, szDrive);
	wcscat_s(m_szPath, szDir);
	wcscpy_s(m_szShortName, fd.cAlternateFileName);

	SetState(FWFS_ATTACHED);

	SetDateCreation();
	SetTimeCreation();
	SetDateLastWrite();
	SetTimeLastWrite();
	SetDateLastAccess();
	SetTimeLastAccess();

	if (0 != FindNextFile(hff, &fd))
	{
		FindClose(hff);
		return FWF_ERR_WILDCARD;
	}
	FindClose(hff);

	SetState(FWFS_ATTACHED);

	SetDateCreation();
	SetTimeCreation();
	SetDateLastWrite();
	SetTimeLastWrite();
	SetDateLastAccess();
	SetTimeLastAccess();

	return FWF_SUCCESS;
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
	wcscpy_s(m_szAttribs, rwf.m_szAttribs);
	wcscpy_s(m_szExt, rwf.m_szExt);
	wcscpy_s(m_szFlags, rwf.m_szFlags);
	wcscpy_s(m_szFullPath, rwf.m_szFullPath);
	wcscpy_s(m_szShortName, rwf.m_szShortName);
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
		return ((0 == _wcsicmp(m_szFullPath, rwf.m_szFullPath))
			&& (m_qwFileVersion == rwf.m_qwFileVersion));
	}

	if (CheckState(FWFS_ATTACHED))
	{
		return (0 == _wcsicmp(m_szFullPath, rwf.m_szFullPath));
	}

	return false;
}

int CWiseFile::Detach()
{
	// does nothing
	return FWF_SUCCESS;
}

int CWiseFile::SetSize(bool bHex)
{
	if (!CheckState(FWFS_ATTACHED))
	{
		return FWF_ERR_INVALID;
	}

	if (bHex)
	{
		return FWF_SUCCESS;
	}
	else
	{
		// Not using StrFormatByteSize because it wordifies everything
		if (int2str(m_szSize, m_qwSize))
		{
			OrState(FWFS_SIZE);
			return FWF_SUCCESS;
		}
		else
		{
			return FWF_ERR_OTHER;
		}
	}
}

int CWiseFile::SetCRC(bool bHex)
{
	if (!CheckState(FWFS_ATTACHED))
	{
		return FWF_ERR_INVALID;
	}

	if (CheckState(FWFS_CRC_COMPLETE))
	{
		if (bHex)
		{
			wsprintf(m_szCRC, L"%08x", m_dwCRC);
		}
		else
		{
			int2str(m_szCRC, m_dwCRC);
		}
		return FWF_SUCCESS;
	}

	if (CheckState(FWFS_CRC_ERROR))
	{
		LoadWstring(STR_CRC_ERROR, m_szCRC, _countof(m_szCRC));
		return FWF_SUCCESS;
	}

	if (CheckState(FWFS_CRC_PENDING))
	{
		LoadWstring(STR_CRC_PENDING, m_szCRC, _countof(m_szCRC));
		return FWF_SUCCESS;
	}

	if (CheckState(FWFS_CRC_WORKING))
	{
		LoadWstring(STR_CRC_WORKING, m_szCRC, _countof(m_szCRC));
		return FWF_SUCCESS;
	}

	return FWF_ERR_OTHER;
}

int CWiseFile::SetFileVersion()
{
	if (!CheckState(FWFS_VERSION))
	{
		return FWF_ERR_INVALID;
	}

	if (!m_fHasVersion)
	{
		return FWF_SUCCESS;
	}

	WCHAR szDec[2];
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, szDec, 2);

	wsprintf(m_szFileVersion, L"%d%s%02d%s%04d%s%04d",
		(int)HIWORD(HIDWORD(m_qwFileVersion)),
		szDec,
		(int)LOWORD(HIDWORD(m_qwFileVersion)),
		szDec,
		(int)HIWORD(LODWORD(m_qwFileVersion)),
		szDec,
		(int)LOWORD(LODWORD(m_qwFileVersion)));

	return FWF_SUCCESS;
}

int CWiseFile::GetFileVersion(LPDWORD pdwMS, LPDWORD pdwLS)
{
	if (!CheckState(FWFS_VERSION))
		return FWF_ERR_INVALID;

	if (pdwMS)
		*pdwMS = HIDWORD(m_qwFileVersion);

	if (pdwLS)
		*pdwLS = LODWORD(m_qwFileVersion);

	return FWF_SUCCESS;
}

int CWiseFile::GetFileVersion(LPWORD pwHighMS, LPWORD pwLowMS, LPWORD pwHighLS, LPWORD pwLowLS)
{
	if (!CheckState(FWFS_VERSION))
		return FWF_ERR_INVALID;

	if (pwHighMS)
		*pwHighMS = HIWORD(HIDWORD(m_qwFileVersion));

	if (pwLowMS)
		*pwLowMS = LOWORD(HIDWORD(m_qwFileVersion));

	if (pwHighLS)
		*pwHighLS = HIWORD(LODWORD(m_qwFileVersion));

	if (pwLowLS)
		*pwLowLS = LOWORD(LODWORD(m_qwFileVersion));

	return FWF_SUCCESS;
}

int CWiseFile::SetProductVersion()
{
	if (!CheckState(FWFS_VERSION))
	{
		return FWF_ERR_INVALID;
	}

	if (!m_fHasVersion)
	{
		return FWF_SUCCESS;
	}

	WCHAR szDec[2];
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, szDec, 2);

	wsprintf(m_szProductVersion, L"%2d%s%02d%s%04d%s%04d",
		(int)HIWORD(HIDWORD(m_qwProductVersion)),
		szDec,
		(int)LOWORD(HIDWORD(m_qwProductVersion)),
		szDec,
		(int)HIWORD(LODWORD(m_qwProductVersion)),
		szDec,
		(int)LOWORD(LODWORD(m_qwProductVersion)));
	return FWF_SUCCESS;
}

int CWiseFile::GetProductVersion(LPDWORD pdwMS, LPDWORD pdwLS)
{
	if (!CheckState(FWFS_VERSION))
		return FWF_ERR_INVALID;

	if (pdwMS)
		*pdwMS = HIDWORD(m_qwProductVersion);

	if (pdwLS)
		*pdwLS = LODWORD(m_qwProductVersion);

	return FWF_SUCCESS;
}

int CWiseFile::GetProductVersion(LPWORD pwHighMS, LPWORD pwLowMS, LPWORD pwHighLS, LPWORD pwLowLS)
{
	if (!CheckState(FWFS_VERSION))
		return FWF_ERR_INVALID;

	if (pwHighMS)
		*pwHighMS = HIWORD(HIDWORD(m_qwProductVersion));

	if (pwLowMS)
		*pwLowMS = LOWORD(HIDWORD(m_qwProductVersion));

	if (pwHighLS)
		*pwHighLS = HIWORD(LODWORD(m_qwProductVersion));

	if (pwLowLS)
		*pwLowLS = LOWORD(LODWORD(m_qwProductVersion));

	return FWF_SUCCESS;
}

void CWiseFile::SetType()
{
	if (!CheckState(FWFS_VERSION))
		return;

	if (!m_fHasVersion)
		return;

	switch (m_dwType)
	{
	case VFT_UNKNOWN: wsprintf(m_szType, L"VFT_UNKNOWN: 0x%08x", m_dwType);
		break;
	case VFT_APP: wcscpy_s(m_szType, L"VFT_APP");
		break;
	case VFT_DLL: wcscpy_s(m_szType, L"VFT_DLL");
		break;
	case VFT_DRV: wcscpy_s(m_szType, L"VFT_DRV");
		break;
	case VFT_FONT: wcscpy_s(m_szType, L"VFT_FONT");
		break;
	case VFT_VXD: wcscpy_s(m_szType, L"VFT_VXD");
		break;
	case VFT_STATIC_LIB: wcscpy_s(m_szType, L"VFT_STATIC_LIB");
		break;
	default:
	{
		wsprintf(m_szType, L"Reserved: 0x%08x", m_dwType);
	}
	}
	m_fszType = true;
}

void CWiseFile::SetOS()
{
	if (!CheckState(FWFS_VERSION))
		return;

	if (!m_fHasVersion)
		return;

	switch (m_dwOS)
	{
		case VOS_UNKNOWN: wsprintf(m_szOS, L"VOS_UNKNOWN: 0x%08x", m_dwOS);
			break;
		case VOS_DOS: wcscpy_s(m_szOS, 64, L"VOS_DOS");
			break;
		case VOS_OS216: wcscpy_s(m_szOS, 64, L"VOS_OS216");
			break;
		case VOS_OS232: wcscpy_s(m_szOS, 64, L"VOS_OS232");
			break;
		case VOS_NT: wcscpy_s(m_szOS, 64, L"VOS_NT");
			break;
		case VOS__WINDOWS16: wcscpy_s(m_szOS, 64, L"VOS__WINDOWS16");
			break;
		case VOS__PM16: wcscpy_s(m_szOS, 64, L"VOS__PM16");
			break;
		case VOS__PM32: wcscpy_s(m_szOS, 64, L"VOS__PM32");
			break;
		case VOS__WINDOWS32: wcscpy_s(m_szOS, 64, L"VOS__WINDOWS32");
			break;
		case VOS_OS216_PM16: wcscpy_s(m_szOS, 64, L"VOS_OS216_PM16");
			break;
		case VOS_OS232_PM32: wcscpy_s(m_szOS, 64, L"VOS_OS232_PM32");
			break;
		case VOS_DOS_WINDOWS16: wcscpy_s(m_szOS, 64,  L"VOS_DOS_WINDOWS16");
			break;
		case VOS_DOS_WINDOWS32: wcscpy_s(m_szOS, 64, L"VOS_DOS_WINDOWS32");
			break;
		case VOS_NT_WINDOWS32: wcscpy_s(m_szOS, L"VOS_NT_WINDOWS32");
			break;
		default:		wsprintf(m_szOS, L"Reserved: 0x%08x", m_dwOS);
	}
	m_fszOS = true;
}

//int CWiseFile::GetOSString(LPWSTR pszText)
//{
//	if (!CheckState(FWFS_VERSION))
//	{
//		lstrinit(pszText);
//		return FWF_ERR_INVALID;
//	}
//
//	if (!m_fHasVersion)
//	{
//		*pszText = 0;
//		return FWF_SUCCESS;
//	}
//
//	if (!m_fszOS)
//	{
//		GetOSString();
//	}
//
//	if (m_fszOS)
//	{
//		wcscpy_s(pszText, 64, m_szOS);
//	}
//
//	return FWF_SUCCESS;
//}

int CWiseFile::SetCodePage(bool bNumeric)
{
	if (!CheckState(FWFS_VERSION))
	{
		return FWF_ERR_INVALID;
	}

	if (!m_fHasVersion)
	{
		return FWF_SUCCESS;
	}

	if (bNumeric)
	{
		wsprintf(m_szCodePage, L"%04x", (WORD)m_CodePage);
	}
	else
	{
		m_CodePage.GetCodePageName(LANGIDFROMLCID(GetThreadLocale()), m_CodePage, m_szCodePage);
	}
	return FWF_SUCCESS;
}

int CWiseFile::ReadVersionInfo()
{

	char szPath[_MAX_PATH];
	WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, m_szFullPath, _MAX_PATH, szPath, _MAX_PATH, nullptr, nullptr);
	PLOADED_IMAGE pli;
	pli = ImageLoad(szPath, NULL);
	if (NULL != pli)
	{
		if (pli->Characteristics & IMAGE_FILE_DEBUG_STRIPPED)
		{
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

	// this takes a long time to call, max size I've seen is 5476
	DWORD	dwVerHandle = 0;
	dwVerSize = ::GetFileVersionInfoSize(m_szFullPath, &dwVerHandle);

	if (dwVerSize < 1)
		return FWF_ERR_NOVERSION;

	lpVerBuffer = (LPVOID) new BYTE[dwVerSize];

	if (NULL == lpVerBuffer)
	{
		delete[] lpVerBuffer;
		return FWF_ERR_LOWMEMORY;
	}

	ZeroMemory(lpVerBuffer, dwVerSize);
#pragma warning(suppress: 6388)
// the warning is that dwVerHandle can not be zero. We never call this with dwVerHandle == 0
// because of the dwVerSize < 1 check above, that returns an error if there is no version information
	if (!::GetFileVersionInfo(m_szFullPath, dwVerHandle, dwVerSize, lpVerBuffer))
	{
		delete[] lpVerBuffer;
		return FWF_ERR_NOVERSION;
	}

	if (!::VerQueryValue(lpVerBuffer, L"\\", &lpVerData, &cbVerData))
	{
		delete[] lpVerBuffer;
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
		delete[] lpVerBuffer;
		return FWF_ERR_NOVERSION;
	}

	m_wLanguage = LOWORD(*(LPDWORD)lpVerData);
	m_CodePage = HIWORD(*(LPDWORD)lpVerData);
	delete[] lpVerBuffer;

	OrState(FWFS_VERSION);

	return FWF_SUCCESS;
}

int CWiseFile::SetDateCreation(bool fLocal)
{
	if (!CheckState(FWFS_ATTACHED))
	{
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

	WCHAR szDate[64];
	if (0 == ::GetDateFormat(::GetThreadLocale(), DATE_SHORTDATE, pst, NULL, szDate, 64))
	{
		return FWF_ERR_OTHER;
	}
	else
	{
		wcscpy_s(m_szDateCreated, szDate);
		return FWF_SUCCESS;
	}
}

int CWiseFile::SetDateLastAccess(bool fLocal)
{
	if (!CheckState(FWFS_ATTACHED))
	{
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

	WCHAR szDate[64];
	if (0 == ::GetDateFormat(::GetThreadLocale(), DATE_SHORTDATE, pst, NULL, szDate, 64))
	{
		return FWF_ERR_OTHER;
	}
	else
	{
		wcscpy_s(m_szDateLastAccess, szDate);
		return FWF_SUCCESS;
	}
}

int CWiseFile::SetDateLastWrite(bool fLocal)
{
	if (!CheckState(FWFS_ATTACHED))
	{
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

	WCHAR szDate[64];
	if (0 == ::GetDateFormat(::GetThreadLocale(), DATE_SHORTDATE, pst, NULL, szDate, 64))
	{
		return FWF_ERR_OTHER;
	}
	else
	{
		wcscpy_s(m_szDateLastWrite, szDate);
		return FWF_SUCCESS;
	}
}

int CWiseFile::SetTimeCreation(bool fLocal)
{
	if (!CheckState(FWFS_ATTACHED))
	{
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

	WCHAR szTime[64];
	if (0 == ::GetTimeFormat(::GetThreadLocale(), TIME_NOSECONDS, pst, NULL, szTime, 64))
	{
		return FWF_ERR_OTHER;
	}
	else
	{
		wcscpy_s(m_szTimeCreated, szTime);
		return FWF_SUCCESS;
	}
}

int CWiseFile::SetTimeLastAccess(bool fLocal)
{
	if (!CheckState(FWFS_ATTACHED))
	{
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

	WCHAR szTime[64];
	if (0 == ::GetTimeFormat(::GetThreadLocale(), TIME_NOSECONDS, pst, NULL, szTime, 64))
	{
		return FWF_ERR_OTHER;
	}
	else
	{
		wcscpy_s(m_szTimeLastAccess, szTime);
		return FWF_SUCCESS;
	}
}

int CWiseFile::SetTimeLastWrite(bool fLocal)
{
	if (!CheckState(FWFS_ATTACHED))
	{
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

	WCHAR szTime[64];
	if (0 == ::GetTimeFormat(::GetThreadLocale(), TIME_NOSECONDS, pst, NULL, szTime, 64))
	{
		return FWF_ERR_OTHER;

	}
	else
	{
		wcscpy_s(m_szTimeLastWrite, 64, szTime);
		return FWF_SUCCESS;
	}
}

int CWiseFile::TouchFileTime(FILETIME* lpTime)
{
	if (!CheckState(FWFS_ATTACHED))
		return FWF_ERR_INVALID;

	if (NULL == lpTime)
		return FWF_ERR_BADPARAM;

	DWORD dwOldAttribs = ::GetFileAttributes(m_szFullPath);
	if (false == ::SetFileAttributes(m_szFullPath, FILE_ATTRIBUTE_NORMAL))
	{
		::SetFileAttributes(m_szFullPath, dwOldAttribs);
		return FWF_ERR_OTHER;
	}

	HANDLE hFile = ::CreateFile(m_szFullPath,
		GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		::SetFileAttributes(m_szFullPath, dwOldAttribs);
		::CloseHandle(hFile);
		return FWF_ERR_OTHER;
	}

	if (false == ::SetFileTime(hFile, lpTime, lpTime, lpTime))
	{
		::SetFileAttributes(m_szFullPath, dwOldAttribs);
		::CloseHandle(hFile);
		return FWF_ERR_OTHER;
	}

	::SetFileAttributes(m_szFullPath, dwOldAttribs);
	::CloseHandle(hFile);

	FILETIME ft;
	::FileTimeToSystemTime(lpTime, &m_stUTCCreation);
	::FileTimeToLocalFileTime(lpTime, &ft);
	::FileTimeToSystemTime(&ft, &m_stLocalCreation);

	::FileTimeToSystemTime(lpTime, &m_stUTCLastAccess);
	::FileTimeToLocalFileTime(lpTime, &ft);
	::FileTimeToSystemTime(&ft, &m_stLocalLastAccess);

	::FileTimeToSystemTime(lpTime, &m_stUTCLastWrite);
	::FileTimeToLocalFileTime(lpTime, &ft);
	::FileTimeToSystemTime(&ft, &m_stLocalLastWrite);

	return true;
}

int CWiseFile::SetAttribs()
{
	if (!CheckState(FWFS_ATTACHED))
	{
		wcscpy_s(m_szAttribs, L"        ");
		return FWF_ERR_INVALID;
	}

	if (m_fszAttribs)
	{
		// the string is already built, return it
		return FWF_SUCCESS;
	}
	m_fszAttribs = true;

	if (m_dwAttribs & FILE_ATTRIBUTE_NORMAL)
	{
		lstrcpy(m_szAttribs, L"        ");
		return FWF_SUCCESS;
	}

	wsprintf(m_szAttribs, L"%c%c%c%c%c%c%c%c",
		((m_dwAttribs & FILE_ATTRIBUTE_ARCHIVE) ? 'A' : ' '),
		((m_dwAttribs & FILE_ATTRIBUTE_HIDDEN) ? 'H' : ' '),
		((m_dwAttribs & FILE_ATTRIBUTE_READONLY) ? 'R' : ' '),
		((m_dwAttribs & FILE_ATTRIBUTE_SYSTEM) ? 'S' : ' '),
		((m_dwAttribs & FILE_ATTRIBUTE_ENCRYPTED) ? 'E' : ' '),
		((m_dwAttribs & FILE_ATTRIBUTE_COMPRESSED) ? 'C' : ' '),
		((m_dwAttribs & FILE_ATTRIBUTE_TEMPORARY) ? 'T' : ' '),
		((m_dwAttribs & FILE_ATTRIBUTE_OFFLINE) ? 'O' : ' '));

	wcscpy_s(m_szAttribs, 10, m_szAttribs);
	return FWF_SUCCESS;
}

int CWiseFile::SetFlags()
{
	if (!CheckState(FWFS_VERSION))
	{
		return FWF_ERR_INVALID;
	}

	if (!m_fHasVersion)
	{
		return FWF_SUCCESS;
	}

	if (m_fszFlags)
	{
		return FWF_SUCCESS;
	}
	m_fszFlags = true;

	// list seperator
	WCHAR szSep[3];
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SLIST, szSep, 2);
	wcscat_s(szSep, L" ");

	wchar_t szStr[256];
	if (m_dwFlags & VS_FF_DEBUG)
	{
		if (m_fDebugStripped)
		{

			LoadWstring(STR_FLAG_DEBUG_STRIPPED, szStr, _countof(szStr));
		}
		else
		{
			LoadWstring(STR_FLAG_DEBUG, szStr, _countof(szStr));
		}
		wcscat_s(m_szFlags, szStr);
		wcscat_s(m_szFlags, szSep);
	}
	if (m_dwFlags & VS_FF_PRERELEASE)
	{
		LoadWstring(STR_FLAG_PRERELEASE, szStr, _countof(szStr));
		wcscat_s(m_szFlags, szStr);
		wcscat_s(m_szFlags, szSep);
	}
	if (m_dwFlags & VS_FF_PATCHED)
	{
		LoadWstring(STR_FLAG_PATCHED, szStr, _countof(szStr));
		wcscat_s(m_szFlags, szStr);
		wcscat_s(m_szFlags, szSep);
	}
	if (m_dwFlags & VS_FF_PRIVATEBUILD)
	{
		LoadWstring(STR_FLAG_PRIVATEBUILD, szStr, _countof(szStr));
		wcscat_s(m_szFlags, szStr);
		wcscat_s(m_szFlags, szSep);
	}
	if (m_dwFlags & VS_FF_INFOINFERRED)
	{
		LoadWstring(STR_FLAG_INFOINFERRED, szStr, _countof(szStr));
		wcscat_s(m_szFlags, szStr);
		wcscat_s(m_szFlags, szSep);
	}
	if (m_dwFlags & VS_FF_SPECIALBUILD)
	{
		LoadWstring(STR_FLAG_SPECIALBUILD, szStr, _countof(szStr));
		wcscat_s(m_szFlags, szStr);
		wcscat_s(m_szFlags, szSep);
	}

	LPWSTR pEnd = wcsrchr(m_szFlags, szSep[0]);
	if (pEnd)
	{
		lstrinit(pEnd);
	}

	// if we've looked at it, and we still don't have a string for it
	// put a default one in (unless the flags are 00000000)
	if ((wcslen(m_szFlags) < 3) && m_dwFlags != 0)
	{
		wsprintf(m_szFlags, L"%08x", m_dwFlags);;
	}

	return FWF_SUCCESS;
}

bool CWiseFile::SetLanguage(UINT Language)
{
	WCHAR szTemp[255];

	if (0 == Language)
	{
		VerLanguageName(Language, szTemp, 255);
		wsprintf(m_szLanguage, L"%lu %s", Language, szTemp);
		OrState(FWFS_LANGUAGE);
		return true;
	}

	if (0 == GetLocaleInfo(Language, LOCALE_SLANGUAGE, szTemp, 255))
	{
		VerLanguageName(Language, szTemp, 255);
	}
	wsprintf(m_szLanguage, L"%lu %s", Language, szTemp);
	OrState(FWFS_LANGUAGE);
	return true;
}

LPWSTR CWiseFile::GetFieldString(int iField, bool fOptions)
{
	fOptions;
	switch (iField)
	{
	case  0: return GetPath();
	case  1: return GetName();
	case  2: return GetExt();
	case  3: return GetSize();
	case  4: return GetDateCreated();
	case  5: return GetTimeCreated();
	case  6: return GetDateLastWrite();
	case  7: return GetTimeLastWrite();
	case  8: return GetDateLastAccess();
	case  9: return GetTimeLastAccess();
	case 10: return GetAttribs();
	case 11: return GetFileVersion();
	case 12: return GetProductVersion();
	case 13: return GetLanguage();
	case 14: return GetCodePage();
	case 15: return GetOS();
	case 16: return GetType();
	case 17: return GetFlags();
	case 18: return GetCRC();
	}
	return L"Unknown";
}

int CWiseFile::ReadVersionInfoEx()
{
	HINSTANCE h = ::LoadLibraryEx(m_szFullPath, NULL, LOAD_LIBRARY_AS_DATAFILE);
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

	const UINT xcb = ::GetFileVersionInfoSize(m_szFullPath, 0);

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
	if (!::VerQueryValue(pb, L"\\", (void**)&pvi, &cb))
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
	lstrinit(m_szFullPath);
	lstrinit(m_szPath);
	lstrinit(m_szName);
	lstrinit(m_szExt);
	lstrinit(m_szShortName);
	lstrinit(m_szAttribs);
	lstrinit(m_szFlags);
	lstrinit(m_szOS);
	lstrinit(m_szType);

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