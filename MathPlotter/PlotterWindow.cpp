#include <Windows.h>
#include <iostream>
#include <utility>
#include <algorithm>
#include <string>
#include <vector>
#include <math.h>

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
	return sin(x);
}

bool CPlotterWindow::drawExtremumPoints(HDC targetDC, std::vector<POINT> points)
{
	HPEN pen = CreatePen(PS_SOLID, 2, colorRed);
	SelectObject(targetDC, pen);
    for( int i = 0; i < static_cast<int>(points.size()); i++ ) {
        Ellipse( targetDC, points[i].x - 5, points[i].y - 5, points[i].x + 5, points[i].y + 5 );
    }
	DeleteObject(pen);
	return true;
}

CPlotterWindow::PointType CPlotterWindow::checkOnExtremum(int pixelX)
{
	RECT clientRect;
	::GetClientRect(handle, &clientRect);
	int currentWidth = clientRect.right - clientRect.left;
    // TODO: use currentHeight
	//int currentHeight = clientRect.bottom - clientRect.top;
	double stepX = (xRange.end - xRange.begin) / currentWidth;
    // TODO: use stepY
    //double stepY = (yRange.end - yRange.begin) / currentHeight;

	//Предыдущая точка
	double prevX = xRange.begin + stepX * (pixelX - 1);
	double prevY = simpleFunc(prevX);

	//Текущая точка
	double currX = xRange.begin + stepX * pixelX;
	double currY = simpleFunc(currX);

	//Следующая точка точка
	double nextX = xRange.begin + stepX * (pixelX + 1);
	double nextY = simpleFunc(nextX);

	if (prevY > currY &&  currY < nextY)
		return MINIMUM;
	if (prevY < currY &&  currY > nextY)
		return MAXIMUM;
	return NONE;
}

void CPlotterWindow::drawFunction( HDC targetDC )
{
	RECT clientRect;
	::GetClientRect( handle, &clientRect );
	int currentWidth = clientRect.right - clientRect.left;
	int currentHeight = clientRect.bottom - clientRect.top;
	std::vector<POINT> points;
	double stepX = (xRange.end - xRange.begin) / currentWidth;
	double stepY = (yRange.end - yRange.begin) / currentHeight;
	std::vector <POINT> maximums;
	std::vector <POINT> minimums;
	for( int pixelX = 0; pixelX <= currentWidth; pixelX++) {
		double coordX = xRange.begin + stepX * pixelX;
		double coordY = -simpleFunc(coordX);
		double pixelY = coordY / stepY + currentHeight / 2;

		switch (checkOnExtremum(pixelX)) {
		case MAXIMUM:
			maximums.push_back({ pixelX, (int)pixelY });
			break;
		case MINIMUM:
			minimums.push_back({ pixelX, (int)pixelY });
			break;
		default:
			break;
		};
		
		if (pixelY >= 0 && pixelY <= currentWidth - 1)
			points.push_back({ pixelX, (int)pixelY });
	}

	for (int i = 0; i < static_cast<int>(points.size()) - 1; i++)
		drawLine(targetDC, points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, colorBlue, PS_SOLID, lineWidth);

	drawExtremumPoints(targetDC, maximums);
	drawExtremumPoints(targetDC, minimums);
}

void CPlotterWindow::drawLine( HDC targetDC, int x1, int y1, int x2, int y2,
	COLORREF color = colorBlack, int fnPenStyle = PS_SOLID, int nWidth = lineWidth )
{
	HPEN pen = CreatePen( fnPenStyle, nWidth, color );
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
	//сколько пикселей в 1 единице координат
	//по x
	double stepX = currentWidth / (xRange.end - xRange.begin);
    //по y	
	double stepY = currentHeight / (yRange.end - yRange.begin);


	//рисуем сетку, подписываем числа
	for ( int i = 0; i * stepY < currentHeight; ++i ) {
		int coord = static_cast<int>(std::round(i * stepY));
		if( i % 5 != 0 ){
			drawLine( targetDC, 0, coord, currentWidth, coord, colorSilver, PS_DASH, 1 );
		}
		else {
			drawLine( targetDC, 0, coord, currentWidth, coord, colorGray, PS_DASH, 2 );
			//TODO 4 надо заменить на длину числа
			TextOut( targetDC, currentWidth / 2 + 1, coord, std::to_wstring( yRange.end - i).c_str(), 4);
		}
	}

	for ( int i = 0; i * stepX < currentWidth; ++i ) {
		int coord = static_cast<int>(std::round(i * stepX));
		if ( i % 5 != 0 ) {
			drawLine( targetDC, coord, 0, coord, currentHeight, colorSilver, PS_DASH, 1 );
		}
		else {
			if( yRange.begin + i == 0 ){
				continue;
			}
			drawLine(targetDC, coord, 0, coord, currentHeight, colorSilver, PS_DASH, 2);
			TextOut(targetDC, coord, currentHeight / 2 + 1, std::to_wstring(xRange.begin + i).c_str(), 4);
		}
	}

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
	xRange = Range(-20, 20);
	yRange = Range(-2, 2);

	drawCoordSystem( displayBufferDC );
	drawFunction( displayBufferDC);

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