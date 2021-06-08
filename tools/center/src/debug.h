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

#pragma once

#include <stdlib.h>
#include <wchar.h>

#define ERRORF(...)                      \
    do                                   \
    {                                    \
        fwprintf_s(stderr, __VA_ARGS__); \
        fwprintf_s(stderr, L"\n");       \
    } while (0)

#define SAYF(...)                        \
    do                                   \
    {                                    \
        wprintf_s(__VA_ARGS__);          \
        wprintf_s(L"\n");                \
    } while (0)

#define FATAL(...)                       \
    do                                   \
    {                                    \
        ERRORF(L"Error: " __VA_ARGS__);  \
        exit(EXIT_FAILURE);              \
    } while (0)

#ifdef DEBUG_BUILD
# define DEBUG(...) ERRORF(__VA_ARGS__)
#else
# define DEBUG(...) do {} while (0)
#endif
