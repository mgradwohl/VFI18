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

class CFileVersion  
{
public:
	CFileVersion();
	virtual ~CFileVersion();

	int GetMajor()			{ return m_V1;}
	int GetMinor()			{ return m_V2;}
	int GetBuild()			{ return m_V3;}

	int GetV1()				{return m_V1;}
	int GetV2()				{return m_V2;}
	int GetV3()				{return m_V3;}
	int GetV4()				{return m_V4;}

	bool GetVersion3(LPWSTR pszVersion);
	bool GetVersion4(LPWSTR pszVersion);
	bool Open(LPCWSTR pszFile);

private:
	bool ReadVersionInfo();
	int m_V1;
	int m_V2;
	int m_V3;
	int m_V4;
	bool m_fRead;
	LPWSTR m_pszFile;
};
