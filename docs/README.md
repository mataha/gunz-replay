# ``gunz-replay``

A simple [GunZ: The Duel][1] replay examiner - as it is right now,
it scans a directory for GunZ replays containing the specified character.

Warning: the code sucks. I repeat: **the code sucks** - it was written
rather hastily during a solo effort. However, there has been a demand
for a working tool like this, and having something available right now
is better than nothing - I hope it will prove useful.

## Installation

Classic approach (from a git repository; requires [Rust][2]):

```shell
cargo install --git "https://gitgud.io/mataha/gunz-replay"
```

Alternatively, one can download a binary from [here][3],
then place it somewhere accessible by the `PATH` (or even directly
in a replay directory).

## Manual

```
gzre <directory> <character>
```

In its current form, the analyzer scans the specified directory for
replays containing the specified character name.

There are a few caveats:

 - works only with replays up to V5 (mid-ijji)
 - doesn't support non-ASCII encodings yet ([EUC-KR][4] comes to mind)
 - only players present from the start are taken into account
 - takes *a rather long time* to slew through gargantuan collections
   of replays
 - doesn't exclude replays in an unviewable state (e.g. when a player
   has started recording a replay while being alone in a TDM game, or
   if a replay got desynchronized due to players dying simultaneously)
 - doesn't take filenames into account - my stance on this is that one
   should not rely on such a fragile mechanism for file organization;
   however, you can perform a manual search easily by yourself,
   e.g. on Windows:
   
   ```batch
   dir /b "<directory>" | find /i "<character>"
   ```

## Sample usage

Assuming that our collection looks like this:

```shell
$ ls -1
'[1v1] AryaVarwin vs. Garamgood.gzr'
'[1v1] Itamz vs. GilGilGil.gzr'
'[1v1] Mataha vs. FireArrowz.gzr'
'[1v1] Screwy vs. Beyondpower.gzr'
'[1v1] toktok vs. mijoma.gzr'
'[CW] Aplus vs. Dowon.gzr'
'[CW] Arashi vs. Magicians.gzr'
'[CW] Curse vs. Flower.gzr'
'[CW] Icon vs. Strike.gzr'
'Empty replay.gzr'
```

An invocation of the program will produce the following output:

```shell
$ gzre . "toktok"  # search in the current directory
Path: .

Replays featuring toktok:

    "./[1v1] toktok vs. mijoma.gzr"
    "./[CW] Curse vs. Flower.gzr"

2 matches found, 9 valid replays scanned.
```

## License

GPLv3+. See [LICENSE](../LICENSE).

[1]: https://en.wikipedia.org/wiki/GunZ:_The_Duel
[2]: https://www.rust-lang.org/
[3]: https://github.com/mataha/gunz-replay/releases
[4]: https://en.wikipedia.org/wiki/Extended_Unix_Code#EUC-KR
