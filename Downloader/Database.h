#pragma once
#include "sqlite3.h"
#include <string>
#include <sstream>

#ifdef UNICODE
#define sqlite3_open sqlite3_open16
#define String std::wstring
#define StringStream std::wstringstream
#else
#define sqlite3_open sqlite3_open
#define String std::string
#define StringStream std::ostringstream
#endif

class Database
{
	sqlite3 *dbHandle;
public:
	Database(const String *filePath);
	~Database(void);
	void query(std::string *query, int (*callback)(void *, int, char**, char**), void *owner = NULL);
};
