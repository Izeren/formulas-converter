#include <Windows.h>

#include "MainWindow.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow) {
    if( !MainWindow::RegisterClass() ) {
        return -1;
    }

	MainWindow notepad_window;
    if( !notepad_window.Create() ) {
        return -1;
    }
	
    notepad_window.Show(nCmdShow);
    
    MSG msg;
    while( GetMessage(&msg, NULL, 0, 0) > 0 ) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
