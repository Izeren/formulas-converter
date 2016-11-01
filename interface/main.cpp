#include <Windows.h>

#include "cmatheditor_window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow) {
    UNREFERENCED_PARAMETER( hInstance );
    if (!CMatheditorWindow::RegisterClassW()) {
        return -1;
    }

    CMatheditorWindow notepad_window;
    if (!notepad_window.Create()) {
        return -1;
    }
    notepad_window.Show(nCmdShow);
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
