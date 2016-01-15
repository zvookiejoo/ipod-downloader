#include "Window.h"

Window::Window(const String *title, const Position *pos, HINSTANCE instance)
{
	if (!title) return;
	if (!pos) return;

	this->className = TEXT("AppWindow");
	this->title = *title;
	this->instance = instance;
	this->controlId = CTL_ID_START;
	this->actions = new std::map<int, void (*)(int)>;
	this->resizableControls = new std::vector<Control *>;

	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = this->instance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.lpszClassName = this->className.c_str();
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Window::WindowProc;

	if (!RegisterClassEx(&wc)) {
		StringStream ss;
		ss << TEXT("Could not register window class. Error ") << GetLastError();
		String error(ss.str());
		MessageBox(NULL, error.c_str(), this->title.c_str(), MB_ICONERROR | MB_OK);
		exit(1);
	}

	this->handle = CreateWindowEx(0, this->className.c_str(), this->title.c_str(), WS_OVERLAPPEDWINDOW | WS_GROUP,
		pos->x, pos->y, pos->width, pos->height, NULL, NULL, this->instance, NULL);

	if (!handle) {
		StringStream ss;
		ss << TEXT("Could not create application window. Error ") << GetLastError();
		String error(ss.str());
		MessageBox(NULL, error.c_str(), this->title.c_str(), MB_ICONERROR | MB_OK);
		exit(2);
	}

	INITCOMMONCONTROLSEX icc;

	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC = ICC_WIN95_CLASSES;

	InitCommonControlsEx(&icc);

	ShowWindow(this->handle, SW_SHOW);

	SetWindowLong(this->handle, GWL_USERDATA, (long) this);
}

Window::~Window(void)
{
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window *wnd = (Window *) GetWindowLong(hWnd, GWL_USERDATA);
	void (*action)(int) = NULL;

	switch(uMsg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_COMMAND:
			if (LOWORD(wParam) < CTL_ID_START)
				break;
			action = wnd->actions->find(LOWORD(wParam))->second;
			if (action)
				action(HIWORD(wParam));
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		case WM_GETMINMAXINFO: {
			MINMAXINFO *mmi = (MINMAXINFO *) lParam;
			mmi->ptMinTrackSize.x = 640;
			mmi->ptMinTrackSize.y = 480; }
			break;
		case WM_SIZE:
			if (!wnd) break;
			std::vector<Control *> *resizable = wnd->getResizableControls();
			for (std::vector<Control *>::iterator it = resizable->begin(); it != resizable->end(); it++)
			{
				Control *ctrl = *it;
				ctrl->resize(LOWORD(lParam), HIWORD(lParam));
			}
			break;
	}

	return 0;
}

Control* Window::ControlFactory(ControlType type, const String *caption, const Position *pos, void (*action)(int))
{
	Control *control = NULL;

	switch(type) {
		case label:
			control = new Label(caption, pos, this->instance, this->handle);
			break;
		case button:
			control = new Button(caption, pos, this->instance, this->handle, this->controlId);
			this->actions->insert(std::make_pair(this->controlId, action));
			this->controlId++;
			break;
		case edit:
			control = new Edit(pos, this->instance, this->handle, this->controlId);
			this->actions->insert(std::make_pair(this->controlId, action));
			this->controlId++;
			break;
		case combo:
			control = new Combo(pos, instance, handle, controlId);
			actions->insert(std::make_pair(controlId, action));
			controlId++;
			break;
		case listview:
			control = new ListView(pos, instance, handle, 16);
			resizableControls->push_back(control);
			break;
	}

	return control;
}

HWND Window::getHandle()
{
	return this->handle;
}

void Window::addStyle(DWORD dwStyle)
{
	long temp = GetWindowLong(handle, GWL_STYLE);
	temp |= dwStyle;
	SetWindowLong(handle, GWL_STYLE, temp);
	UpdateWindow(handle);
}

void Window::removeStyle(DWORD dwStyle)
{
	long temp = GetWindowLong(handle, GWL_STYLE);
	temp &= ~dwStyle;
	SetWindowLong(handle, GWL_STYLE, temp);
	UpdateWindow(handle);
}

std::vector<Control *> * Window::getResizableControls()
{
	return this->resizableControls;
}