#pragma once
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
	static const COLORREF colorRed = RGB(255, 10, 10);
	static const COLORREF colorBlack = RGB(0, 0, 0);
	static const COLORREF colorSilver = RGB(192, 192, 192);
	static const COLORREF colorGray = RGB(140, 140, 140);
	enum PointType {MAXIMUM, MINIMUM, NONE};
	//размеры рисуемого диапазона
	Range xRange;
	Range yRange;
	//отрисовка прямой
	void drawLine( HDC targetDC, int x1, int y1, int x2, int y2,
		COLORREF color, int fnPenStyle, int nWidth );
	//отрисовка стартового поля с системой координат
	void drawCoordSystem( HDC targetDC );
	//главная процедура отрисовки функции
	void drawFunction(HDC targetDC/*здесь будет указатель на структуру с функцией*/);
	//пример простой функции
	double simpleFunc( double x );
	//Проверят текущую точку на экстремум, возвращает
	// -1 - точка локального минимума
	// 0 - не экстремум
	//1 - точка локального максимума
	PointType checkOnExtremum(int pixelX);
	bool drawExtremumPoints(HDC targetDC, std::vector<POINT> points);
	
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	
};