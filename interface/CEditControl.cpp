#include "CEditControl.h"
#include "resource.h"
#include <string>

CEditControl::CEditControl()
{
	height = MIN_HEIGHT_DEFAULT;
	width = MIN_WIDTH_DEFAULT;
	countSymbols = 0;
	handle = 0;
	leftMargin = SIZE_MARGIN_DEFAULT;
	upperMargin = -SIZE_MARGIN_DEFAULT;
	rect = CRect(0, 0, 0, 0);
	rectAroundSubtree = CRect(0, 0, 0, 0);
}

CEditControl::~CEditControl()
{
}

bool CEditControl::Create(HWND parentHandle) {
	handle = CreateWindowEx(0, L"EDIT", 0, WS_CHILD | WS_VISIBLE | ES_CENTER ,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parentHandle, 0, GetModuleHandle(0), 0);
	SetFocus(handle);
	SetFont(handle);
	return handle != 0;
}

void CEditControl::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
}

HWND CEditControl::GetHandle()
{
	return handle;
}

int CEditControl::GetHeight()
{
	return height;
}

int CEditControl::GetWidth()
{
	return width;
}

void CEditControl::SetHeight(int _height)
{
	if ( _height >= MIN_HEIGHT_DEFAULT ) {
		this->height = _height;
	}
}

void CEditControl::SetWidth(int _width) {
	if ( _width >= MIN_WIDTH_DEFAULT 
		//&& !isSpecialSymbol 
		)
	{
		this->width = _width;
	}
}

void CEditControl::SetCountSymbols(int _countSymbols)
{
	if ( _countSymbols >= this->countSymbols && !isSpecialSymbol ) {
		this->countSymbols = _countSymbols;
		SetWidth(this->countSymbols * MIN_SIZE_SYMBOL);
		SetWindowPos(GetHandle(), HWND_TOP, 0, 0, width, height, SWP_NOMOVE);
	}
}

CRect CEditControl::GetRect()
{
	return rect;
}

void CEditControl::SetDefaultRect()
{
	// should be corrected
	// ширина должна определяться по количеству символов
	rect = CRect(0, 0, GetWidth(), GetHeight());
	//rect = CRect(0, 0, MIN_WIDTH_DEFAULT, MIN_HEIGHT_DEFAULT);
}

void CEditControl::SetRect(CRect rect_)
{
	rect = rect_;
}

CRect CEditControl::GetRectAroundSubtree()
{
	return rectAroundSubtree;
}

void CEditControl::SetDefaultRectAroundSubtree()
{
	rectAroundSubtree = rect;
}

void CEditControl::SetRectAroundSubtree(CRect rectAroundSubtree_)
{
	rectAroundSubtree = rectAroundSubtree_;
}

void CEditControl::moveLeftAgainstRect(CRect neighbour_rect) {
	//int top = neighbour_rect.top + (neighbour_rect.Height() - GetHeight()) / 2;
	//rect = CRect(POINT{ neighbour_rect.right, top }, SIZE{ GetWidth(), GetHeight() });
	rect = CRect(POINT{ neighbour_rect.right, neighbour_rect.top }, SIZE{ GetWidth(), GetHeight() });
}

void CEditControl::moveLeftCentringHeightAgainstRect(CRect neighbour_rect) {
	int top = neighbour_rect.top + (neighbour_rect.Height() - GetHeight()) / 2;
	rect = CRect(POINT{ neighbour_rect.right, top }, SIZE{ GetWidth(), GetHeight() });
}

void CEditControl::moveDownAgainstRect(CRect neighbour_rect) {
	//int left = neighbour_rect.left + (neighbour_rect.Width() - GetWidth()) / 2;
	//rect = CRect(POINT{ left, neighbour_rect.bottom }, SIZE{ GetWidth(), GetHeight() });
	rect = CRect(POINT{ neighbour_rect.left, neighbour_rect.bottom }, SIZE{ GetWidth(), GetHeight() });
}

void CEditControl::copyRectWidthIfLarger(CRect neighbour_rect) {
	if (neighbour_rect.Width() > GetWidth()) {
		SetWidth(neighbour_rect.Width());
		rect = CRect(POINT{ neighbour_rect.left, rect.top }, SIZE{ GetWidth(), GetHeight() });
	}
}

void CEditControl::offsetInnerRect(CPoint offset) {
	rect.OffsetRect(offset);
}

void CEditControl::unionSubtreeRect(CRect rect_around_child) {
	rectAroundSubtree.UnionRect(rectAroundSubtree, rect_around_child);
}

void CEditControl::offsetBothRects(CPoint offset) {
	rect.OffsetRect(offset);
	rectAroundSubtree.OffsetRect(offset);
}

void CEditControl::deleteWhiteSpaces() {
	std::wstring text = this->GetText();
	size_t position = text.find(' ');
	if (position != std::string::npos) {
		text.replace(position, 1, L"");
		::SetWindowText(handle, (LPWSTR)text.c_str());
	}
	this->SetCountSymbols(text.size() + COUNT_ADDITIONAL_SYMBOLS);
}

int CEditControl::GetCountSymbols() const {
	return countSymbols;
}

std::wstring CEditControl::GetText() const {
	int length = SendMessage(handle, WM_GETTEXTLENGTH, 0, 0);
	length++;
	std::wstring text;
	text.resize(length);
	::GetWindowText(handle, (LPWSTR)text.c_str(), length);
	return text;
}

void CEditControl::SetFont(HWND handleEditControl) {
	HFONT font = (HFONT)::SendMessage(handleEditControl, WM_GETFONT, 0, 0);
	if (!font) {
		font = (HFONT)::GetStockObject(SYSTEM_FIXED_FONT);
	}
	LOGFONT logfont;
	::GetObject(font, sizeof(LOGFONT), &logfont);
	logfont.lfHeight = SIZE_FONT;
	DeleteObject(font);
	font = ::CreateFontIndirect(&logfont);
	::SendMessage(handleEditControl, WM_SETFONT, (WPARAM)font, true);
}

void CEditControl::setIsSpecialSymbol(bool _isSpecialSymbol) {
	this->isSpecialSymbol = _isSpecialSymbol;
}