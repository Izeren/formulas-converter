#pragma once

#include <Windows.h>

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
private:
	static const LPCWSTR class_name_;
	HWND handle_;

	virtual LRESULT __stdcall localWindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};