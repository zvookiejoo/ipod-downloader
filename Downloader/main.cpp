#pragma setlocale("C")

#include "Window.h"
#include "Database.h"
#include "LibraryItem.h"
#include <regex>

#ifdef UNICODE
#define Regex std::tr1::wregex
#else
#define Regex std::tr1::regex
#endif

Combo *drivesList;
Label *status;
Edit *targetDir;
ListView *trackList;
Button *btnDownload;
Label *summary;
Label *progress;

Window *appWindow;
String caption(TEXT("iPod Downloader"));
String libraryPath;
String locationsPath;
String destPath;
std::vector<LibraryItem *> libItems;
Database *libContainers;
int totalFiles, filesCopied = 0;

void drivesList_SelChange(int notificationCode);
void btnBrowse_Click(int);
bool fileExists(const String *filePath);
bool directoryExists(const String *path);
int libCallback(void *, int, char**, char**);
void btnDownload_Click(int);
String* trim(const String *str);
DWORD WINAPI download(PVOID param);

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR lpCmdline, int nShowCmd)
{
	Position p = {CW_USEDEFAULT, CW_USEDEFAULT, 640, 480};
	appWindow = new Window(&caption, &p, instance);

	p.x = 8; p.y = 8; p.width = 156; p.height = 24;
	Label *label1 = (Label *) appWindow->ControlFactory(label, new String(TEXT("Select drive letter of your iPod:")), &p);

	p.x = 164; p.y = 5; p.width = 80; p.height = 128;
	drivesList = (Combo *) appWindow->ControlFactory(combo, NULL, &p, drivesList_SelChange);

	TCHAR *drive = (TCHAR *) malloc(sizeof(TCHAR) * MAX_PATH);

	GetLogicalDriveStrings(MAX_PATH, (LPTSTR) drive);

	while (*drive) {
		drivesList->addItem(new String(drive));
		drive += _tcslen(drive) + 1;
	}

	p.x = 256; p.y = 8; p.width = 112; p.height = 24;
	status = (Label *) appWindow->ControlFactory(label, NULL, &p);

	p.x = 8; p.y = 40; p.width = 140; p.height = 24;
	Label *targetDirLabel = (Label *) appWindow->ControlFactory(label,
		new String(TEXT("Destination for iPod content:")), &p);

	p.x = 148; p.y = 36; p.width = 256; p.height = 22;
	targetDir = (Edit *) appWindow->ControlFactory(edit, NULL, &p);

	p.x = 412; p.width = 64;
	Button *btnBrowse = (Button *) appWindow->ControlFactory(button,
		new String(TEXT("Browse... ")), &p, btnBrowse_Click);

	p.y = 8; p.width = 128;
	summary = (Label *) appWindow->ControlFactory(label, NULL, &p);
	p.x += 132;
	progress = (Label *) appWindow->ControlFactory(label, NULL, &p);

	p.x = 486; p.width = 80; p.y = 36;
	btnDownload = (Button *) appWindow->ControlFactory(button,
		new String(TEXT("Download all")), &p, btnDownload_Click);

	btnDownload->disable();

	RECT window;
	GetClientRect(appWindow->getHandle(), &window);

	p.x = window.left + 8;
	p.y = window.top + 64;
	p.width = window.right - 16;
	p.height = window.bottom - p.y - 8;
	trackList = (ListView *) appWindow->ControlFactory(listview, NULL, &p);

	trackList->addColumn(new String(TEXT("Artist")));
	trackList->addColumn(new String(TEXT("Album")));
	trackList->addColumn(new String(TEXT("Title")));
	trackList->addColumn(new String(TEXT("Container")));
	trackList->addColumn(new String(TEXT("Local file")));

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) {
		if (!IsDialogMessage(appWindow->getHandle(), &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

void drivesList_SelChange(int notificationCode)
{
	if (notificationCode == CBN_SELENDOK) {
		StringStream ssLibPath;
		StringStream ssLocPath;
		String *temp = drivesList->getSelectedItem();
		ssLibPath << *temp;
		ssLocPath << *temp;
		ssLibPath << TEXT("iPod_Control\\iTunes\\iTunes Library.itlp\\Library.itdb");
		ssLocPath << TEXT("iPod_Control\\iTunes\\iTunes Library.itlp\\Locations.itdb");
		libraryPath.clear();
		locationsPath.clear();
		libraryPath = ssLibPath.str();
		locationsPath = ssLocPath.str();

		if (fileExists(&libraryPath) && fileExists(&locationsPath)) {
			status->setCaption(new String(TEXT("Found iTunes library")));
		} else {
			status->setCaption(new String(TEXT("iTunes library not found")));
			btnDownload->disable();
			return;
		}

		LibraryItem::containersDBPath = locationsPath;
		libItems.clear();
		Database lib(&libraryPath);
		lib.query(new std::string("select pid, title, artist, album from item where media_kind=1 order by artist"), libCallback);

		long long totalSize = 0;

		for (std::vector<LibraryItem *>::iterator i = libItems.begin(); i != libItems.end(); ++i) {
			LibraryItem *item = *i;
			int index = trackList->addItem(item->getArtist());
			trackList->addSubItem(item->getAlbum(), index, 1);
			trackList->addSubItem(item->getTitle(), index, 2);
			trackList->addSubItem(item->getContainer(), index, 3);
			trackList->addSubItem(item->getFilename(), index, 4);
			totalSize += (item->getSize() / 1024) / 1024;
		}

		StringStream *sum = new StringStream();
		*sum << TEXT("Total: ") << totalSize << TEXT(" Gb");

		summary->setCaption(new String(sum->str()));
		totalFiles = libItems.size();
		StringStream *totalProgress = new StringStream;
		*totalProgress << filesCopied << " / " << totalFiles;
		String *tmp = new String(totalProgress->str());
		progress->setCaption(tmp);
		delete totalProgress;
		delete tmp;

		if (!libItems.empty() && !targetDir->getText()->empty())
			btnDownload->enable();
		else
			btnDownload->disable();
	}
}

void btnBrowse_Click(int)
{
	BROWSEINFO bi;

	memset(&bi, 0, sizeof(BROWSEINFO));

	bi.hwndOwner = appWindow->getHandle();
	bi.lpszTitle = TEXT("Select the folder where to place iPod contents:");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl) {
		TCHAR *path = (TCHAR *) malloc(sizeof(TCHAR) * MAX_PATH);
		memset(path, 0, sizeof(TCHAR) * MAX_PATH);
		if (SHGetPathFromIDList(pidl, path)) {
			destPath = *(new String(path));
			destPath.append(TEXT("\\"));
		}
	}

	targetDir->setText(&destPath);
	
	if (!libItems.empty() && !targetDir->getText()->empty())
		btnDownload->enable();
	else
		btnDownload->disable();
}

bool fileExists(const String *filePath)
{
	if (!filePath)
		return false;

	String file = *filePath;

	HANDLE hFile = CreateFile(
		file.c_str(),
		0,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_NO_BUFFERING | FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);

	return (hFile != INVALID_HANDLE_VALUE);
}

int libCallback(void *, int colNumber, char* values[], char* colNames[])
{
	if (colNumber == 4) {
		long long pid = _atoi64(values[0]);
		char *title = values[1];
		char *artist = values[2];
		char *album = values[3];

		LibraryItem *item = new LibraryItem(pid, title, artist, album);

		libItems.push_back(item);
	}

	return 0;
}

void btnDownload_Click(int)
{
	HANDLE downThread = CreateThread(NULL, 0, download, NULL, 0, NULL);
}

DWORD WINAPI download(PVOID param)
{
	filesCopied = 0;
	for (std::vector<LibraryItem *>::iterator it = libItems.begin(); it != libItems.end(); it++)
	{
		StringStream destination;
		LibraryItem *item = *it;

		String fromFile = *drivesList->getSelectedItem();
		fromFile += TEXT("iPod_Control\\Music\\");
		fromFile += *item->getContainer();

		String toFile = destPath;

		Regex pattern(TEXT("[*|\\\\:\"<>?/]|\\.+$"));
		String replace(TEXT(" "));
		Regex antispace(TEXT("^[[:s:]]+$|^\\s+|\\s+$"));
		String sp_replace(TEXT(""));

		String artist = std::tr1::regex_replace(*item->getArtist(), pattern, replace);
		String album = std::tr1::regex_replace(*item->getAlbum(), pattern, replace);
		if (std::tr1::regex_match(album, antispace))
			album = std::tr1::regex_replace(album, antispace, sp_replace);
		String file = std::tr1::regex_replace(*item->getFilename(), pattern, replace);

		toFile += *trim(&artist);

		if (!directoryExists(&toFile))
			CreateDirectory(toFile.c_str(), NULL);

		toFile += TEXT("\\");
		toFile += *trim(&album);

		if (!directoryExists(&toFile))
			CreateDirectory(toFile.c_str(), NULL);

		toFile += TEXT("\\");
		toFile += *trim(&file);

		if (!fileExists(&toFile)) {
			if (!CopyFile(fromFile.c_str(), toFile.c_str(), FALSE)) {
				StringStream err;
				err << "Could not copy file. Error " << GetLastError();
				String text = err.str();
				MessageBox(appWindow->getHandle(), text.c_str(), caption.c_str(), MB_ICONERROR | MB_OK);
				break;
			}
		}

		filesCopied++;
		StringStream *progr = new StringStream();
		*progr << filesCopied << TEXT(" / ") << totalFiles;
		String *cap = new String(progr->str());
		progress->setCaption(cap);
		delete cap;
		delete progr;
	}

	ExitThread(0);
}

bool directoryExists(const String *path)
{
	DWORD result = GetFileAttributes(path->c_str());
	
	if (result == -1)
		return false;

	if (result & FILE_ATTRIBUTE_DIRECTORY)
		return true;

	return false;
}

String* trim(const String *str)
{
	Regex pattern(TEXT("^[[:s:]]+|[[:s:]]+$"));
	
	String replace(TEXT(""));
	String *result = new String;
	*result = std::tr1::regex_replace(*str, pattern, replace);
	return result;
}