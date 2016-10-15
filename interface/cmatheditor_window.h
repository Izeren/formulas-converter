#pragma once

#include <Windows.h>
#include "resource.h"
#include "CEditControl.h"

class CMatheditorWindow {
public:
	CMatheditorWindow();
	~CMatheditorWindow();

	static bool RegisterClassW();

	bool Create();
	void Show(int cmdShow);

protected:
	void OnDestroy();
	void OnNCCreate(HWND handle);
	void OnCreate();
	void OnSize();
private:
	static const LPCWSTR class_name_;
	HWND hWndMainWindow;

	CEditControl editControl;

	virtual LRESULT __stdcall localWindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	void createToolbar();
};