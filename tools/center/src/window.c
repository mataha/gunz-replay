// Copyright (c) 2021 mataha
// 
// Permission to use, copy, modify, and distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
// 
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include <windows.h>

#include "window.h"

static inline LONG GetRectangleWidth(RECT rect)
{
    return rect.right - rect.left;
}

static inline LONG GetRectangleHeight(RECT rect)
{
    return rect.bottom - rect.top;
}

BOOL CenterWindowOnCurrentDisplay(HWND hWnd, BOOL bWorkArea)
{
    // https://docs.microsoft.com/pl-pl/windows/win32/gdi/positioning-objects-on-a-multiple-display-setup
    HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    MONITORINFO monitor;
    monitor.cbSize = sizeof(monitor);
    if (!GetMonitorInfoW(hMonitor, &monitor))
    {
        return FALSE;
    }

    RECT rcDisplay = bWorkArea ? monitor.rcWork : monitor.rcMonitor;
    LONG lDisplayWidth  = GetRectangleWidth(rcDisplay);
    LONG lDisplayHeight = GetRectangleHeight(rcDisplay);

    WINDOWINFO window;
    window.cbSize = sizeof(window);
    if (!GetWindowInfo(hWnd, &window))
    {
        return FALSE;
    }

    RECT rcWindow = window.rcWindow;
    LONG lWindowWidth  = GetRectangleWidth(rcWindow);
    LONG lWindowHeight = GetRectangleHeight(rcWindow);

    // I'm too lazy to do this properly, assume the window is borderless for now
    int x = rcDisplay.left + (lDisplayWidth  - lWindowWidth)  / 2;
    int y = rcDisplay.top  + (lDisplayHeight - lWindowHeight) / 2;
    
    return SetWindowPos(
        hWnd,
        HWND_TOP,
        x, y,
        0, 0,
        SWP_NOREPOSITION | SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW
    );
}

typedef struct
{
    DWORD dwProcessId;
    HWND hWnd;
} ENUMWINDOWSCALLBACKDATA;

static BOOL CALLBACK EnumWindowsCallback(HWND hWnd, LPARAM lParam)
{
    ENUMWINDOWSCALLBACKDATA * data = (ENUMWINDOWSCALLBACKDATA *) lParam;
    DWORD dwProcessId = 0; // kernel thread container doesn't have any windows

    GetWindowThreadProcessId(hWnd, &dwProcessId);
    if (data->dwProcessId != dwProcessId || !IsMainWindow(hWnd))
    {
        return TRUE;
    }

    data->hWnd = hWnd;
    return FALSE;
}

HWND FindMainWindowByProcessId(DWORD dwProcessId)
{
    ENUMWINDOWSCALLBACKDATA data;
    data.dwProcessId = dwProcessId;
    data.hWnd = NULL;

    EnumWindows(EnumWindowsCallback, (LPARAM) &data);
    return data.hWnd;
}

inline BOOL IsMainWindow(HWND hWnd)
{   
    return GetWindow(hWnd, GW_OWNER) == NULL && IsWindowVisible(hWnd);
}
