mkdir -p build/linux

cd build/linux

cmake ../../src/.
cmake --build . --config Release

# Strip
strip ./engine/4ku --strip-all

# Create gzexe version
gzexe ./engine/4ku

# Delete gzexe backup
rm ./engine/4ku~
