#pragma comment(lib, "comctl32.lib")

#include "MainWindow.h"

const LPCWSTR classNameMainWindow = L"MainWindow";
const COLORREF colorActiveCell = RGB(200, 200, 200);

MainWindow::MainWindow() : handleMainWindow(0)
{
}

MainWindow::~MainWindow()
{
	if( handleMainWindow ) {
		DestroyWindow(handleMainWindow);
	}
}

bool MainWindow::RegisterClass()
{
	WNDCLASSEX windowClass = { };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = MainWindow::windowProc;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.lpszClassName = classNameMainWindow;
	windowClass.hCursor = ::LoadCursor(GetModuleHandle(0), IDC_ARROW);
	windowClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);

	return (RegisterClassEx(&windowClass) != 0);
}

bool MainWindow::Create()
{
	handleMainWindow = CreateWindowEx(0, classNameMainWindow,
		L"Matheditor",
		WS_EX_OVERLAPPEDWINDOW | WS_SIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		this);

	if( handleMainWindow == NULL ) {
		return false;
	}

	treeOfEdits.Create(handleMainWindow);

	createToolbar();

	return (handleMainWindow != 0);
}

void MainWindow::Show(int cmdShow)
{
	ShowWindow(handleMainWindow, cmdShow);
	treeOfEdits.showTree(cmdShow);
}

void MainWindow::OnNCCreate(HWND handle)
{
	handleMainWindow = handle;
}

void MainWindow::OnCreate()
{
	//createCell();
}

void MainWindow::OnPaint()
{
	InvalidateRect(handleMainWindow, NULL, FALSE);
	PAINTSTRUCT paintStruct;
	::BeginPaint(handleMainWindow, &paintStruct);

	HBRUSH brush;
	brush = CreateSolidBrush(colorActiveCell);
	::FillRect(paintStruct.hdc, &paintStruct.rcPaint, brush);
	::EndPaint(handleMainWindow, &paintStruct);

	RECT rect;
	::GetClientRect(handleMainWindow, &rect);

	RECT toolbarRect;
	::GetClientRect(handleToolbar, &toolbarRect);

/* //Рисование Cell'ов
	int currentTop = rect.top + (toolbarRect.bottom - toolbarRect.top) + sizeBetweenCells;
	for( auto window = childrensWindow.begin(); window != childrensWindow.end(); ++window ) {
		LONG leftBorder = rect.left + marginLeftRightCells;
		LONG width = rect.right - rect.left - 2 * marginLeftRightCells;

		::SetWindowPos(window->getHandle(), HWND_TOP, leftBorder, currentTop, width, window->getHeight(), 0);
		currentTop += sizeBetweenCells + window->getHeight();

		if( window->isResult() ) {
			leftBorder = rect.left + 2 * marginLeftRightCells;
			width = rect.right - rect.left - 4 * marginLeftRightCells;

			::SetWindowPos(window->getHandleOfResult(), HWND_TOP, leftBorder, currentTop, width, window->getHeightOfResult(), 0);
			currentTop += sizeBetweenCells + window->getHeightOfResult();
		}
	}
*/


	// SetScrollRange(handleMainWindow, SB_VERT, 0, 100, TRUE);

	//Подсвечивает activeCell - нужно для того, чтобы проверять, что activeCell на нужном сell'е
	//PAINTSTRUCT paintStruct2;
	//BeginPaint(activeCell->getHandle(), &paintStruct2);
	//brush = CreateSolidBrush(RGB(0, 250, 0));
	//FillRect(paintStruct2.hdc, &paintStruct2.rcPaint, brush);
	//EndPaint(activeCell->getHandle(), &paintStruct2);
}

void MainWindow::OnSize()
{
	SendMessage(handleToolbar, TB_AUTOSIZE, 0, 0);
}

void MainWindow::OnDestroy()
{
	PostQuitMessage(0);
}

void MainWindow::OnCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( HIWORD(wParam) == 0 ) {
		switch( LOWORD(wParam) ) {
			case ID_FILE_NEW:
			{
				//newFile();
				break;
			}
			case ID_FILE_EXIT:
			{
				SendMessage(hWnd, WM_CLOSE, wParam, lParam);
				break;
			}
			case ID_FILE_SAVE:
			{
				//saveFile();
				break;
			}
			case ID_FILE_OPEN:
			{
				//loadFile();
				break;
			}
			case ID_OPERATOR_EMPTY: // Теперь он ненужен
			{
				//createEditControl();
				break;
			}
			case ID_OPERATOR_EQUAL:
			{
				//createEditControl(L"=");
				break;
			}
			case ID_OPERATOR_PLUS:
			{
				//createEditControl(L"+");
				break;
			}
			case ID_OPERATOR_MINUS:
			{
				//createEditControl(L"-");
				break;
			}
			case ID_OPERATOR_MULTIPLY:
			{
				//createEditControl(L"*");
				break;
			}
			case ID_OPERATOR_DIVIDE:
			{
				break;
			}
			case ID_OPERATOR_POWER:
			{
				break;
			}
			case ID_OPERATOR_SUMM:
			{
				break;
			}
			case ID_OPERATOR_DELETE:
			{
				//deleteEditControl();
				break;
			}
			default:
			{
				break;
			}
		}
	} else {
		switch( HIWORD(wParam) )
		{
			case EN_CHANGE:
			{
				break;
			}
			case EN_UPDATE:
			{
				break;
			}
			case EN_SETFOCUS:
			{
				//clickEditControl();
				break;
			}
			default:
			{
				break;
			}
		}
	}
}

LRESULT MainWindow::OnCtlColorEdit(WPARAM wParam, LPARAM lParam)
{
	HDC hdc = reinterpret_cast<HDC>(wParam);
	HBRUSH hbr = 0;
	if( lParam == reinterpret_cast<long>(GetFocus()) ) {
		hbr = CreateSolidBrush(RGB(255, 255, 0));
		SetBkColor(hdc, RGB(255, 255, 0));
	}
	return reinterpret_cast<LRESULT>(hbr);
}

bool MainWindow::OnClose()
{
	switch( MessageBox(handleMainWindow, L"Вы действительно хотите выйти?", L"Завершение работы", MB_YESNO | MB_ICONWARNING) ) {
		case IDNO:
		{
			return false;
		}
		case IDYES:
		{
			return true;
		}
	}
	return true;
}

LRESULT MainWindow::windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( message == WM_NCCREATE ) {
		MainWindow* window = reinterpret_cast<MainWindow*> ((reinterpret_cast<CREATESTRUCT*>(lParam))->lpCreateParams);
		SetLastError(0);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG>(window));
		if( GetLastError() ) {
			return GetLastError();
		}
		window->OnNCCreate(hwnd);
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	MainWindow* window = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	switch( message ) {
		case WM_CREATE:
		{
			window->OnCreate();
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		case WM_PAINT:
		{
			window->OnPaint();
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		case WM_SIZE:
		{
			window->OnSize();
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		case WM_CLOSE:
		{
			if (window->OnClose()) {
				::PostQuitMessage(0);
				return DefWindowProc(hwnd, message, wParam, lParam);
			}
			else {
				return 0;
			}
		}
		case WM_COMMAND:
		{
			window->OnCommand(hwnd, message, wParam, lParam);
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		case WM_DESTROY:
		{
			window->OnDestroy();
			return 0;
		}
		case WM_CTLCOLOREDIT:
		{
			return window->OnCtlColorEdit(wParam, lParam);
		}
		case WM_VSCROLL:
		{
			//MessageBox(window->handleMainWindow, L"sdfsdf", L"sss", MB_OK);
		}
		default:
		{
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
	}
}

void MainWindow::createToolbar() {

	// Declare and initialize local constants.
	const int bitmapSize = 16;

	HINSTANCE hInstance = ::GetModuleHandle(0);

	handleToolbar = CreateToolbarEx(handleMainWindow, WS_CHILD | WS_VISIBLE | CCS_TOP, 1,
		0, HINST_COMMCTRL, IDB_STD_SMALL_COLOR, NULL, 0, 0, 0, 0, 0, sizeof(TBBUTTON));

	//Enable multiple image lists
	SendMessage(handleToolbar, CCM_SETVERSION, (WPARAM)5, 0);

	//Add icons from default imagelist
	TBBUTTON tbb_buildin[] = {
		{ STD_FILENEW, ID_FILE_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ STD_FILEOPEN, ID_FILE_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ STD_FILESAVE, ID_FILE_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
	};
	SendMessage(handleToolbar, (UINT)TB_ADDBUTTONS, _countof(tbb_buildin), (LPARAM)&tbb_buildin);

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
	SendMessage(handleToolbar, TB_SETIMAGELIST, (WPARAM)1, (LPARAM)hImageList);

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

	SendMessage(handleToolbar, (UINT)TB_ADDBUTTONS, _countof(tbb), (LPARAM)&tbb);

	SendMessage(handleToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(handleToolbar, TRUE);
}

HBITMAP MainWindow::loadTransparentBitmap(HINSTANCE hInstance, int resource)
{	
	HBITMAP hbmSrc = LoadBitmap(hInstance, MAKEINTRESOURCE(resource));

	HDC hdcSrc, hdcDst;
	BITMAP bm;
	COLORREF clrTP, clrBK;
	HBITMAP hbmOld, hbmNew = NULL;

	if( (hdcSrc = CreateCompatibleDC(NULL)) != NULL ) {
		if( (hdcDst = CreateCompatibleDC(NULL)) != NULL ) {
			int nRow, nCol;
			GetObject(hbmSrc, sizeof(bm), &bm);
			hbmOld = reinterpret_cast<HBITMAP>(SelectObject(hdcSrc, hbmSrc));
			hbmNew = CreateBitmap(bm.bmWidth, bm.bmHeight, bm.bmPlanes, bm.bmBitsPixel, NULL);
			SelectObject(hdcDst, hbmNew);

			BitBlt(hdcDst, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, SRCCOPY);

			clrTP = GetPixel(hdcDst, 1, 1);// Get color of first pixel at 0,0
			clrBK = GetSysColor(COLOR_MENU);// Get the current background color of the menu

			for( nRow = 0; nRow < bm.bmHeight; nRow++ ) {// work our way through all the pixels changing their color
				for( nCol = 0; nCol < bm.bmWidth; nCol++ ) {// when we hit our set transparency color.
					if( GetPixel(hdcDst, nCol, nRow) == clrTP ) {
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