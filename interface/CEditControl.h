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
	int GetCountSymbols() const;
	std::wstring GetText() const;
	void SetFont(HWND handleEditControl);
	int PositionWhiteSpace();
	std::pair<std::wstring, std::wstring> ParseTextByWhiteSpace(size_t positionWhiteSpace);

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
