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

#include <stdlib.h>
#include <windows.h>
#include <wchar.h>

#include "config.h"
#include "debug.h"
#include "process.h"
#include "string.h"
#include "window.h"

static void error(const wchar_t * message)
{
    MessageBoxW(NULL, message, L"Error", MB_OK | MB_ICONERROR);
    exit(EXIT_FAILURE);
}

static void usage(const wchar_t * program)
{
    wprintf_s(L"Usage: %ls [executable]\n", program);
    exit(EXIT_FAILURE);
}

int wmain(int argc, wchar_t * argv[])
{
    if (argc > 2)
    {
        usage(argv[0]);
    }

    LPCWSTR name = (argc > 1) ? argv[1] : WIDEN(CFG_DEFAULT_EXECUTABLE_NAME);

    DEBUG(L"Executable: %ls\n", name);

    DWORD pid = FindFirstProcessByExecutableName(name);
    if (pid == INVALID_PROCESS_VALUE)
    {
        error(L"An executable with the specified name could not be found.");
    }

    DEBUG(L"Process ID: %lu\n", pid);

    HWND handle = FindMainWindowByProcessId(pid);
    if (handle == NULL)
    {
        error(L"An executable with the specified name has no visible windows.");
    }

    DEBUG(L"Handle:     0x%p\n", handle);

    if (!CenterWindowOnPrimaryDisplay(handle))
    {
        error(L"Could not center the window on the screen - access denied.");
    }

    return EXIT_SUCCESS;
}
