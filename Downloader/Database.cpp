#include "Database.h"

Database::Database(const String *filePath)
{
	if (!filePath) return;
	String path = *filePath;

	sqlite3_open(path.c_str(), &dbHandle);
}

Database::~Database(void)
{
}

void Database::query(std::string *query, int (*callback)(void *, int, char **, char **), void *owner)
{
	sqlite3_exec(dbHandle, query->c_str(), callback, owner, NULL);
}