#include <Windows.h>
#include <iostream>
#include <utility>
#include <algorithm>
#include <string>

#include "PlotterWindow.h"

const int CPlotterWindow::lineWidth = 3;

CPlotterWindow::CPlotterWindow()
{
	handle = 0;
}

CPlotterWindow::~CPlotterWindow() {}

bool CPlotterWindow::RegisterClass()
{
	WNDCLASSEX windowClass;
	::ZeroMemory( &windowClass, sizeof(windowClass) );
	windowClass.cbSize = sizeof( WNDCLASSEX );
	windowClass.lpfnWndProc = CPlotterWindow::windowProc;
	windowClass.hInstance = GetModuleHandle( 0 );
	windowClass.lpszClassName = L"PlotterWindow";
	windowClass.hbrBackground = ( HBRUSH )GetStockObject( GRAY_BRUSH );
	return (::RegisterClassEx( &windowClass ) != 0);
}

bool CPlotterWindow::Create()
{
	CreateWindowEx(
		0,
		L"PlotterWindow",
		L"Plotter",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowWidth,
		windowHeight,
		0,//парент данного окна. впилить как будет вызов из редактора
		0,
		GetModuleHandle(0),
		this);
	return (handle != 0);
}

void CPlotterWindow::Show(int cmdShow)
{
	ShowWindow(handle, cmdShow);
}

void CPlotterWindow::OnDestroy()
{
	PostQuitMessage(0);
}

void CPlotterWindow::OnNCCreate(HWND _handle) {
	handle = _handle;
}

void CPlotterWindow::OnCreate()
{}

double CPlotterWindow::simpleFunc( double x )
{
	return x*x;
}

void CPlotterWindow::drawFunction( HDC targetDC, Range xRange, Range yRange )
{
	RECT clientRect;
	::GetClientRect( handle, &clientRect );
	int currentWidth = clientRect.right - clientRect.left;
	int currentHeight = clientRect.bottom - clientRect.top;
	POINT* points = new POINT[ currentWidth ];
	//сколько пикселей в 1 единице координат
	double step = currentWidth / ( xRange.end - xRange.begin );
	for( int x = 0; x <= currentWidth; x++) {
		double coord = x / step - currentWidth / step / 2;
		double value =( -simpleFunc( coord ) + currentHeight / 2 );
		points[ x ] = { x, ( int )value };
	}
	HPEN pen = CreatePen( PS_SOLID, lineWidth, colorBlue );
	SelectObject( targetDC, pen );
	Polyline( targetDC, points, currentWidth );
	DeleteObject( pen );

}

void CPlotterWindow::drawLine( HDC targetDC, int x1, int y1, int x2, int y2, COLORREF color = colorBlack )
{
	HPEN pen = CreatePen( PS_SOLID, lineWidth, colorBlack );
	SelectObject( targetDC, pen );
	POINT* points = new POINT[ 2 ];
	points[ 0 ] = { x1, y1 };
	points[ 1 ] = { x2, y2 };
	Polyline( targetDC, points, 2 );
	DeleteObject( pen );
}

void CPlotterWindow::drawCoordSystem( HDC targetDC )
{
	RECT clientRect;
	::GetClientRect(handle, &clientRect);
	int currentWidth = clientRect.right - clientRect.left;
	int currentHeight = clientRect.bottom - clientRect.top;
	drawLine( targetDC, currentWidth / 2, 0, currentWidth / 2, currentHeight );
	drawLine( targetDC, 0, currentHeight / 2, currentWidth, currentHeight / 2 );
	

}

void CPlotterWindow::OnSize()
{
	RECT rect;
	::GetClientRect( handle, &rect );
	InvalidateRect( handle, &rect, FALSE );
}



void CPlotterWindow::OnPaint()
{
	RECT clientRect;
	PAINTSTRUCT paintStruct;
	HDC windowDC = ::BeginPaint(handle, &paintStruct);
	::GetClientRect(handle, &clientRect);
	HDC displayBufferDC = CreateCompatibleDC(windowDC);
	HBITMAP displayBuffer = CreateCompatibleBitmap(windowDC, clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top);
	HGDIOBJ oldDisplayBuffer = SelectObject(displayBufferDC, displayBuffer);

	FillRect( displayBufferDC, &clientRect, ( HBRUSH )GetStockObject( WHITE_BRUSH ) );
		
//рисовать здесь
	drawCoordSystem( displayBufferDC );
	drawFunction( displayBufferDC, Range( -20.0, 20.0 ), Range( -20.0, 20.0 ) );

	BitBlt(windowDC, clientRect.left, clientRect.top, clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top, displayBufferDC, 0, 0, SRCCOPY);
	SelectObject(displayBufferDC, oldDisplayBuffer);
	DeleteObject(displayBuffer);
	DeleteDC(displayBufferDC);
	::EndPaint(handle, &paintStruct);
}

// Оконная процедура. Обработка сообщений, приходящий в окно.
LRESULT CPlotterWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{	
	if (message == WM_NCCREATE) {
		CPlotterWindow* window = (CPlotterWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetLastError(0);
		SetWindowLongPtr(handle, GWLP_USERDATA, (LONG)window);
		if (GetLastError() != 0) {
			return GetLastError();
		}
		window->OnNCCreate(handle);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	else {
		CPlotterWindow* window = (CPlotterWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		switch (message) {
		case WM_CREATE:
		{
			window->OnCreate();
			return DefWindowProc(handle, message, wParam, lParam);
		}
		case WM_DESTROY:
		{
			window->OnDestroy();
			return 0;
		}
		case WM_PAINT:
		{
			window->OnPaint();
			return 0;
		}
		case WM_SIZE:
		{
			window->OnSize();
			return 0;
		}
		default:
			return DefWindowProc(handle, message, wParam, lParam);
		}
	}
}