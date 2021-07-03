# 4ku
Trying to fit a chess engine into 4,096 bytes and failing miserably. Uses gzexe to compress the executable.

---

## Build
```
git clone https://github.com/kz04px/4ku
cd 4ku
sh build-linux.sh
```

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
