#pragma once

#include <Windows.h>
#include <string>
#include "atltypes.h"


const int MIN_HEIGHT_DEFAULT = 16;
const int MIN_WIDTH_DEFAULT = 8;
const int MIN_SIZE_SYMBOL = 8;
const int SIZE_FONT = 14;
const int COUNT_ADDITIONAL_SYMBOLS = 1;
const int SIZE_MARGIN_DEFAULT = 5;

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
	void SetHeight(int _height);
	void SetWidth(int _width);
	void SetCountSymbols(int _countSymbols);
	int GetCountSymbols() const;
	std::wstring GetText() const;
	void SetFont(HWND handleEditControl);
	void deleteWhiteSpaces();

	CRect GetRect();
	void SetDefaultRect();
	void SetRect(CRect rect_);
	CRect GetRectAroundSubtree();
	void SetDefaultRectAroundSubtree();
	void SetRectAroundSubtree(CRect rect_);

	// ������ ���������� ��������������, ��� ������������ ����� � neighbour_rect
	void moveLeftAgainstRect(CRect neighbour_rect);
	// ������ ���������� ��������������, ��� ������������ ����� � neighbour_rect 
	// ���������� ��� ������������� ������������ neighbour_rect
	void moveLeftCentringHeightAgainstRect(CRect neighbour_rect);
	// ������ ���������� ��������������, ��� ������������ ����� � neighbour_rect 
	void moveDownAgainstRect(CRect neighbour_rect);
	// �������� ���������� �������������
	void offsetInnerRect(CPoint offset);
	// ����������� ������� ������������� ���, ����� ���� ��������� rect_around_child
	void unionSubtreeRect(CRect rect_around_child);
	// �������� � ����������, � ������� ��������������
	void offsetBothRects(CPoint offset); 
	// ���� � neighbour_rect ����� ������, ��� � ���, �� ����� ��� ���������� ����� � ������ �����
	void copyRectWidthIfLarger(CRect neighbour_rect);

	void setIsSpecialSymbol(bool);

private:

	HWND handle;
	int height;
	int width;
	int countSymbols;
	int leftMargin;
	int upperMargin;
	bool isSpecialSymbol = false;

	CRect rect;
	CRect rectAroundSubtree;
};
