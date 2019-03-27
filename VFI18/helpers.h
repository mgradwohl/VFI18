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

#include <lmcons.h>		// for UNLEN only

struct CLASSATTRIBS
{
	WORD idIcon;
	WORD idSmallIcon;
	WORD idMenu;
	WORD idAccelerators;
	WORD idClass;
	WORD idTitle;
};

static const size_t MAX_DRIVE = _MAX_DRIVE;
static const size_t MAX_DIR = _MAX_DIR;
static const size_t MAX_FNAME = _MAX_FNAME;
static const size_t MAX_EXT = _MAX_EXT;
static const size_t MAX_USERNAME = UNLEN + 1;
static const size_t maxExtendedPathLength = 0x7FFF - 24;

// QWORD
#define QWORD			DWORDLONG
#define LPQWORD			QWORD*
#define MAKEDWORD(a, b)	((DWORD)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define MAKEQWORD(a, b)	((QWORD)(((DWORD)(a)) | ((QWORD)((DWORD)(b))) << 32))

#define LOWORD(_dw)     ((WORD)(((DWORD_PTR)(_dw)) & 0xffff))
#define HIWORD(_dw)     ((WORD)((((DWORD_PTR)(_dw)) >> 16) & 0xffff))
#define LODWORD(_qw)    ((DWORD)(_qw))
#define HIDWORD(_qw)    ((DWORD)(((_qw) >> 32) & 0xffffffff))

// pointer macros
#define zero(x)			(::SecureZeroMemory((LPVOID)&x, sizeof(x)))

bool LoadWstring(std::wstring& strDest, UINT id);
bool LoadSZstring(LPWSTR szOut, UINT id, int cchMax);

// Get number as a string
bool int2wstr(std::wstring& strDest, QWORD i);
bool int2str(LPWSTR pszDest, QWORD i);

bool pipe2null(std::wstring& strSource);
bool MyGetUserName(std::wstring& strUserName);

__forceinline void lstrinit(LPWSTR psz)
{
	*psz = '\0';
}

__int64 GetFileSize64(LPCWSTR pszFileSpec);
bool GetModuleFolder(HINSTANCE hInst, LPWSTR pszFolder);
bool DoesFileExist(LPCWSTR pszFileName);
bool DoesFolderExist(LPCWSTR pszFolder);
bool PathGetFolder(LPWSTR pszFileSpec);
bool PathGetFileName(LPWSTR pszFileSpec);

bool OpenBox(const HWND hWnd, LPCWSTR pszTitle, LPCWSTR pszFilter, LPWSTR pszFile, LPWSTR pszFolder, const DWORD dwFlags);