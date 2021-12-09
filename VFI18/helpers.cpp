#include "stdafx.h"
#include "helpers.h"

bool LoadWstring(std::wstring& strDest, UINT id)
{
	LPWSTR pszBuffer = nullptr;
	const size_t length = ::LoadString(GetModuleHandle(nullptr), id, (LPWSTR)&pszBuffer, 0);
	if (length == 0)
	{
		return false;
	}

	strDest.assign(pszBuffer, length);
	return true;
}

bool LoadWstring(UINT id, LPWSTR pszDest, int cchMax)
{
	const size_t length = ::LoadString(GetModuleHandle(nullptr), id, pszDest, cchMax);
	if (length == 0)
	{
		return false;
	}

	return true;
}

bool LoadWstring(LPWSTR pszDest, UINT id)
{
	LPWSTR pszBuffer = nullptr;
	const size_t length = ::LoadString(GetModuleHandle(nullptr), id, pszDest, 0);
	if (length == 0)
	{
		return false;
	}

	return true;
}

// Get number as a string
bool int2wstr(std::wstring& strDest, QWORD i)
{
	// get the separator
	std::wstring strDec;
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, &strDec[0], 2);

	std::wstring strBuffer = strDest;
	strBuffer.resize(68);
	_ui64tow_s(i, &strBuffer[0], 67, 10);

	if (0 == GetNumberFormat(LOCALE_USER_DEFAULT, 0, strBuffer.c_str(), nullptr, &strDest[0], 65))
		return false;

	const size_t decimal = strDest.find_last_of(strDec[0]);
	strDest.resize(decimal);

	return true;
}

bool int2str(LPWSTR pszDest, QWORD i)
{
	if (NULL == pszDest)
	{
		return false;
	}

	WCHAR szIn[67];
	WCHAR szOut[67];
	WCHAR szDec[2];

	_ui64tow_s(i, szIn, 67, 10);

	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, (LPWSTR)szDec, 2);
	if (0 == GetNumberFormat(LOCALE_USER_DEFAULT, 0, (LPWSTR)szIn, NULL, (LPWSTR)szOut, 65))
		return false;

	LPWSTR pszDec = wcsrchr((LPWSTR)szOut, szDec[0]);
	*pszDec = '\0';
	return (lstrcpy(pszDest, (LPWSTR)szOut) != nullptr);
}

bool pipe2null(std::wstring& strSource)
{
	if (strSource.length() < 1)
	{
		return false;
	}

	replace(strSource.begin(), strSource.end(), 'L', '\0');
	return true;
}

bool MyGetUserName(std::wstring& strUserName)
{
	DWORD dwLen = MAX_USERNAME;

	if (!GetUserName(&strUserName[0], &dwLen))
	{
		return false;
	}

	// todo use shrink_to_fit
	strUserName.resize(wcslen(strUserName.c_str()));
	return true;
}

__int64 GetFileSize64(LPCWSTR pszFileSpec)
{
	if (pszFileSpec == nullptr)
		return 0;

	HANDLE hFile = CreateFile(pszFileSpec,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		CloseHandle(hFile);
		return 0;
	}

	LARGE_INTEGER qwSize;
	if (!GetFileSizeEx(hFile, &qwSize))
	{
		CloseHandle(hFile);
		return 0;
	}
	CloseHandle(hFile);
	return qwSize.QuadPart;
}

bool GetModuleFolder(HINSTANCE hInst, LPWSTR pszFolder)
{
	if (NULL == pszFolder)
	{
		return false;
	}

	::GetModuleFileName(hInst, pszFolder, _MAX_PATH);

	PathGetFolder(pszFolder);
	return true;
}
bool DoesFileExist(LPCWSTR pszFileName)
{
	if (NULL == pszFileName)
	{
		return false;
	}

	DWORD const dw = ::GetFileAttributes(pszFileName);
	if (dw == (DWORD)-1 || dw & FILE_ATTRIBUTE_DIRECTORY || dw & FILE_ATTRIBUTE_OFFLINE)
	{
		return false;
	}

	return true;
}

bool DoesFolderExist(LPCWSTR pszFolder)
{
	if (NULL == pszFolder)
	{
		return false;
	}

	DWORD const dw = ::GetFileAttributes(pszFolder);
	if (dw == (DWORD)-1 || dw & FILE_ATTRIBUTE_OFFLINE)
	{
		return false;
	}

	if (dw & FILE_ATTRIBUTE_DIRECTORY)
	{
		return true;
	}

	return false;
}

bool PathGetFolder(LPWSTR pszFileSpec)
{

	if (NULL == pszFileSpec)
	{
		return false;
	}

	PathRemoveFileSpec(pszFileSpec);
	return true;
}

bool PathGetFileName(LPWSTR pszFileSpec)
{

	if (NULL == pszFileSpec)
	{
		return false;
	}

	pszFileSpec = PathFindFileName(pszFileSpec);

	return true;
}

bool OpenBox(const HWND hWnd, LPCWSTR pszTitle, LPCWSTR pszFilter, LPWSTR pszFile, int cchFile, LPWSTR pszFolder, const DWORD dwFlags)
{
	OPENFILENAME of;
	::ZeroMemory(&of, sizeof(OPENFILENAME));

	LPWSTR wszPath;
	if (NULL == pszFolder)
	{
		SHGetKnownFolderPath(FOLDERID_Desktop, KF_FLAG_DEFAULT, NULL, &wszPath);
		std::wstring strPath(wszPath);
		of.lpstrInitialDir = strPath.c_str();
		CoTaskMemFree(wszPath);
	}
	else
	{
		of.lpstrInitialDir = pszFolder;
	}

	*pszFile = L'\0';
	of.lStructSize = sizeof(OPENFILENAME);
	of.hwndOwner = hWnd;
	of.hInstance = GetModuleHandle(NULL);
	of.lpstrFilter = pszFilter;
	of.lpstrFile = pszFile;
	of.nMaxFile = cchFile;
	of.lpstrTitle = pszTitle;
	of.Flags = dwFlags | OFN_DONTADDTORECENT | OFN_LONGNAMES | OFN_ENABLESIZING | OFN_EXPLORER | OFN_NONETWORKBUTTON;

	if (!GetOpenFileNameW(&of))
	{
		DWORD dwError = CommDlgExtendedError();
		return false;
	}

	return true;
}
