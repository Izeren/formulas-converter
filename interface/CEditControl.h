#pragma once

#include <Windows.h>
#include "atltypes.h"

class CEditControl
{
public:

	CEditControl();
	~CEditControl();

	bool Create(HWND parentHandle);
	void Show(int cmdShow);
	HWND GetHandle();
	int GetHeight();
	int GetWidth();
	void SetHeight(int height);
	void SetWidth(int width);
	void SetCountSymbols(int countSymbols);

	CRect GetRect();
	void SetRect(CRect rect_);
	CRect GetRectAroundSubtree();
	void SetRectAroundSubtree(CRect rect_);

private:

	HWND handle;
	int height;
	int width;
	int countSymbols;
	int leftMargin;
	int upperMargin;

	CRect rect;
	CRect rectAroundSubtree;
};
