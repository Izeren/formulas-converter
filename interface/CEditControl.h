#pragma once
#include <Windows.h>

class CEditControl {
public:
	CEditControl();
	~CEditControl();

	bool Create(HWND parentHandle);
	void Show(int cmdShow);
	HWND GetHandle();
	int GetHeight();
	int GetWidth();

private:
	HWND handle;
};