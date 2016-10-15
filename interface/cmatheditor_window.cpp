#pragma comment(lib, "comctl32.lib")
#include "cmatheditor_window.h"
#include <Windows.h>
#include <commctrl.h>

const LPCWSTR CMatheditorWindow::class_name_ = L"MatheditorWindow";
const int ToolbarSize = 42;

CMatheditorWindow::CMatheditorWindow() : hWndMainWindow(0) {
	editControl = CEditControl();
}

CMatheditorWindow::~CMatheditorWindow() {
	if (hWndMainWindow) {
		DestroyWindow(hWndMainWindow);
	}
}

bool CMatheditorWindow::RegisterClassW() {
	WNDCLASSEX wnd_class = { };
	wnd_class.cbSize = sizeof(WNDCLASSEX);
	wnd_class.style = CS_HREDRAW | CS_VREDRAW;
	wnd_class.lpfnWndProc = windowProc;
	wnd_class.hInstance = GetModuleHandle(NULL);
	wnd_class.lpszClassName = class_name_;
	wnd_class.hCursor = ::LoadCursor(GetModuleHandle(0), IDC_ARROW);
	return RegisterClassEx(&wnd_class);
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

	return true;
}

#define COLORREF2RGB(Color) (Color & 0xff00) | ((Color >> 16) & 0xff) \
                                 | ((Color << 16) & 0xff0000)

//HBITMAP ReplaceColor(HBITMAP hBmp, COLORREF cOldColor, COLORREF cNewColor, HDC hBmpDC)
//{
//	HBITMAP RetBmp = NULL;
//	if (hBmp)
//	{
//		HDC BufferDC = CreateCompatibleDC(NULL);    // DC for Source Bitmap
//		if (BufferDC)
//		{
//			HBITMAP hTmpBitmap = (HBITMAP)NULL;
//			if (hBmpDC)
//				if (hBmp == (HBITMAP)GetCurrentObject(hBmpDC, OBJ_BITMAP))
//				{
//					hTmpBitmap = CreateBitmap(1, 1, 1, 1, NULL);
//					SelectObject(hBmpDC, hTmpBitmap);
//				}
//
//			HGDIOBJ PreviousBufferObject = SelectObject(BufferDC, hBmp);
//			// here BufferDC contains the bitmap
//
//			HDC DirectDC = CreateCompatibleDC(NULL); // DC for working
//			if (DirectDC)
//			{
//				// Get bitmap size
//				BITMAP bm;
//				GetObject(hBmp, sizeof(bm), &bm);
//
//				// create a BITMAPINFO with minimal initilisation 
//				// for the CreateDIBSection
//				BITMAPINFO RGB32BitsBITMAPINFO;
//				ZeroMemory(&RGB32BitsBITMAPINFO, sizeof(BITMAPINFO));
//				RGB32BitsBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//				RGB32BitsBITMAPINFO.bmiHeader.biWidth = bm.bmWidth;
//				RGB32BitsBITMAPINFO.bmiHeader.biHeight = bm.bmHeight;
//				RGB32BitsBITMAPINFO.bmiHeader.biPlanes = 1;
//				RGB32BitsBITMAPINFO.bmiHeader.biBitCount = 32;
//
//				// pointer used for direct Bitmap pixels access
//				UINT * ptPixels;
//
//				HBITMAP DirectBitmap = CreateDIBSection(DirectDC,
//					(BITMAPINFO *)&RGB32BitsBITMAPINFO,
//					DIB_RGB_COLORS,
//					(void **)&ptPixels,
//					NULL, 0);
//				if (DirectBitmap)
//				{
//					// here DirectBitmap!=NULL so ptPixels!=NULL no need to test
//					HGDIOBJ PreviousObject = SelectObject(DirectDC, DirectBitmap);
//					BitBlt(DirectDC, 0, 0,
//						bm.bmWidth, bm.bmHeight,
//						BufferDC, 0, 0, SRCCOPY);
//
//					// here the DirectDC contains the bitmap
//
//					// Convert COLORREF to RGB (Invert RED and BLUE)
//					cOldColor = COLORREF2RGB(cOldColor);
//					cNewColor = COLORREF2RGB(cNewColor);
//
//					// After all the inits we can do the job : Replace Color
//					for (int i = ((bm.bmWidth*bm.bmHeight) - 1); i >= 0; i--)
//					{
//						if (ptPixels[i] == cOldColor) ptPixels[i] = cNewColor;
//					}
//					// little clean up
//					// Don't delete the result of SelectObject because it's 
//					// our modified bitmap (DirectBitmap)
//					SelectObject(DirectDC, PreviousObject);
//
//					// finish
//					RetBmp = DirectBitmap;
//				}
//				// clean up
//				DeleteDC(DirectDC);
//			}
//			if (hTmpBitmap)
//			{
//				SelectObject(hBmpDC, hBmp);
//				DeleteObject(hTmpBitmap);
//			}
//			SelectObject(BufferDC, PreviousBufferObject);
//			// BufferDC is now useless
//			DeleteDC(BufferDC);
//		}
//	}
//	return RetBmp;
//}
//
//HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
//{
//	HDC hdcMem, hdcMem2;
//	HBITMAP hbmMask;
//	BITMAP bm;
//
//	// Create monochrome (1 bit) mask bitmap.  
//
//	GetObject(hbmColour, sizeof(BITMAP), &bm);
//	hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
//
//	// Get some HDCs that are compatible with the display driver
//
//	hdcMem = CreateCompatibleDC(0);
//	hdcMem2 = CreateCompatibleDC(0);
//
//	::SelectObject(hdcMem, hbmColour);
//	::SelectObject(hdcMem2, hbmMask);
//
//	// Set the background colour of the colour image to the colour
//	// you want to be transparent.
//	SetBkColor(hdcMem, crTransparent);
//
//	// Copy the bits from the colour image to the B+W mask... everything
//	// with the background colour ends up white while everythig else ends up
//	// black...Just what we wanted.
//
//	BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
//
//	// Take our new mask and use it to turn the transparent colour in our
//	// original colour image to black so the transparency effect will
//	// work right.
//	BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);
//
//	HDC hdc;
//	hdc = CreateCompatibleDC(0);
//	::SelectObject(hdc, hbmMask);
//
//	SelectObject(hdcMem, hbmMask);
//	BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCAND);
//
//	SelectObject(hdcMem, hbmMask);
//	BitBlt(hdc, 0, bm.bmHeight, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCPAINT);
//
//	// Clean up.
//
//	DeleteDC(hdcMem);
//	DeleteDC(hdcMem2);
//
//	return hbmMask;
//}

HBITMAP MakeBitMapTransparent(HBITMAP hbmSrc)
{
		HDC hdcSrc, hdcDst;
		BITMAP bm;
		COLORREF clrTP, clrBK;
		HBITMAP hbmOld,
			hbmNew = NULL;

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

						for (nRow = 0; nRow < bm.bmHeight; nRow++)// work our way through all the pixels changing their color
							for (nCol = 0; nCol < bm.bmWidth; nCol++)// when we hit our set transparency color.
								if (GetPixel(hdcDst, nCol, nRow) == clrTP)
									SetPixel(hdcDst, nCol, nRow, clrBK);

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
	SendMessage(hWndToolbar, TB_SETIMAGELIST, (WPARAM)1, (LPARAM)hImageList);

	TBBUTTON tbb[] =
	{
		{ MAKELONG(0, 1), ID_OPERATOR_EQUAL, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(1, 1), ID_OPERATOR_PLUS, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(2, 1), ID_OPERATOR_MINUS, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(3, 1), ID_OPERATOR_MULTIPLY, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(4, 1), ID_OPERATOR_DIVIDE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(5, 1), ID_OPERATOR_POWER, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
		{ MAKELONG(6, 1), ID_OPERATOR_SUMM, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, 0, 0 },
	};

	SendMessage(hWndToolbar, (UINT)TB_ADDBUTTONS, _countof(tbb), (LPARAM)&tbb);

	SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
	ShowWindow(hWndToolbar, TRUE);
}

void CMatheditorWindow::Show(int cmdShow) {
	ShowWindow(hWndMainWindow, cmdShow);
	editControl.Show(cmdShow);
	UpdateWindow(hWndMainWindow);
}

void CMatheditorWindow::OnCreate() {
	editControl.Create(hWndMainWindow);
}

void CMatheditorWindow::OnNCCreate(HWND hWnd) {
	hWndMainWindow = hWnd;
}

void CMatheditorWindow::OnSize()
{
	RECT rect;
	::GetClientRect(hWndMainWindow, &rect);
	SetWindowPos(editControl.GetHandle(), HWND_TOP, rect.left, rect.top + ToolbarSize, rect.right - rect.left, rect.bottom - rect.top, 0);
	SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
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
	case WM_NOTIFY:
		switch (((NMHDR *)lParam)->code) {
		case NM_CLICK:
			NMLINK* pNMLink = (NMLINK*)lParam;
			LITEM iItem = pNMLink->item;
			// Custom OutputDebugString
			if (wParam)
			{
				int c;
			}
			break;
		}
		break;
	case WM_CREATE:
		OnCreate();
		return DefWindowProc(hwnd, message, wParam, lParam);
	case WM_SIZE:
		OnSize();
		return DefWindowProc(hwnd, message, wParam, lParam);
	case WM_COMMAND:
		OnCommand(hwnd, message, wParam, lParam);
		return DefWindowProc(hwnd, message, wParam, lParam);
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
		case ID_OPERATOR_EQUAL:
			break;
		case ID_OPERATOR_PLUS:
			break;
		case ID_OPERATOR_MINUS:
			break;
		case ID_OPERATOR_MULTIPLY:
			break;
		case ID_OPERATOR_DIVIDE:
			break;
		case ID_OPERATOR_POWER:
			break;
		case ID_OPERATOR_SUMM:
			break;
		default:
			break;
		}
	}
	else {
		switch (HIWORD(wParam)) {
		case EN_CHANGE:
			break;
		case EN_UPDATE:
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



