#include <Windows.h>

#include "cmatheditor_window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow) {
    if( !CMatheditorWindow::RegisterClassW() ) {
        return -1;
    }

    CMatheditorWindow notepad_window;
    if( !notepad_window.Create() ) {
        return -1;
    }
    notepad_window.Show(nCmdShow);
    
    MSG msg;

    while( GetMessage(&msg, NULL, 0, 0)) {
		if (!TranslateAccelerator(notepad_window.GetHandle(), // handle to receiving window
			notepad_window.GetHaccel(), // handle to active accelerator table
			&msg)) {
			::TranslateMessage(&msg);
			if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP) {
				SendMessage(notepad_window.GetHandle(), msg.message, msg.wParam, msg.lParam);
			}
			::DispatchMessage(&msg);
		}
    }
    return msg.wParam;
}
