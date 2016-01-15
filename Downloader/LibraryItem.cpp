#include "LibraryItem.h"

String LibraryItem::containersDBPath =TEXT("");
Database *LibraryItem::containers = NULL;

LibraryItem::LibraryItem(long long pid, const char *title, const char *artist, const char *album)
{
	if (!title) return;
	if (!artist) return;
	
	if (!containers)
		containers = new Database(&LibraryItem::containersDBPath);

	this->pid = pid;
	this->title = title;
	this->artist = artist;
	this->album = (album != NULL) ? album : "";

	std::ostringstream *temp = new std::ostringstream();

	*temp << "select location, file_size from location where item_pid=" << pid;
	std::string *query = new std::string(temp->str());

	delete temp;

	containers->query(query, LibraryItem::containerCallback, this);
	delete query;

	std::ostringstream *path = new std::ostringstream();		
	*path << this->title << ".";
	*path << this->containerPath.substr(containerPath.length() - 3);

	containerPath.replace(containerPath.find("/"), 1, "\\");

	localPath = path->str();

	delete path;
}

LibraryItem::~LibraryItem(void)
{
}

int LibraryItem::containerCallback(void *param, int colNumber, char *values[], char *colNames[])
{
	LibraryItem *item = (LibraryItem *) param;
	item->containerPath = values[0];
	item->size = _atoi64(values[1]);
	return 0;
}

String * LibraryItem::getArtist()
{
	String *result = new String(mb2wc(artist.c_str()));
	return result;
}

String * LibraryItem::getAlbum()
{
	String *result = new String(mb2wc(album.c_str()));
	return result;
}

String * LibraryItem::getTitle()
{
	String *result = new String(mb2wc(title.c_str()));
	return result;
}

wchar_t * LibraryItem::mb2wc(const char *str)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	wchar_t *result = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, result, size);
	return result;
}

String * LibraryItem::getFilename()
{
	String *result = new String(mb2wc(localPath.c_str()));
	return result;
}

String * LibraryItem::getContainer()
{
	String *result = new String(mb2wc(containerPath.c_str()));
	return result;
}

long long LibraryItem::getSize()
{
	return size;
}