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

	// задаем координаты прямоугольника, как примыкающего слева к neighbour_rect
	void moveLeftAgainstRect(CRect neighbour_rect);
	// задаем координаты прямоугольника, как примыкающего слева к neighbour_rect 
	// центрируем наш прямоугольник относительно neighbour_rect
	void moveLeftCentringHeightAgainstRect(CRect neighbour_rect);
	// задаем координаты прямоугольника, как примыкающего снизу к neighbour_rect 
	void moveDownAgainstRect(CRect neighbour_rect);
	// сместить внутренний прямоугольник
	void offsetInnerRect(CPoint offset);
	// увеличиваем внешний прямоугольник так, чтобы туда помещался rect_around_child
	void unionSubtreeRect(CRect rect_around_child);
	// сместить и внутренний, и внешний прямоугольники
	void offsetBothRects(CPoint offset); 
	// если у neighbour_rect длина больше, чем у нас, то берем его координаты левой и правой точки
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
