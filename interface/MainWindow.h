#pragma once

#include <Windows.h>
#include <commctrl.h>

#include "resource.h"
#include "TreeVisualisation.h"

class MainWindow
{

public:
	
	MainWindow();
	~MainWindow();

	static bool RegisterClass();

	bool Create();
	void Show(int cmdShow);

protected:

	void OnNCCreate(HWND handle);
	void OnCreate();
	void OnPaint();
	void OnSize();
	void OnDestroy();
	void OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnCtlColorEdit(WPARAM wParam, LPARAM lParam);
	bool OnClose();

private:

	HWND handleMainWindow;
	HWND handleToolbar;
	TreeVisualisation treeOfEdits;

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	void createToolbar();
	HBITMAP loadTransparentBitmap(HINSTANCE hInstance, int resource);
};
