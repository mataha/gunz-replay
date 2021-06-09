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

#include <stdbool.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <wchar.h>

#include <windows.h>

#include "config.h"
#include "debug.h"
#include "process.h"
#include "string.h"
#include "window.h"

#define OPTION_MARKER ((const wchar_t) L'-')

static noreturn void error(const wchar_t * message)
{
    MessageBoxW(NULL, message, L"Error", MB_OK | MB_ICONERROR);
    exit(EXIT_FAILURE);
}

static noreturn void usage(const wchar_t * program)
{
    ERRORF(L"Usage: %ls [-h] [-w] [executable]", program);
    exit(EXIT_FAILURE);
}

int wmain(int argc, wchar_t * argv[])
{
    bool w = false;

    enum { ARGUMENT_MODE = 0, OPTION_MODE } mode = OPTION_MODE;
    int position = 0;

    for (int index = 1; index < argc; ++index)
    {
        if (mode == OPTION_MODE && argv[index][0] == OPTION_MARKER)
        {
            const wchar_t * option = &argv[index][1];
            if (option != NULL && wcslen(option) == 0)
            {
                FATAL(L"Reading from stdin is not supported.");
            }
            else if (wcseq(option, L"-"))
            {
                mode = ARGUMENT_MODE;
            }
            else if (wcseq(option, L"w"))
            {
                if (w)
                {
                    FATAL(L"Multiple '-w' options are not supported.");
                }

                w = true;
            }
            else
            {
                usage(argv[0]);
            }
        }
        else
        {
            if (position)
            {
                FATAL(L"Too many arguments supplied.");
            }
            else
            {
                position = index;
            }
        }
    }

    LPCWSTR name = position ? argv[position] : widen(EXECUTABLE_NAME);

    DEBUG(L"Executable: %ls", name);

    DWORD pid = FindFirstProcessByExecutableName(name);
    if (pid == INVALID_PROCESS_VALUE)
    {
        error(L"An executable with the specified name could not be found.");
    }

    DEBUG(L"Process ID: %lu", pid);

    HWND handle = FindMainWindowByProcessId(pid);
    if (handle == NULL)
    {
        error(L"An executable with the specified name has no visible windows.");
    }

    DEBUG(L"Handle:     0x%p", (void *) handle);

    if (!CenterWindowOnCurrentDisplay(handle, w || WORK_AREA_CENTERING))
    {
        error(L"Could not center the window on the screen - access denied.");
    }

    return EXIT_SUCCESS;
}
