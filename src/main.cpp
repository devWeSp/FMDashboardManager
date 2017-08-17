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

#include "config.h"
#include "resource.h"

#include <iostream>
#include <windows.h>

#include "FmDashboardManager.h"
#include "DataManager.h"
#include "Win32Api.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "STARTING APPLICATION" << endl;

    Win32Api::initGui();

    // Run main loop
	MSG msg;
	while (GetMessageW(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
    return 0;
}

