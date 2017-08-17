/**
 * Win32Api.h
 *
 * Class implementing Win32 API GUI on application.
 */

#ifndef WIN32_API_H_
#define WIN32_API_H_

#include "config.h"
#include <iostream>
#include <windows.h>
#include "resource.h"

using namespace std;

class Win32Api {
private:
    static WNDCLASSEXW m_mainWindowClass;
    static HINSTANCE m_hInstance;
    static HWND m_hMainWindow;
    static HMENU m_hTrayMenu;
    static NOTIFYICONDATA m_notifyIconData;

    static TCHAR m_trayIconTip[];
    static UINT WM_TASKBAR;

private:
    static LRESULT CALLBACK MainWindowProcedure(HWND hWindow, UINT msg, WPARAM wp, LPARAM lp);
    static void initNotifyIconData(void);
    static void minimize(void);
    static void restore(void);
    static HWND initMainWindow(void);
    static void showMainWindow(void);

public:
    Win32Api();
    ~Win32Api();
    static bool initGui(void);

 };

#endif // WIN32_API_H_
