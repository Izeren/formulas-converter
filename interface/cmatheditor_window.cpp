#pragma comment(lib, "comctl32.lib")
#include "cmatheditor_window.h"
#include <Windows.h>
#include <commctrl.h>

const LPCWSTR CMatheditorWindow::class_name_ = L"MatheditorWindow";
const int ToolbarSize = 42;

CMatheditorWindow::CMatheditorWindow() : handle_(0) {
	editControl = CEditControl();
}

CMatheditorWindow::~CMatheditorWindow() {
	if (handle_) {
		DestroyWindow(handle_);
	}
}

bool CMatheditorWindow::RegisterClassW() {
	WNDCLASSEX wnd_class = { };
	wnd_class.cbSize = sizeof(WNDCLASSEX);
	wnd_class.style = CS_HREDRAW | CS_VREDRAW;
	wnd_class.lpfnWndProc = windowProc;
	wnd_class.hInstance = GetModuleHandle(NULL);
	wnd_class.lpszClassName = class_name_;
	wnd_class.hCursor = ::LoadCursor(GetModuleHandle(0), IDC_ARROW);
	return RegisterClassEx(&wnd_class);
}

bool CMatheditorWindow::Create() {
	handle_ = CreateWindowEx(0, class_name_,
		L"Matheditor",
		WS_EX_OVERLAPPEDWINDOW | WS_SIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		this
);
	if (handle_ == NULL) {
		return false;
	}
	//HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0, handle_, NULL, GetModuleHandle(NULL), NULL);
	// Declare and initialize local constants.
	const int ImageListID = 0;
	const int numButtons = 3;
	const int bitmapSize = 16;

	const DWORD buttonStyles = BTNS_AUTOSIZE;

	// Create the toolbar.
	HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
		WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0,
		handle_, NULL, GetModuleHandle(NULL), NULL);

	if (hWndToolbar == NULL)
		return NULL;

	HIMAGELIST g_hImageList = NULL;

	// Create the image list.
	g_hImageList = ImageList_Create(bitmapSize, bitmapSize,   // Dimensions of individual bitmaps.
		ILC_COLOR16 | ILC_MASK,   // Ensures transparent background.
		numButtons, 0);

	// Set the image list.
	SendMessage(hWndToolbar, TB_SETIMAGELIST,
		(WPARAM)ImageListID,
		(LPARAM)g_hImageList);

	// Load the button images.
	SendMessage(hWndToolbar, TB_LOADIMAGES,
		(WPARAM)IDB_STD_SMALL_COLOR,
		(LPARAM)HINST_COMMCTRL);

	// Initialize button info.
	// IDM_NEW, IDM_OPEN, and IDM_SAVE are application-defined command constants.

	TBBUTTON tbButtons[numButtons] =
	{
		{ MAKELONG(STD_FILENEW,  ImageListID), NULL,  TBSTATE_ENABLED, buttonStyles,{ 0 }, 0, (INT_PTR)L"New" },
		{ MAKELONG(STD_FILEOPEN, ImageListID), NULL, TBSTATE_ENABLED, buttonStyles,{ 0 }, 0, (INT_PTR)L"Open" },
		{ MAKELONG(STD_FILESAVE, ImageListID), NULL, 0,               buttonStyles,{ 0 }, 0, (INT_PTR)L"Save" }
	};

	// Add buttons.
	SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);

	// Resize the toolbar, and then show it.
	SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(hWndToolbar, TRUE);

	return hWndToolbar;
	return true;
}

void CMatheditorWindow::Show(int cmdShow) {
	ShowWindow(handle_, cmdShow);
	editControl.Show(cmdShow);
	UpdateWindow(handle_);
}

void CMatheditorWindow::OnCreate() {
	editControl.Create(handle_);
}

void CMatheditorWindow::OnNCCreate(HWND handle) {
	handle_ = handle;
}

void CMatheditorWindow::OnSize()
{
	RECT rect;
	::GetClientRect(handle_, &rect);
	SetWindowPos(editControl.GetHandle(), HWND_TOP, rect.left, rect.top + ToolbarSize, rect.right - rect.left, rect.bottom - rect.top, 0);
}

LRESULT _stdcall CMatheditorWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_NCCREATE: {
		CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
		CMatheditorWindow* overlapped_window = (CMatheditorWindow*)cs->lpCreateParams;
		SetLastError(0);
		if (SetWindowLongPtr(handle, GWLP_USERDATA, (LONG)overlapped_window) == 0 &&
			GetLastError() != 0) {
				return FALSE;
		}
		overlapped_window->OnNCCreate(handle);
		return TRUE;
	}
	default:
		CMatheditorWindow* overlapped_window = (CMatheditorWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		if (overlapped_window) {
			return overlapped_window->localWindowProc(handle, message, wParam, lParam);
		}
		return DefWindowProc(handle, message, wParam, lParam);
	}
	return 0;
}

void CMatheditorWindow::OnDestroy() {
    PostQuitMessage(0);
}

LRESULT _stdcall CMatheditorWindow::localWindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_DESTROY:
		OnDestroy();
		break;
	case WM_CREATE:
		OnCreate();
		return DefWindowProc(handle, message, wParam, lParam);
	case WM_SIZE:
		OnSize();
		return DefWindowProc(handle, message, wParam, lParam);
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
	return 0;
}


