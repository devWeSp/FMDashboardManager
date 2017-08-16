/**
 * FM Dashboard Manager
 *
 * AUTHOR:
 *      Frank Mejzlik
 *      http://www.frankmejzlik.com/
 *
 * LICENCE:
 *      MIT License
 *
 * DESCRIPTIOIN:
 *      Application to efficiently manage multiple dashboards. You can have
 *      up to 10 extra dashboards available on keyboard shortcuts you choose.
 *
 *      Or maybe you want to setup your own snippets to those dashboards to make
 *      your work faster. Just simply paste your multiple snippets in no time.
 */

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500
#define _UNICODE
#define UNICODE

#include <iostream>
#include <windows.h>
#include "resource.h"

//#include <tchar.h>
//#include <shellapi.h>

#define APP_NAME "FM Dashboard Manager"
#define APP_NAME_W L"FM Dashboard Manager"

using namespace std;


// Declare main window class structure
WNDCLASSEXW mainWindowClass;
// Get this application instance handle
HINSTANCE hInstance = GetModuleHandle(0);
HWND hMainWindow;

UINT WM_TASKBAR = 0;
NOTIFYICONDATA notifyIconData;
TCHAR szTIP[64] = APP_NAME_W;
HMENU hMenu;

LRESULT CALLBACK MainWindowProcedure(HWND hWindow, UINT msg, WPARAM wp, LPARAM lp);
void initNotifyIconData();
void minimize(HWND hMainWindow);
void restore(HWND hMainWindow);
HWND initMainWindow();

LRESULT CALLBACK MainWindowProcedure(HWND hWindow, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_ACTIVATE:
        Shell_NotifyIcon(NIM_ADD, &notifyIconData);
        break;
    case WM_CREATE:

        ShowWindow(hWindow, SW_HIDE);
        hMenu = CreatePopupMenu();
        AppendMenu(hMenu, MF_STRING, ID_TRAY_SETTINGS,  L"Settings");
        AppendMenu(hMenu, MF_SEPARATOR, 0,0);
        AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT,  L"Exit");

        break;

    case WM_SYSCOMMAND:
        /*In WM_SYSCOMMAND messages, the four low-order bits of the wParam parameter
		are used internally by the system. To obtain the correct result when testing the value of wParam,
		an application must combine the value 0xFFF0 with the wParam value by using the bitwise AND operator.*/

		switch( wp & 0xFFF0 )
        {
        case SC_MINIMIZE:
        case SC_CLOSE:
            minimize(hWindow) ;
            return 0 ;
            break;
        }
        break;


        // Our user defined WM_SYSICON message.
    case WM_SYSICON:
    {
        switch(wp) {
        case ID_TRAY_APP_ICON:
            SetForegroundWindow(hWindow);

            break;
        }

        if (lp == WM_LBUTTONUP) {
            restore(hWindow);
        } else if (lp == WM_RBUTTONDOWN) {
            // Get current mouse position.
            POINT curPoint ;
            GetCursorPos( &curPoint ) ;
			SetForegroundWindow(hWindow);

            // TrackPopupMenu blocks the app until TrackPopupMenu returns

            UINT clicked = TrackPopupMenu(hMenu,TPM_RETURNCMD | TPM_NONOTIFY,curPoint.x + 100,curPoint.y - 100,0,hWindow,NULL);

            SendMessage(hWindow, WM_NULL, 0, 0); // send benign message to window to make sure the menu goes away.
            if (clicked == ID_TRAY_EXIT)
            {
                // quit the application.
                Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
                PostQuitMessage(0) ;
            }
        }
    }
    break;

    // intercept the hittest message..
    case WM_NCHITTEST:
    {
       UINT uHitTest = DefWindowProc(hWindow, WM_NCHITTEST, wp, lp);
        if(uHitTest == HTCLIENT)
            return HTCAPTION;
        else
            return uHitTest;
    }

    case WM_CLOSE:
        minimize(hWindow) ;
        return 0;
        break;

    case WM_DESTROY:
        PostQuitMessage (0);
        break;

    default:
        return DefWindowProcW(hWindow, msg, wp, lp);
        break;
    }
    return 0;
}

HWND initMainWindow() {

    // Fill in window class structure
    mainWindowClass.cbSize = sizeof(mainWindowClass);                       // size of structure
    mainWindowClass.style = CS_HREDRAW | CS_VREDRAW;            // redraw if size changes
    mainWindowClass.lpfnWndProc = &MainWindowProcedure;             // points to window procedure
    mainWindowClass.cbClsExtra = 0;                             // no extra class memory
    mainWindowClass.cbWndExtra = 0;                             // no extra window memory
    mainWindowClass.hInstance = hInstance;                      // handle to instance
    mainWindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);    // predefined app. icon
    mainWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);      // predefined arrow
    mainWindowClass.hbrBackground = HBRUSH(COLOR_WINDOW + 1);   // white background brush
    mainWindowClass.lpszMenuName =  L"MainMenu";                // name of menu resource
    mainWindowClass.lpszClassName = L"MainWindowClass";         // name of window class
    mainWindowClass.hIconSm = (HICON)LoadImage(                 // small class icon
        hInstance,
        MAKEINTRESOURCE(5),
        IMAGE_ICON,
        GetSystemMetrics(SM_CXSMICON),
        GetSystemMetrics(SM_CYSMICON),
        LR_DEFAULTCOLOR);


    // Register the window class
    if (!RegisterClassExW(&mainWindowClass)) {
        cout << "Register MainWindowClass class failed!" << endl;
        return 0;
    } else {
        HWND hMainWindow = CreateWindowExW(
            0,
            L"MainWindowClass",
            APP_NAME_W,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            hInstance,
            0
        );
        if (!hMainWindow) {
            cout << "Creating main window failed!" << endl;
            return 0;
        }
        return hMainWindow;
    }
}


int main(int argc, char* argv[]) {
    cout << "STARTING APPLICATION" << endl;

    // Construct, register main window class
    hMainWindow = initMainWindow();
    if (!hMainWindow) {
        cout << "Initialization of main window failed" << endl;
    }
    initNotifyIconData();

    // Show main window
    ShowWindow(hMainWindow, SW_SHOWDEFAULT);
    // Run main loop
	MSG msg;
	while (GetMessageW(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
    return 0;
}

void minimize(HWND hMainWindow) {
    // hide the main window
    ShowWindow(hMainWindow, SW_HIDE);
}


void restore(HWND hMainWindow){
    ShowWindow(hMainWindow, SW_SHOW);
}

void initNotifyIconData() {

    memset(&notifyIconData, 0, sizeof( NOTIFYICONDATA ) ) ;
    notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
    notifyIconData.hWnd = hMainWindow;
    notifyIconData.uID = ID_TRAY_APP_ICON;
    notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    notifyIconData.uCallbackMessage = WM_SYSICON; //Set up our invented Windows Message
    notifyIconData.hIcon = (HICON)LoadIcon( GetModuleHandle(NULL),      MAKEINTRESOURCE(ICO1) ) ;
    wcscpy(notifyIconData.szTip, szTIP);
    return;
}
