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
    : xRange( -5, 5 ), yRange( -5, 5 )
{
    CTexParser parser;
    treeRoot = parser.parseFromFile( "D:\\My Docs\\Prog\\WinProg\\4c1t_MathRedactor\\LSV_module\\format_files\\test2.tex" );
    handle = 0;
}

CPlotterWindow::~CPlotterWindow() {}

bool CPlotterWindow::RegisterClass()
{
    WNDCLASSEX windowClass;
    ZeroMemory( &windowClass, sizeof( windowClass ) );
    windowClass.cbSize = sizeof( WNDCLASSEX );
    windowClass.lpfnWndProc = CPlotterWindow::windowProc;
    windowClass.hInstance = GetModuleHandle( 0 );
    windowClass.lpszClassName = L"PlotterWindow";
    windowClass.hbrBackground = (HBRUSH)GetStockObject( GRAY_BRUSH );
    return (RegisterClassEx( &windowClass ) != 0);
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
        GetModuleHandle( 0 ),
        this );
    return (handle != 0);
}

void CPlotterWindow::Show( int cmdShow )
{
    ShowWindow( handle, cmdShow );
}

void CPlotterWindow::OnDestroy()
{
    PostQuitMessage( 0 );
}

void CPlotterWindow::OnNCCreate( HWND _handle )
{
    handle = _handle;
}

void CPlotterWindow::OnCreate()
{}

std::pair<bool, double> CPlotterWindow::simpleFunc( double x )
{
    std::unordered_map<std::string, double> idExpValues( { { "x", x } } );
    CEvalVisitor visitor( idExpValues );
    treeRoot->Accept( visitor );

    bool success = !visitor.isEvalFailed();
    double result = 0.0;
    if( success ) {
        result = visitor.getValue();
    }
    return std::make_pair( success, result );
}

std::pair<bool, double> CPlotterWindow::simpleFunc3( double x )
{
    std::shared_ptr<CIdExp> xExp = std::make_shared<CIdExp>(CIdExp("x"));
    std::shared_ptr<CNumExp> sqrExp = std::make_shared<CNumExp>( CNumExp( 2 ) );
    std::shared_ptr<COpExp> powerExp = std::make_shared<COpExp>( COpExp( xExp, sqrExp, LSVUtils::TOperation::POWER ) );
    std::shared_ptr<CNumExp> addExp = std::make_shared<CNumExp>( CNumExp( 1 ) );
    std::shared_ptr<COpExp> root = std::make_shared<COpExp>( COpExp( powerExp, addExp, LSVUtils::TOperation::MINUS ) );

    std::unordered_map<std::string, double> idExpValues( { { "x", x } } );
    CEvalVisitor visitor(idExpValues);
    root->Accept( visitor );

    bool success = !visitor.isEvalFailed();
    double result = 0.0;
    if( success ) {
        result = visitor.getValue();
    }
    return std::make_pair( success, result );
}

std::pair<bool, double> CPlotterWindow::simpleFunc2( double x )
{
    std::shared_ptr<CIdExp> xExp = std::make_shared<CIdExp>( CIdExp( "x" ) );
    std::shared_ptr<CIdExp> kExp = std::make_shared<CIdExp>( CIdExp( "k" ) );

    std::shared_ptr<CNumExp> oneExp = std::make_shared<CNumExp>( CNumExp( 1.0 ) );
    std::shared_ptr<CNumExp> twoExp = std::make_shared<CNumExp>( CNumExp( 2.0 ) );
    std::shared_ptr<CNumExp> threeExp = std::make_shared<CNumExp>( CNumExp( 3.0 ) );

    std::shared_ptr<COpExp> xSqrExp = std::make_shared<COpExp>( COpExp( xExp, twoExp, LSVUtils::TOperation::POWER ) );
    std::shared_ptr<COpExp> xCubExp = std::make_shared<COpExp>( COpExp( xExp, threeExp, LSVUtils::TOperation::POWER ) );

    std::shared_ptr<COpExp> xMultExp = std::make_shared<COpExp>( COpExp( twoExp, xSqrExp, LSVUtils::TOperation::MULTIPLY ) );

    std::shared_ptr<COpExp> twoPowExp = std::make_shared<COpExp>( COpExp( twoExp, kExp, LSVUtils::TOperation::POWER ) );
    std::shared_ptr<COpExp> twoFracExp = std::make_shared<COpExp>( COpExp( oneExp, twoPowExp, LSVUtils::TOperation::FRAC ) );
    std::shared_ptr<CSumExp> sumExp = std::make_shared<CSumExp>( CSumExp( "k", 1, 5, twoFracExp ) );

    std::shared_ptr<COpExp> subtrExp = std::make_shared<COpExp>( COpExp( xCubExp, xMultExp, LSVUtils::TOperation::MINUS ) );
    std::shared_ptr<COpExp> root = std::make_shared<COpExp>( COpExp( subtrExp, sumExp, LSVUtils::TOperation::MULTIPLY ) );

    std::unordered_map<std::string, double> idExpValues( { { "x", x } } );
    CEvalVisitor visitor( idExpValues );
    root->Accept( visitor );

    bool success = !visitor.isEvalFailed();
    double result = 0.0;
    if( success ) {
        result = visitor.getValue();
    }
    return std::make_pair(success, result);
}

bool CPlotterWindow::drawExtremumPoints( HDC targetDC, std::vector<POINT> points )
{
    HPEN pen = CreatePen( PS_SOLID, 2, colorRed );
    SelectObject( targetDC, pen );
    for( int i = 0; i < static_cast<int>(points.size()); i++ ) {
        Ellipse( targetDC, points[i].x - 5, points[i].y - 5, points[i].x + 5, points[i].y + 5 );
    }
    DeleteObject( pen );
    return true;
}

CPlotterWindow::PointType CPlotterWindow::checkOnExtremum( int pixelX )
{
    RECT clientRect;
    GetClientRect( handle, &clientRect );
    int currentWidth = clientRect.right - clientRect.left;
    // TODO: применить currentHeight
    //int currentHeight = clientRect.bottom - clientRect.top;
    double stepX = (xRange.end - xRange.begin) / currentWidth;
    // TODO: применить stepY
    //double stepY = (yRange.end - yRange.begin) / currentHeight;

    //Предыдущая точка
    double prevX = xRange.begin + stepX * (pixelX - 1);
    std::pair<bool, double> res = simpleFunc( prevX );
    if( !res.first ) {
        return NONE;
    }
    double prevY = res.second;

    //Текущая точка
    double currX = xRange.begin + stepX * pixelX;
    res = simpleFunc( currX );
    if( !res.first ) {
        return NONE;
    }
    double currY = res.second;

    //Следующая точка точка
    double nextX = xRange.begin + stepX * (pixelX + 1);
    res = simpleFunc( nextX );
    if( !res.first ) {
        return NONE;
    }
    double nextY = res.second;

    if( prevY > currY &&  currY < nextY )
        return MINIMUM;
    if( prevY < currY &&  currY > nextY )
        return MAXIMUM;
    return NONE;
}

void CPlotterWindow::drawFunction( HDC targetDC )
{
    RECT clientRect;
    GetClientRect( handle, &clientRect );
    int currentWidth = clientRect.right - clientRect.left;
    int currentHeight = clientRect.bottom - clientRect.top;
    std::vector<POINT> points;
    double stepX = (xRange.end - xRange.begin) / currentWidth;
    double stepY = (yRange.end - yRange.begin) / currentHeight;
    std::vector <POINT> maximums;
    std::vector <POINT> minimums;
    for( int pixelX = 0; pixelX <= currentWidth; pixelX++ ) {
        double coordX = xRange.begin + stepX * pixelX;
        std::pair<bool, double> res = simpleFunc( coordX );
        if( !res.first ) {
            continue;
        }
        double coordY = -res.second;
        double pixelY = coordY / stepY + currentHeight / 2;

        switch( checkOnExtremum( pixelX ) ) {
        case MAXIMUM:
            maximums.push_back( { pixelX, (int)pixelY } );
            break;
        case MINIMUM:
            minimums.push_back( { pixelX, (int)pixelY } );
            break;
        default:
            break;
        };

        if( pixelY >= 0 && pixelY <= currentWidth - 1 )
            points.push_back( { pixelX, (int)pixelY } );
    }

    for( int i = 0; i < static_cast<int>(points.size()) - 1; i++ )
        drawLine( targetDC, points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, colorBlue, PS_SOLID, lineWidth );

    drawExtremumPoints( targetDC, maximums );
    drawExtremumPoints( targetDC, minimums );
}

void CPlotterWindow::drawLine( HDC targetDC, int x1, int y1, int x2, int y2,
    COLORREF color = colorBlack, int fnPenStyle = PS_SOLID, int nWidth = lineWidth )
{
    HPEN pen = CreatePen( fnPenStyle, nWidth, color );
    SelectObject( targetDC, pen );
    POINT* points = new POINT[2];
    points[0] = { x1, y1 };
    points[1] = { x2, y2 };
    Polyline( targetDC, points, 2 );
    DeleteObject( pen );
}


void CPlotterWindow::drawCoordSystem( HDC targetDC )
{
    RECT clientRect;
    GetClientRect( handle, &clientRect );
    int currentWidth = clientRect.right - clientRect.left;
    int currentHeight = clientRect.bottom - clientRect.top;
    //сколько пикселей в 1 единице координат
    //по x
    double stepX = currentWidth / (xRange.end - xRange.begin);
    //по y	
    double stepY = currentHeight / (yRange.end - yRange.begin);


    //рисуем сетку, подписываем числа

    //горизонтальные прямые
    for( int i = 0; i * stepY < currentHeight; ++i ) {
        int coord = static_cast<int>(std::round( i * stepY ));
        if( i % 5 != 0 ) {
            drawLine( targetDC, 0, coord, currentWidth, coord, colorSilver, PS_DASH, 1 );
        } else {
            drawLine( targetDC, 0, coord, currentWidth, coord, colorGray, PS_DASH, 2 );
            std::wstring numberOnGrid = std::to_wstring( yRange.end - i );
            std::wstring numberOnGridRounded = std::to_wstring( static_cast<int>(std::round( yRange.end - i )) );
            TextOut( targetDC, currentWidth / 2 + 1, coord, numberOnGrid.c_str(), numberOnGridRounded.size() + 3 );
        }
    }

    //вертикальные прямые
    for( int i = 0; i * stepX < currentWidth; ++i ) {
        int coord = static_cast<int>(std::round( i * stepX ));
        if( i % 5 != 0 ) {
            drawLine( targetDC, coord, 0, coord, currentHeight, colorSilver, PS_DASH, 1 );
        } else {
            if( yRange.begin + i == 0 ) {
                continue;
            }
            drawLine( targetDC, coord, 0, coord, currentHeight, colorSilver, PS_DASH, 2 );
            std::wstring numberOnGrid = std::to_wstring( xRange.begin + i );
            std::wstring numberOnGridRounded = std::to_wstring( static_cast<int>(std::round( xRange.begin + i )) );
            TextOut( targetDC, coord, currentHeight / 2 + 1, numberOnGrid.c_str(), numberOnGridRounded.size() + 3 );
        }
    }

    drawLine( targetDC, currentWidth / 2, 0, currentWidth / 2, currentHeight );
    drawLine( targetDC, 0, currentHeight / 2, currentWidth, currentHeight / 2 );
}

void CPlotterWindow::OnSize()
{
    RECT rect;
    GetClientRect( handle, &rect );
    InvalidateRect( handle, &rect, FALSE );
}



void CPlotterWindow::OnPaint()
{
    RECT clientRect;
    PAINTSTRUCT paintStruct;
    HDC windowDC = BeginPaint( handle, &paintStruct );
    GetClientRect( handle, &clientRect );
    HDC displayBufferDC = CreateCompatibleDC( windowDC );
    HBITMAP displayBuffer = CreateCompatibleBitmap( windowDC, clientRect.right - clientRect.left,
        clientRect.bottom - clientRect.top );
    HGDIOBJ oldDisplayBuffer = SelectObject( displayBufferDC, displayBuffer );

    FillRect( displayBufferDC, &clientRect, (HBRUSH)GetStockObject( WHITE_BRUSH ) );

    drawCoordSystem( displayBufferDC );
    drawFunction( displayBufferDC );

    BitBlt( windowDC, clientRect.left, clientRect.top, clientRect.right - clientRect.left,
        clientRect.bottom - clientRect.top, displayBufferDC, 0, 0, SRCCOPY );
    SelectObject( displayBufferDC, oldDisplayBuffer );
    DeleteObject( displayBuffer );
    DeleteDC( displayBufferDC );
    EndPaint( handle, &paintStruct );
}

// Оконная процедура. Обработка сообщений, приходящий в окно.
LRESULT CPlotterWindow::windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam )
{
    if( message == WM_NCCREATE ) {
        CPlotterWindow* window = (CPlotterWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
        SetLastError( 0 );
        SetWindowLongPtr( handle, GWLP_USERDATA, (LONG)window );
        if( GetLastError() != 0 ) {
            return GetLastError();
        }
        window->OnNCCreate( handle );
        return DefWindowProc( handle, message, wParam, lParam );
    } else {
        CPlotterWindow* window = (CPlotterWindow*)GetWindowLongPtr( handle, GWLP_USERDATA );
        switch( message ) {
        case WM_CREATE:
        {
            window->OnCreate();
            return DefWindowProc( handle, message, wParam, lParam );
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
            return DefWindowProc( handle, message, wParam, lParam );
        }
    }
}
