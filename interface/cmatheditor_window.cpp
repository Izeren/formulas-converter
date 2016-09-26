#include "cmatheditor_window.h"

#include <Windows.h>

const LPCWSTR CMatheditorWindow::class_name_ = L"MatheditorWindow";

CMatheditorWindow::CMatheditorWindow() : handle_(0) {
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
		WS_OVERLAPPED,
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
	return true;
}

void CMatheditorWindow::Show(int cmdShow) {
	ShowWindow(handle_, cmdShow);
	UpdateWindow(handle_);
}

void CMatheditorWindow::OnNCCreate(HWND handle) {
	handle_ = handle;
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

void CMatheditorWindow::OnCreate() {
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
		break;
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
	return 0;
}
