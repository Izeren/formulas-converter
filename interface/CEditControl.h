#pragma once
#include <Windows.h>
#include <string>
#include <utility>

class CEditControl {
public:
	CEditControl();
	~CEditControl();

	bool Create(HWND parentHandle);
	void Show(int cmdShow);
	HWND GetHandle();
	int GetHeight() const;
	int GetWidth() const;
	void SetHeight(int height);
	void SetWidth(int width);
	void SetCountSymbols(int countSymbols);
	int GetCountSymbols() const;
	std::wstring GetText() const;
	void SetFont(HWND handleEditControl);
	int PositionWhiteSpace();
	std::pair<std::wstring, std::wstring> ParseTextByWhiteSpace(size_t positionWhiteSpace);

private:
	HWND handle;
	int height;
	int width;
	int countSymbols;
};