#include "ListView.h"

ListView::ListView(const Position *pos, HINSTANCE instance, HWND parent, int padding)
: Control(new String(WC_LISTVIEW), NULL, pos, instance, LVS_REPORT, parent, NULL, WS_EX_CLIENTEDGE)
{
	columnCount = 0;
	itemIndex = 0;
	this->padding = padding;
	SendMessage(handle, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
}

ListView::~ListView(void)
{
}

void ListView::addColumn(const String *name)
{
	LVCOLUMN col;
	memset(&col, 0, sizeof(LVCOLUMN));
	col.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_FMT | LVCF_MINWIDTH | LVCF_WIDTH;
	col.fmt = LVCFMT_LEFT;
	col.cx = 128;
	col.cxMin = 64;
	col.pszText = (LPTSTR) name->c_str();
	SendMessage(handle, LVM_INSERTCOLUMN, columnCount, LPARAM(&col));
	columnCount++;
}

int ListView::addItem(const String *itemName)
{
	LVITEM item;
	item.mask = LVIF_TEXT;
	item.pszText = (LPTSTR) itemName->c_str();
	item.iItem = itemIndex;
	item.iSubItem = 0;
	SendMessage(handle, LVM_INSERTITEM, 0, (LPARAM) &item);
	int index = itemIndex;
	itemIndex++;

	return index;
}

void ListView::addSubItem(const String *subItemName, int itemIndex, int subItemIndex)
{
	LVITEM item;
	item.mask = LVIF_TEXT;
	item.pszText = (LPTSTR) subItemName->c_str();
	item.iItem = itemIndex;
	item.iSubItem = subItemIndex;
	SendMessage(handle, LVM_SETITEM, 0, (LPARAM) &item);
}

void ListView::resize(int width, int height)
{
	pos.width = width;
	pos.height = height - pos.y;

	MoveWindow(handle, pos.x, pos.y, pos.width - padding, pos.height - padding, TRUE);
}