#!/usr/bin/env python3

# Copyright (c) 2021 mataha
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

import argparse
import pathlib
import sys
import zlib

EXTENSION = ".gzf"


class SingleLineArgumentParser(argparse.ArgumentParser):
    def __init__(self, *args, **kwargs):
        kwargs["fromfile_prefix_chars"] = "@"
        super().__init__(*args, **kwargs)

    def convert_arg_line_to_args(self, arg_line):
        return arg_line.split()


class ReplayException(Exception):
    pass


class OverwriteException(Exception):
    pass


def unpack(input, output):
    stream = input.read_bytes()
    data = zlib.decompress(stream)
    output.write_bytes(data)

    return len(data)


def process(filename, overwrite=True):
    path = pathlib.Path(filename)

    if not path.is_file():
        raise ValueError(f"path doesn't point to an operable file: '{path}'")

    file = path.resolve()

    if not file.suffix == ".gzr":
        raise ValueError(f"file is not a valid GunZ replay file: '{file}'")

    if not file.stat().st_size:
        raise ReplayException(f"corrupted or incomplete replay file: '{file}'")

    output = pathlib.Path(file.stem + EXTENSION)
    if not overwrite and output.exists():
        raise OverwriteException(f"output file already exists: '{output}'")

    length = unpack(file, output)

    if length:
        print(f"{length:>10n} {output!s}")


def error(exception, quit=False):
    import traceback

    program = pathlib.Path(sys.argv[0]).name
    error = exception.__str__()
    cause = traceback.extract_stack(None, 2)[1][2]  # name of this function

    if error:
        message = f"{error!s}"
    else:
        message = type(exception).__name__

    print(f"{program}: {cause}: {message}", file=sys.stderr)

    if quit:
        sys.exit(True)


def main(argv=None):
    if argv is None:
        argv = sys.argv[1:]

    parser = SingleLineArgumentParser(description="a simple replay unpacker")

    parser.add_argument(
        "--no-overwrite", "-N", action="store_false", dest="overwrite",
        help="don't overwrite existing decompressed files"
    )
    parser.add_argument(
        "--zlib", action="version",
        version=f"{zlib.__name__} {zlib.ZLIB_RUNTIME_VERSION}",
        help="show zlib's runtime version instead"
    )
    parser.add_argument(
        "args", nargs="+", type=pathlib.Path, metavar="replay",
        help="replay filename to unpack"
    )

    args = parser.parse_args(argv)
    code = 0

    for filename in args.args:
        try:
            process(filename, args.overwrite)

        except Exception as exception:
            error(exception)
            code = 1

    sys.exit(code)


if __name__ == "__main__":
    main()
