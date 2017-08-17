/**
 * Win32Api.cpp
 *
 * Class implementing Win32 API GUI on application.
 */

#include "Win32Api.h"
#include "FmDashboardManager.h"

// Predefine static members
HINSTANCE Win32Api::m_hInstance = GetModuleHandle(0);
TCHAR Win32Api::m_trayIconTip[64] = APP_NAME_W;
UINT Win32Api::WM_TASKBAR = 0;

NOTIFYICONDATA Win32Api::m_notifyIconData = { 0 };
HMENU Win32Api::m_hTrayMenu = 0;
HWND Win32Api::m_hMainWindow = 0;
WNDCLASSEXW Win32Api::m_mainWindowClass = { 0 };

Win32Api::Win32Api() {}

Win32Api::~Win32Api() {}

LRESULT CALLBACK Win32Api::MainWindowProcedure(HWND hWindow, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_ACTIVATE:
        Shell_NotifyIcon(NIM_ADD, &m_notifyIconData);
        break;
    case WM_CREATE:

        ShowWindow(hWindow, SW_HIDE);
        m_hTrayMenu = CreatePopupMenu();
        AppendMenu(m_hTrayMenu, MF_STRING, ID_TRAY_SETTINGS,  L"Settings");
        AppendMenu(m_hTrayMenu, MF_SEPARATOR, 0,0);
        AppendMenu(m_hTrayMenu, MF_STRING, ID_TRAY_EXIT,  L"Exit");

        break;

    case WM_SYSCOMMAND:
        /*In WM_SYSCOMMAND messages, the four low-order bits of the wParam parameter
		are used internally by the system. To obtain the correct result when testing the value of wParam,
		an application must combine the value 0xFFF0 with the wParam value by using the bitwise AND operator.*/

		switch( wp & 0xFFF0 )
        {
        case SC_MINIMIZE:
        case SC_CLOSE:
            minimize() ;
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
            restore();
        } else if (lp == WM_RBUTTONDOWN) {
            // Get current mouse position.
            POINT curPoint ;
            GetCursorPos( &curPoint ) ;
			SetForegroundWindow(m_hMainWindow);

            // TrackPopupMenu blocks the app until TrackPopupMenu returns

            UINT clicked = TrackPopupMenu(Win32Api::m_hTrayMenu,TPM_RETURNCMD | TPM_NONOTIFY,curPoint.x + 100,curPoint.y - 100,0,hWindow,NULL);

            SendMessage(hWindow, WM_NULL, 0, 0); // send benign message to window to make sure the menu goes away.
            if (clicked == ID_TRAY_EXIT)
            {
                // quit the application.
                Shell_NotifyIcon(NIM_DELETE, &m_notifyIconData);
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
        minimize() ;
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

 void Win32Api::initNotifyIconData() {

    memset(&Win32Api::m_notifyIconData, 0, sizeof( NOTIFYICONDATA ) ) ;
    Win32Api::m_notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
    Win32Api::m_notifyIconData.hWnd = m_hMainWindow;
    Win32Api::m_notifyIconData.uID = ID_TRAY_APP_ICON;
    Win32Api::m_notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    Win32Api::m_notifyIconData.uCallbackMessage = WM_SYSICON; //Set up our invented Windows Message
    Win32Api::m_notifyIconData.hIcon = (HICON)LoadIcon( GetModuleHandle(NULL),      MAKEINTRESOURCE(ICO1) ) ;
    wcscpy(Win32Api::m_notifyIconData.szTip, m_trayIconTip);

    cout << "initNotifyIconData(): Notification icon data initialized." << endl;
    return;
}

void Win32Api::minimize(void) {
    // hide the main window
    ShowWindow(m_hMainWindow, SW_HIDE);
}

void Win32Api::restore(void) {
    ShowWindow(m_hMainWindow, SW_SHOW);
    return;
}

HWND Win32Api::initMainWindow(void) {

    // Fill in window class structure
    m_mainWindowClass.cbSize = sizeof(m_mainWindowClass);                       // size of structure
    m_mainWindowClass.style = CS_HREDRAW | CS_VREDRAW;            // redraw if size changes
    m_mainWindowClass.lpfnWndProc = &MainWindowProcedure;             // points to window procedure
    m_mainWindowClass.cbClsExtra = 0;                             // no extra class memory
    m_mainWindowClass.cbWndExtra = 0;                             // no extra window memory
    m_mainWindowClass.hInstance = m_hInstance;                      // handle to instance
    m_mainWindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);    // predefined app. icon
    m_mainWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);      // predefined arrow
    m_mainWindowClass.hbrBackground = HBRUSH(COLOR_WINDOW + 1);   // white background brush
    m_mainWindowClass.lpszMenuName =  L"MainMenu";                // name of menu resource
    m_mainWindowClass.lpszClassName = L"MainWindowClass";         // name of window class
    m_mainWindowClass.hIconSm = (HICON)LoadImage(                 // small class icon
        m_hInstance,
        MAKEINTRESOURCE(5),
        IMAGE_ICON,
        GetSystemMetrics(SM_CXSMICON),
        GetSystemMetrics(SM_CYSMICON),
        LR_DEFAULTCOLOR);


    // Register the window class
    if (!RegisterClassExW(&m_mainWindowClass)) {
        cout << "Register MainWindowClass class failed!" << endl;
        return 0;
    } else {
        m_hMainWindow = CreateWindowExW(
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
            m_hInstance,
            0
        );
        if (!m_hMainWindow) {
            cout << "Creating main window failed!" << endl;
            return 0;
        }
        cout << "initMainWindow(): Main window initialized" << endl;
        return m_hMainWindow;
    }
}

void Win32Api::showMainWindow(void) {


    ShowWindow(m_hMainWindow, SW_SHOWDEFAULT);
    return;
}

bool Win32Api::initGui(void) {
    HWND result =  Win32Api::initMainWindow();
    Win32Api::initNotifyIconData();
    Win32Api::showMainWindow();
    if (result) {
        return 1;
    } else {
        return 0;
    }
}
