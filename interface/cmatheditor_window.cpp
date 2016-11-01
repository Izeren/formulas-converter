#pragma comment(lib, "comctl32.lib")
#include "cmatheditor_window.h"
#include <Windows.h>
#include <commctrl.h>

const LPCWSTR CMatheditorWindow::class_name_ = L"MatheditorWindow";
const COLORREF COLOUR_MAIN_WINDOW = RGB(200, 200, 200);

const int SIZE_BETWEEN_CONTROLS = 10;
const int CONTROL_WIDTH = 20;


CMatheditorWindow::CMatheditorWindow() 
	: hWndMainWindow(0) {
	editControls.resize(0);
	activeEditControl = editControls.end();
	//editControl = CEditControl();
}

CMatheditorWindow::~CMatheditorWindow() {
	if( hWndMainWindow ) {
		DestroyWindow(hWndMainWindow);
	}
}

bool CMatheditorWindow::RegisterClassW() {
	WNDCLASSEX windowClass = { };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = windowProc;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.lpszClassName = class_name_;
	windowClass.hCursor = ::LoadCursor(GetModuleHandle(0), IDC_ARROW);
	windowClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	if (RegisterClassEx(&windowClass)) {
		return true;
	}
	else {
		return false;
	}
}

bool CMatheditorWindow::Create() {
	hWndMainWindow = CreateWindowEx(0, class_name_,
		L"Matheditor",
		WS_EX_OVERLAPPEDWINDOW | WS_SIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		this
	);
	if (hWndMainWindow == NULL) {
		return false;
	}

	createToolbar();

	editControlsTree = std::shared_ptr<TreeVisualisation*>(new TreeVisualisation(hWndMainWindow));

	return true;
}

HBITMAP MakeBitMapTransparent(HBITMAP hbmSrc) {
		HDC hdcSrc, hdcDst;
		BITMAP bm;
		COLORREF clrTP, clrBK;
		HBITMAP hbmOld, hbmNew = NULL;

		if ((hdcSrc = CreateCompatibleDC(NULL)) != NULL) {
				if ((hdcDst = CreateCompatibleDC(NULL)) != NULL) {
						int nRow, nCol;
						GetObject(hbmSrc, sizeof(bm), &bm);
						hbmOld = (HBITMAP)SelectObject(hdcSrc, hbmSrc);
						hbmNew = CreateBitmap(bm.bmWidth, bm.bmHeight, bm.bmPlanes, bm.bmBitsPixel, NULL);
						SelectObject(hdcDst, hbmNew);

						BitBlt(hdcDst, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);

						clrTP = GetPixel(hdcDst, 1, 1);// Get color of first pixel at 0,0
						clrBK = GetSysColor(COLOR_MENU);// Get the current background color of the menu

						for (nRow = 0; nRow < bm.bmHeight; nRow++) {// work our way through all the pixels changing their color
							for (nCol = 0; nCol < bm.bmWidth; nCol++) {// when we hit our set transparency color.
								if (GetPixel(hdcDst, nCol, nRow) == clrTP) {
									SetPixel(hdcDst, nCol, nRow, clrBK);
								}
							}
						}
						DeleteDC(hdcDst);
				}
				DeleteDC(hdcSrc);
		}
		return hbmNew;// return our transformed bitmap.
}

HBITMAP CMatheditorWindow::loadTransparentBitmap(HINSTANCE hInstance, int resource) {
	return MakeBitMapTransparent(LoadBitmap(hInstance, MAKEINTRESOURCE(resource)));
}

void CMatheditorWindow::createToolbar() {

	// Declare and initialize local constants.
	const int bitmapSize = 16;

	HINSTANCE hInstance = ::GetModuleHandle(0);

	hWndToolbar = CreateToolbarEx(hWndMainWindow, WS_CHILD | WS_VISIBLE | CCS_TOP, 1,
		0, HINST_COMMCTRL, IDB_STD_SMALL_COLOR, NULL, 0, 0, 0, 0, 0, sizeof(TBBUTTON));

	//Enable multiple image lists
	SendMessage(hWndToolbar, CCM_SETVERSION, (WPARAM)5, 0);

	//Add icons from default imagelist
	TBBUTTON tbb_buildin[] = {
		{ STD_FILENEW, ID_FILE_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ STD_FILEOPEN, ID_FILE_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ STD_FILESAVE, ID_FILE_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
	};
	SendMessage(hWndToolbar, (UINT)TB_ADDBUTTONS, _countof(tbb_buildin), (LPARAM)&tbb_buildin);

	//Create custom imagelist
	HIMAGELIST hImageList = ImageList_Create(bitmapSize, bitmapSize, ILC_COLOR16 | ILC_MASK, 0, 0);
	ImageList_Add(hImageList, loadTransparentBitmap(hInstance, IDB_EQUAL), NULL);
	ImageList_Add(hImageList, loadTransparentBitmap(hInstance, IDB_PLUS), NULL);
	ImageList_Add(hImageList, loadTransparentBitmap(hInstance, IDB_MINUS), NULL);
	ImageList_Add(hImageList, loadTransparentBitmap(hInstance, IDB_MULTIPLY), NULL);
	ImageList_Add(hImageList, loadTransparentBitmap(hInstance, IDB_DIVIDE), NULL);
	ImageList_Add(hImageList, loadTransparentBitmap(hInstance, IDB_POWER), NULL);
	ImageList_Add(hImageList, loadTransparentBitmap(hInstance, IDB_SUMM), NULL);
	ImageList_Add(hImageList, loadTransparentBitmap(hInstance, IDB_EMPTY), NULL);
	ImageList_Add(hImageList, loadTransparentBitmap(hInstance, IDB_DELETE), NULL);
	SendMessage(hWndToolbar, TB_SETIMAGELIST, (WPARAM)1, (LPARAM)hImageList);

	TBBUTTON tbb[] =
	{
		{ MAKELONG(7, 1), ID_OPERATOR_EMPTY, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(0, 1), ID_OPERATOR_EQUAL, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(1, 1), ID_OPERATOR_PLUS, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(2, 1), ID_OPERATOR_MINUS, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(3, 1), ID_OPERATOR_MULTIPLY, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(4, 1), ID_OPERATOR_DIVIDE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(5, 1), ID_OPERATOR_POWER, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(6, 1), ID_OPERATOR_SUMM, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(8, 1), ID_OPERATOR_DELETE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
	};

	SendMessage(hWndToolbar, (UINT)TB_ADDBUTTONS, _countof(tbb), (LPARAM)&tbb);

	SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(hWndToolbar, TRUE);
}

void CMatheditorWindow::Show(int cmdShow) {
	ShowWindow(hWndMainWindow, cmdShow);
	//editControl.Show(cmdShow);
	//UpdateWindow(hWndMainWindow); //зачем это?
}

void CMatheditorWindow::OnCreate() {
	//createEditControl();
	//editControl.Create(hWndMainWindow);
}

void CMatheditorWindow::OnNCCreate(HWND hWnd) {
	hWndMainWindow = hWnd;
}

void CMatheditorWindow::OnSize()
{
	InvalidateRect(hWndMainWindow, NULL, FALSE);

	PAINTSTRUCT paintStruct;
	HBRUSH brush;
	brush = CreateSolidBrush(COLOUR_MAIN_WINDOW);
	::BeginPaint(hWndMainWindow, &paintStruct);
	::FillRect(paintStruct.hdc, &paintStruct.rcPaint, brush);
	::EndPaint(hWndMainWindow, &paintStruct);


	RECT mainRect;
	::GetClientRect(hWndMainWindow, &mainRect);

	RECT toolbarRect;
	::GetClientRect(hWndToolbar, &toolbarRect);

	int currentTop = mainRect.top + (toolbarRect.bottom - toolbarRect.top) + SIZE_BETWEEN_CONTROLS;
	int currentLeft = mainRect.left + SIZE_BETWEEN_CONTROLS;
	if (editControlsTree.get()) {
		(*editControlsTree.get())->paint(currentTop, currentLeft);
	}

	//int currentTop = mainRect.top + (toolbarRect.bottom - toolbarRect.top) + SIZE_BETWEEN_CONTROLS;
	//int currentLeft = mainRect.left + SIZE_BETWEEN_CONTROLS;
	//for (auto window = editControls.begin(); window != editControls.end(); ++window) {
	//	::SetWindowPos(window->GetHandle(), HWND_TOP, currentLeft, currentTop, window->GetWidth(), window->GetHeight(), 0);
	//	currentLeft += SIZE_BETWEEN_CONTROLS + window->GetWidth();
	//}

	SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
}

LRESULT CMatheditorWindow::OnCtlColorEdit(WPARAM wParam, LPARAM lParam)
{
	HDC hdc = reinterpret_cast<HDC>(wParam);
	HBRUSH hbr = 0;
	if (lParam == reinterpret_cast<long>(GetFocus())) {
		hbr = CreateSolidBrush(RGB(255, 255, 0));
		SetBkColor(hdc, RGB(255, 255, 0));
	}
	return reinterpret_cast<LRESULT>(hbr);
}

LRESULT _stdcall CMatheditorWindow::windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_NCCREATE: {
			CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
			CMatheditorWindow* overlapped_window = (CMatheditorWindow*)cs->lpCreateParams;
			SetLastError(0);
			if (SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)overlapped_window) == 0 &&
				GetLastError() != 0) {
				return FALSE;
			}
			overlapped_window->OnNCCreate(hWnd);
			return TRUE;
		}
		default: {
			CMatheditorWindow* overlapped_window = (CMatheditorWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			if (overlapped_window) {
				return overlapped_window->localWindowProc(hWnd, message, wParam, lParam);
			}
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return 0;
}

void CMatheditorWindow::OnDestroy() {
    PostQuitMessage(0);
}

LRESULT _stdcall CMatheditorWindow::localWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_DESTROY:
		OnDestroy();
		break;
	case WM_CREATE:
		OnCreate();
		return DefWindowProc(hwnd, message, wParam, lParam);
	case WM_SIZE:
		OnSize();
		return DefWindowProc(hwnd, message, wParam, lParam);
	case WM_COMMAND:
		//switch (HIWORD(wParam)) { // ID control
		//case 1:
		//{
		//	switch (LOWORD(wParam)) {
		//	case ID_ACCELERATOR40012:
		//	{
		//		::PostQuitMessage(0);
		//		return 0;
		//	}
		//	}
		//}
		//default:
		//{
			OnCommand(hwnd, message, wParam, lParam);
		/*}
		}*/
		return DefWindowProc(hwnd, message, wParam, lParam);
	case WM_CTLCOLOREDIT: {
		return OnCtlColorEdit(wParam, lParam);
	}
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

void CMatheditorWindow::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == 0) {
		switch (LOWORD(wParam)) {
		case ID_FILE_NEW:
			newFile();
			break;
		case ID_FILE_EXIT:
			SendMessage(hWnd, WM_CLOSE, wParam, lParam);
			break;
		case ID_FILE_SAVE:
			saveFile();
			break;
		case ID_FILE_OPEN:
			loadFile();
			break;
		case ID_OPERATOR_EMPTY:
			createEditControl(Value);
			break;
		case ID_OPERATOR_EQUAL:
			createEditControl(Assign);
			break;
		case ID_OPERATOR_PLUS:
			createEditControl(Plus);
			break;
		case ID_OPERATOR_MINUS:
			createEditControl(Minus);
			break;
		case ID_OPERATOR_MULTIPLY:
			createEditControl(Multiply);
			break;
		case ID_OPERATOR_DIVIDE:
			createEditControl(Divide);
			break;
		case ID_OPERATOR_POWER:
			createEditControl(Power);
			break;
		case ID_OPERATOR_SUMM:
			createEditControl(Summ);
			break;
		case ID_OPERATOR_DELETE:
			deleteEditControl();
			break;
		default:
			break;
		}
	}
	else {
		switch (HIWORD(wParam)) {
		case EN_CHANGE:
			break;
		case EN_UPDATE: {
			if (editControlsTree != NULL) {
				CEditControl editControl = (*editControlsTree.get())->getActiveNode().getEditControl();
				editControl.deleteWhiteSpaces();
				(*editControlsTree.get())->setControlWidth(editControl.GetWidth());
			}
			SendMessage(hWndMainWindow, WM_SIZE, 0, 0);
			break;
		}
		case EN_SETFOCUS:
			clickEditControl();
			break;
		default:
			break;
		}
	}
}

void CMatheditorWindow::newFile() {

}

void CMatheditorWindow::saveFile() {

}

void CMatheditorWindow::loadFile() {

}

void CMatheditorWindow::clickEditControl() {
	HWND handle = ::GetFocus();
	if (editControlsTree.get())
	{
		(*editControlsTree.get())->changeActiveNode(handle);
	}
	InvalidateRect(hWndMainWindow, NULL, FALSE);
}

void CMatheditorWindow::createEditControl(NodeType nodeType) 
{
	(*editControlsTree.get())->createChildrens(nodeType);
	SendMessage(hWndMainWindow, WM_SIZE, 0, 0);
}

void CMatheditorWindow::deleteEditControl()
{
	if (activeEditControl != editControls.end()) {
		HWND hwnd = activeEditControl->GetHandle();
		//checkHandle(hwnd);
		DestroyWindow(hwnd);
		auto oldCell = activeEditControl;
		auto nextCell = activeEditControl;
		++nextCell;
		activeEditControl = (nextCell != editControls.end()
			? nextCell
			: (activeEditControl != editControls.begin()
				? --activeEditControl
				: editControls.end()));
		editControls.erase(oldCell);
		editControlsHandles.erase(editControlsHandles.find(hwnd));
		SendMessage(hWndMainWindow, WM_SIZE, 0, 0);
		if (activeEditControl != editControls.end()) {
			SetFocus(activeEditControl->GetHandle());
		}
	}
	else {
		MessageBox(hWndMainWindow, L"Выберите ячейку!", L"Не выбрана ячейка", MB_OK | MB_ICONWARNING);
	}
	InvalidateRect(hWndMainWindow, NULL, FALSE);
}

HACCEL CMatheditorWindow::GetHaccel() const
{
	return haccel;
}

HWND CMatheditorWindow::GetHandle() const
{
	return hWndMainWindow;
}

void CMatheditorWindow::loadFromLSVModule()
{
	std::shared_ptr<TreeVisualisation*> editControlsTreeFromLSVTree = std::shared_ptr<TreeVisualisation*>(new TreeVisualisation(hWndMainWindow));

	// копия кода из main.cpp из папки LSV модуля

	CMathMLParser parser;
	std::shared_ptr<IExpression> operationTree = parser.parseFromFile("format_files/expr.mathml");
	recursiveTransformation(*editControlsTreeFromLSVTree.get(), operationTree);

	editControlsTree = editControlsTreeFromLSVTree;
}

void CMatheditorWindow::recursiveTransformation(TreeVisualisation* tree, std::shared_ptr<IExpression> expr)
{
	if( typeid(expr.get()) == typeid(COpExp) ) {
		COpExp* operExpr = reinterpret_cast<COpExp*>(expr.get());
		NodeType operation;
		switch( operExpr->getOperation() )
		{
			case LSVUtils::PLUS:
			{
				operation = NodeType::Plus;
				break;
			}
			case LSVUtils::MINUS:
			{
				operation = NodeType::Minus;
				break;
			}
			case LSVUtils::MULTIPLY:
			{
				operation = NodeType::Multiply;
				break;
			}
			case LSVUtils::DIVIDE:
			case LSVUtils::FRAC:
			{
				operation = NodeType::Divide;
				break;
			}
			case LSVUtils::POWER:
			{
				operation = NodeType::Power;
				break;
			}
			case LSVUtils::SUM:
			{
				// Особый случай!!!
				operation = NodeType::Summ;
				break;
			}
			default:
			{

			}
		}

		tree->createChildrens(operation);
		std::shared_ptr<IExpression> leftChild = (operExpr->getFirstOperand());
		// уже
		// tree->changeActiveNode(/*handle левого ребенка);
		recursiveTransformation(tree, leftChild);
		std::shared_ptr<IExpression> rightChild = (operExpr->getSecondOperand());
		tree->changeActiveNode(tree->getActiveNode().getParentNode()->getRightNode()->getHandle());
		recursiveTransformation(tree, rightChild);
	}
	if( typeid(expr.get()) == typeid(CIdExp) ) {
		CIdExp* operExpr = reinterpret_cast<CIdExp*>(expr.get());

		std::string text = std::to_string(operExpr->getValue());

		::SetWindowText(tree->getActiveNode().getHandle(), (LPWSTR)text.c_str());
	}
	if (typeid(expr.get()) == typeid(CNumExp) ) {
		CNumExp* operExpr = reinterpret_cast<CNumExp*>(expr.get());

		std::string text = std::to_string(operExpr->getValue());

		::SetWindowText(tree->getActiveNode().getHandle(), (LPWSTR)text.c_str());
	}
	if( typeid(expr.get()) == typeid(CSumExp) ) {
		CSumExp* operExpr = reinterpret_cast<CSumExp*>(expr.get());
		// Пока не до этого
	}
}
