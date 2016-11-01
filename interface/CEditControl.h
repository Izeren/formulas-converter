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
