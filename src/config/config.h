/**
 * config.h
 *
 * Global configuration file for application.
 */

#ifndef CONFIG_H_
#define CONFIG_H_

// Compile flags
//#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE

#include <windows.h>
#include <shellapi.h>

// Define strings
#define APP_NAME "FM Dashboard Manager"
#define APP_NAME_W L"FM Dashboard Manager"


// Define control IDs
#define ID_TRAY_APP_ICON    1001
#define ID_TRAY_EXIT        1002
#define ID_TRAY_SETTINGS    1003


 #endif // CONFIG_H_

