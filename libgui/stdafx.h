#pragma once
#include <windows.h>
#include <commctrl.h>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <tchar.h>
#include <shlobj.h>

#ifdef UNICODE
#define String std::wstring
#define StringStream std::wstringstream
#else
#define String std::string
#define StringStream std::ostringstream
#endif

typedef enum {
	label,
	button,
	combo,
	edit,
	listview
} ControlType;

typedef struct {
	int x;
	int y;
	int width;
	int height;
} Position;