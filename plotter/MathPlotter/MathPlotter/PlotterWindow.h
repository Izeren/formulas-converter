﻿#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <utility>

//пока что это пробная версия без настроек взаимодействия между редактором и плоттером

struct Range
{
	double begin, end;
	Range() :
		begin( 0 ),
		end( 0 )
	{}
	Range( double start, double finish ) :
		begin( start ),
		end( finish )
	{}
};

class CPlotterWindow {
public:
	CPlotterWindow();
	~CPlotterWindow();
	static bool RegisterClass();
	bool Create();
	void Show(int cmdShow);
protected:
	void OnDestroy();
	void OnNCCreate(HWND handle);
	void OnCreate();
	void OnSize();
	void OnPaint();

private:
	HWND handle; // Хэндл окна
	//размеры окошка
	static const int windowWidth = 600;
	static const int windowHeight = 600;
	//ширина линии
	static const int lineWidth;
	//базовые цвета
	static const COLORREF colorBlue = RGB(23,189,255);
	static const COLORREF colorBlack = RGB(0, 0, 0);
	//размеры рисуемого диапазона
	Range xRange;
	Range yRange;
	//отрисовка прямой
	void drawLine( HDC targetDC, int x1, int y1, int x2, int y2, COLORREF color );
	//отрисовка стартового поля с системой координат
	void drawCoordSystem( HDC targetDC );
	//главная процедура отрисовки функции
	void drawFunction(HDC targetDC,/*здесь будет указатель на структуру с функцией*/ Range xRange, Range yRange );
	//пример простой функции
	double simpleFunc( double x );
	
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	
};