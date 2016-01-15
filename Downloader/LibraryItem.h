#pragma once
#include <string>
#include <windows.h>
#include "Database.h"

class LibraryItem
{
	std::string title;
	std::string artist;
	std::string album;
	long long pid;
	std::string containerPath;
	static Database *containers;
	std::string localPath;
	wchar_t * mb2wc(const char *str);
	long long size;
public:
	static String containersDBPath;
	static int containerCallback(void *param, int colNumber, char* values[], char* colNames[]);
	LibraryItem(long long pid, const char *title, const char *artist, const char *album);
	~LibraryItem(void);
	String * getArtist();
	String * getAlbum();
	String * getTitle();
	String * getFilename();
	String * getContainer();
	long long getSize();
};
