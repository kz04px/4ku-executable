mkdir -p build/linux

cd build/linux

cmake ../../src/.
cmake --build . --config Release

xz ./engine/4ku
cat ../../launcher.sh ./engine/4ku.xz > ./engine/4ku
rm ./engine/4ku.xz
chmod +x ./engine/4ku
