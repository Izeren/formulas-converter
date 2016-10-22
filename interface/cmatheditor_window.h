#pragma once

#include <Windows.h>
#include "resource.h"
#include "CEditControl.h"
#include <list>
#include <map>
#include "TreeVisualisation.h"

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
	void OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnCtlColorEdit(WPARAM wParam, LPARAM lParam);

private:
	static const LPCWSTR class_name_;
	HWND hWndMainWindow;
	HWND hWndToolbar;

	std::list<CEditControl> editControls;
	std::list<CEditControl>::iterator activeEditControl;
	std::map<HWND, std::list<CEditControl>::iterator> editControlsHandles;

	TreeVisualisation* editControlsTree = nullptr;

	virtual LRESULT __stdcall localWindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	void createToolbar();
	HBITMAP loadTransparentBitmap(HINSTANCE hInstance, int resource);

	void newFile();
	void saveFile();
	void loadFile();

	void clickEditControl();
	void createEditControl();
	void createEditControl(std::wstring text);
	void deleteEditControl();
};