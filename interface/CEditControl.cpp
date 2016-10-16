#include "CEditControl.h"
#include "resource.h"

const int HEIGHT_DEFAULT = 16;
const int WIDTH_DEFAULT = 20;

CEditControl::CEditControl()
{
	handle = 0;
}

CEditControl::~CEditControl()
{
}

bool CEditControl::Create(HWND parentHandle) {
	handle = CreateWindowEx(0, L"EDIT", 0, WS_CHILD | WS_VISIBLE | ES_LEFT ,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parentHandle, 0, GetModuleHandle(0), 0);
	SetFocus(handle);
	return handle != 0;
}

void CEditControl::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
}

HWND CEditControl::GetHandle() {
	return handle;
}

int CEditControl::GetHeight() {
	return HEIGHT_DEFAULT;
}

int CEditControl::GetWidth() {
	return WIDTH_DEFAULT;
}