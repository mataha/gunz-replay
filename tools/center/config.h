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

/*
 * Default name of the executable to center its main window on the primary
 * display. Note that this is case-sensitive, as some servers can be easily
 * distinguished by their binaries' filenames - for reference, most servers
 * have 'Gunz.exe' as their client's executable name, but this program targets
 * neo-International GunZ, which uses 'GunZ.exe' instead.
 *
 * Adjustable via the first parameter.
 */
#ifndef DEFAULT_EXECUTABLE
# define DEFAULT_EXECUTABLE            "GunZ.exe"
#endif
