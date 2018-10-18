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

#include "stdafx.h"
#include <mlang.h>

//#include "strlib.h"

#define __T(x)      L ## x

class TCodePage  
{
public:
	TCodePage()
	{
		m_wCodePage = 0;
		m_pszCodePage = NULL;
		m_pML = NULL;

		if (SUCCEEDED(CoInitialize(NULL)))
		{
			if (FAILED(CoCreateInstance(CLSID_CMultiLanguage, NULL, CLSCTX_INPROC_SERVER, IID_IMultiLanguage2, (void**)&m_pML)))
			{
				m_pML = NULL;
			}
		}
	}

	TCodePage(WORD wCodePage)
	{
		m_wCodePage = wCodePage;
		m_pszCodePage = NULL;
		CoUninitialize();
	}

	bool GetCodePageName(WORD wLanguage, UINT CodePage, LPWSTR pszBuf)
	{
		if (NULL == pszBuf)
			return false;
		
		if (m_pML)
		{
			ZeroMemory(&m_mcp, sizeof(MIMECPINFO));
			if (S_OK  == m_pML->GetCodePageInfo(CodePage, wLanguage, &m_mcp))
			{
				wsprintf( pszBuf, L"%s (%lu)", m_mcp.wszDescription, CodePage);
				return true;
			}
		}

		CPINFOEX cp;
		ZeroMemory(&cp, sizeof(CPINFOEX));
		if (GetCPInfoEx(CodePage, 0, &cp))
		{
			wsprintf( pszBuf, L"%s", cp.CodePageName);
			return true;
		}
		
		switch (CodePage)
		{
			case	37:		wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - U.S./Canada"));
			break;

			case	709:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("Arabic (ASMO 449+, BCON V4)"));
			break;

			case	710:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("Arabic (Transparent Arabic)"));
			break;

			case	437:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("OEM - United States"));
			break;

			case	500:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - International"));
			break;

			case	708:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("Arabic - ASMO"));
			break;

			case	720:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("Arabic - Transparent ASMO"));
			break;

			case	737:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("OEM - Greek 437G"));
			break;

			case	775:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("OEM - Baltic"));
			break;

			case	850:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("OEM - Multilingual Latin I"));
			break;

			case	852:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("OEM - Latin II"));
			break;

			case	855:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("OEM - Cyrillic"));
			break;

			case	857:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("OEM - Turkish"));
			break;

			case	860:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("OEM - Portuguese"));
			break;

			case	861:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("OEM - Icelandic"));
			break;

			case	862:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("OEM - Hebrew"));
			break;

			case	863:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("OEM - Canadian French"));
			break;

			case	864:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("OEM - Arabic"));
			break;

			case	865:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("OEM - Nordic"));
			break;

			case	866:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("OEM - Russian"));
			break;

			case	869:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("OEM - Modern Greek"));
			break;

			case	870:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Multilingual/ROECE (Latin-2)"));
			break;

			case	874:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ANSI/OEM - Thai"));
			break;

			case	875:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Modern Greek"));
			break;

			case	932:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ANSI/OEM - Japanese Shift-JIS"));
			break;

			case	936:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ANSI/OEM - Simplified Chinese GBK"));
			break;

			case	949:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ANSI/OEM - Korean"));
			break;

			case	950:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ANSI/OEM - Traditional Chinese Big5"));
			break;

			case	1200:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("Unicode (BMP of ISO 10646)"));
			break;

			case	1026:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Turkish (Latin-5)"));
			break;

			case	1250:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ANSI - Central Europe"));
			break;

			case	1251:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ANSI - Cyrillic"));
			break;

			case	1252:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ANSI - Latin I"));
			break;

			case	1253:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ANSI - Greek"));
			break;

			case	1254:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ANSI - Turkish"));
			break;

			case	1255:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ANSI - Hebrew"));
			break;

			case	1256:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ANSI - Arabic"));
			break;

			case	1257:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ANSI - Baltic"));
			break;

			case	1258:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ANSI/OEM - Viet Nam"));
			break;

			case	1361:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("Korean - Johab"));
			break;

			case	10000:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("MAC - Roman"));
			break;

			case	10001:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("MAC - Japanese"));
			break;

			case	10002:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("MAC - Traditional Chinese Big5"));
			break;

			case	10003:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("MAC - Korean"));
			break;

			case	10004:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("MAC - Arabic"));
			break;

			case	10005:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("MAC - Hebrew"));
			break;

			case	10006:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("MAC - Greek I"));
			break;

			case	10007:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("MAC - Cyrillic"));
			break;

			case	10008:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("MAC - Simplified Chinese GB 2312"));
			break;

			case	10010:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("MAC - Romania"));
			break;

			case	10017:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("MAC - Ukraine"));
			break;

			case	10029:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("MAC - Latin II"));
			break;

			case	10079:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("MAC - Icelandic"));
			break;

			case	10081:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("MAC - Turkish"));
			break;

			case	10082:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("MAC - Croatia"));
			break;

			case	20866:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("Russian - KOI8"));
			break;

			case	29001:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("Europa 3"));
			break;

			case	65000:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("UTF-7"));
			break;

			case	65001:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("UTF-8"));
			break;

			case	20000:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("CNS - Taiwan"));
			break;

			case	20001:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("TCA - Taiwan"));
			break;

			case	20002:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("Eten - Taiwan"));
			break;

			case	20003:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM5550 - Taiwan"));
			break;

			case	20004:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("TeleText - Taiwan"));
			break;

			case	20005:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("Wang - Taiwan"));
			break;

			case	20105:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IA5 IRV International Alphabet No. 5"));
			break;

			case	20106:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IA5 German"));
			break;

			case	20107:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IA5 Swedish"));
			break;

			case	20108:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IA5 Norwegian"));
			break;

			case	20127:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("US-ASCII"));
			break;

			case	20261:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("T.61"));
			break;

			case	20269:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ISO 6937 Non-Spacing Accent"));
			break;

			case	20273:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Germany"));
			break;

			case	20277:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Denmark/Norway"));
			break;

			case	20278:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Finland/Sweden"));
			break;

			case	20280:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Italy"));
			break;

			case	20284:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Latin America/Spain"));
			break;

			case	20285:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - United Kingdom"));
			break;

			case	20290:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Japanese Katakana Extended"));
			break;

			case	20297:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - France"));
			break;

			case	20420:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Arabic"));
			break;

			case	20423:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Greek"));
			break;

			case	20424:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Hebrew"));
			break;

			case	20833:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Korean Extended"));
			break;

			case	20838:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Thai"));
			break;

			case	20871:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Icelandic"));
			break;

			case	20880:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Cyrillic,(Russian)"));
			break;

			case	20905:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Turkish"));
			break;

			case	21025:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Cyrillic (Serbian, Bulgarian)"));
			break;

			case	21027:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("Ext Alpha Lowercase"));
			break;

			case	28591:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ISO 8859-1 Latin I"));
			break;

			case	28592:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ISO 8859-2 Central Europe"));
			break;

			case	28593:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ISO 8859-3 Turkish"));
			break;

			case	28594:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ISO 8859-4 Baltic"));
			break;

			case	28595:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ISO 8859-5 Cyrillic"));
			break;

			case	28596:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ISO 8859-6 Arabic"));
			break;

			case	28597:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ISO 8859-7 Greek"));
			break;

			case	28598:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ISO 8859-8 Hebrew"));
			break;

			case	28599:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ISO 8859-9 Latin 5"));
			break;

			case	50220:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ISO-2022 Japanese with no halfwidth Katakana"));
			break;

			case	50221:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ISO-2022 Japanese with halfwidth Katakana"));
			break;

			case	50222:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ISO-2022 Japanese JIS X 0201-1989"));
			break;

			case	50225:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ISO-2022 Korean"));
			break;

			case	50227:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ISO-2022 Simplified Chinese"));
			break;

			case	50229:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("ISO-2022 Traditional Chinese"));
			break;

			case	52936:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("HZ-GB2312 Simplified Chinese"));
			break;

			case	51932:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("EUC-Japanese"));
			break;

			case	51949:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("EUC-Korean"));
			break;

			case	51936:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("EUC-Simplified Chinese"));
			break;

			case	51950:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("EUC-Traditional Chinese"));
			break;

			case	50930:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Japanese (Katakana) Extended and Japanese"));
			break;

			case	50931:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - US/Canada and Japanese"));
			break;

			case	50933:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Korean Extended and Korean"));
			break;

			case	50935:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Simplified Chinese"));
			break;

			case	50937:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - US/Canada and Traditional Chinese"));
			break;

			case	50939:	wsprintf(pszBuf,__T("%lu %s"),CodePage,__T("IBM EBCDIC - Japanese (Latin) Extended and Japanese"));
			break;

			default:	wsprintf( pszBuf, __T("%lu Unknown"), CodePage);
			break;
		}

		return true;
	}

	size_t GetStringLength()
	{
		return wcslen(m_pszCodePage);
	}

	virtual ~TCodePage()
	{
		m_wCodePage = 0;
		delete [] m_pszCodePage;
	}

	operator LPCTSTR()
	{
		return NULL;
	}

	operator WORD() const
	{
		return m_wCodePage;
	}

	void Set(WORD wCodePage)
	{
		m_wCodePage = wCodePage;
	}

	WORD Get(void) const
	{
		return m_wCodePage;
	}

	const TCodePage& operator=(const TCodePage& cp)
	{
		m_wCodePage = (WORD)cp;
		return *this;
	}

	const TCodePage& operator=(const WORD w)
	{
		m_wCodePage = w;
		return *this;
	}

	bool operator==(const TCodePage& cp)
	{
		return m_wCodePage == (WORD)cp;
	}

	bool operator!=(const TCodePage& cp)
	{
		return m_wCodePage != (WORD)cp;
	}

	bool operator==(const WORD w)
	{
		return m_wCodePage == w;
	}

	bool operator!=(const WORD w)
	{
		return m_wCodePage != w;
	}

private:
	IMultiLanguage2* m_pML;

	MIMECPINFO m_mcp;
	WORD m_wCodePage;
	LPWSTR m_pszCodePage;
};
