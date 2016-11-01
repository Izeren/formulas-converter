#pragma once

#include <Windows.h>
#include "resource.h"
#include "CEditControl.h"
#include <list>
#include <map>
#include "TreeVisualisation.h"

#include "./../LSV_module/expression_tree/Expression.h"
#include "./../LSV_module/parsers/MathMLParser.h"

class CMatheditorWindow {
public:
	CMatheditorWindow();
	~CMatheditorWindow();

	static bool RegisterClassW();

	bool Create();
	void Show(int cmdShow);

	HACCEL GetHaccel() const;
	HWND GetHandle() const;

	void loadFromLSVModule();

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

	std::shared_ptr<TreeVisualisation*> editControlsTree = nullptr;

	virtual LRESULT __stdcall localWindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	void createToolbar();
	HBITMAP loadTransparentBitmap(HINSTANCE hInstance, int resource);

	void newFile();
	void saveFile();
	void loadFile();

	HACCEL haccel;

	void clickEditControl();
	//void createEditControl();
	//void createEditControl(std::wstring text);
	void deleteEditControl();

	void createEditControl(NodeType nodeType);

	void recursiveTransformation(TreeVisualisation* tree, std::shared_ptr<IExpression> expr);
};
