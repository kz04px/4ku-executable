# 4ku-executable
Trying to fit a chess engine into 4,096 bytes with some mild success. Uses [xz](https://linux.die.net/man/1/xz). This engine is no longer actively worked on in favour of 4ku found [here](https://github.com/kz04px/4ku). 4ku-executable uses a compressed executable, where as 4ku uses compressed source and is significantly stronger while still fitting into the same size limitations.

This engine should probably not appear in any rating lists.

---

## Build
```
git clone https://github.com/kz04px/4ku-executable
cd 4ku-executable
sh build-linux.sh
```

---

## Executable Size
Varies from system to system, but for me it's currently at **2,908 bytes**.

---

## UCI Support
The UCI protocol isn't strictly followed in the interests of space:
- The first line of input after startup will be interpreted as "uci" even if it isn't.
- "stop" is unsupported.
- Both "position fen [fen]" and "position startpos" are ignored.
- Only wtime and btime are supported.

---

## Thanks
- [Catch2](https://github.com/catchorg/Catch2) -- A modern, C++-native, header-only, test framework for unit-tests, TDD and BDD - using C++11, C++14, C++17 and later.
- Roman Hartmann's perft suite.
- Thanks to friends for help with compiler flags, xz scripts, and code review.
