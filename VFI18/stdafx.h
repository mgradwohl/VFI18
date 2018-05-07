// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN
#define NOCOMM
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

// Windows head files
#include <windows.h>
#include <CommCtrl.h>
#include <commdlg.h>
#include <shlwapi.h>
#include <ShlObj.h>

// std header files
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

// project header files
#include "resource.h"
#include "helpers.h"
#include "mylist.h"
#include "trace.h"