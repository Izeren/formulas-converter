#include "CEditControl.h"
#include "resource.h"

const int MIN_HEIGHT_DEFAULT = 16;
const int MIN_WIDTH_DEFAULT = 10;

const int SIZE_MARGIN_DEFAULT = 5;

CEditControl::CEditControl()
{
	height = MIN_HEIGHT_DEFAULT;
	width = MIN_WIDTH_DEFAULT;
	handle = 0;
	leftMargin = SIZE_MARGIN_DEFAULT;
	upperMargin = -SIZE_MARGIN_DEFAULT;
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

HWND CEditControl::GetHandle() const
{
	return handle;
}

int CEditControl::GetHeight() const
{
	return height;
}

int CEditControl::GetWidth() const
{
	return height;
}

void CEditControl::SetHeight(int height)
{
	if( height >= MIN_HEIGHT_DEFAULT ) {
		this->height = height;
	}
}

void CEditControl::SetWidth(int width) {
	if( width >= MIN_WIDTH_DEFAULT )
	{
		this->width = width;
	}
}

void CEditControl::SetCountSymbols(int countSymbols)
{
	if( countSymbols >= this->countSymbols ) {
		this->countSymbols = countSymbols;
		SetWidth(this->countSymbols * MIN_WIDTH_DEFAULT);
		SetWindowPos(handle, HWND_TOP, 0, 0, width, height, SWP_NOMOVE);
	}
}
