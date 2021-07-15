# 4ku
Trying to fit a chess engine into 4,096 bytes with some mild success. Uses [xz](https://linux.die.net/man/1/xz).

---

## Build
```
git clone https://github.com/kz04px/4ku
cd 4ku
sh build-linux.sh
```

---

## Executable Size
Varies from system to system, but for me it's currently at **3,574 bytes**.

---

## UCI Support
The UCI protocol isn't strictly followed in the interests of space:
- The first line of input after startup will be interpreted as "uci" even if it isn't.
- "stop" is unsupported.
- Both "position fen [fen]" and "position startpos" are ignored.
- Only wtime and btime are supported.

---

## Thanks
- [Catch2](https://github.com/catchorg/Catch2) -- A modern, C++-native, header-only, test framework for unit-tests, TDD and BDD - using C++11, C++14, C++17 and later
- Roman Hartmann's perft suite
- Thanks to friends for help with compiler flags and xz scripts
