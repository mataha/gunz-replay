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

BOOL CenterWindowOnPrimaryDisplay(HWND hWnd)
{
    WINDOWINFO info;
    info.cbSize = sizeof(WINDOWINFO);
    if (!GetWindowInfo(hWnd, &info))
    {
        return FALSE;
    }

    LONG lWidth = info.rcWindow.right - info.rcWindow.left;
    LONG lHeight = info.rcWindow.bottom - info.rcWindow.top;

    // I'm too lazy to do this properly, assume the window is borderless for now
    int x = (GetSystemMetrics(SM_CXSCREEN) - (int) lWidth) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - (int) lHeight) / 2;
    
    return SetWindowPos(
        hWnd,
        HWND_TOPMOST,
        x, y,
        0, 0,
        SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW
    );
}

typedef struct {
    DWORD dwProcessId;
    HWND hWnd;
} ENUMWINDOWSCALLBACKDATA;

static BOOL CALLBACK EnumWindowsCallback(HWND hWnd, LPARAM lParam)
{
    ENUMWINDOWSCALLBACKDATA * data = (ENUMWINDOWSCALLBACKDATA *) lParam;
    DWORD dwProcessId = 0;

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
