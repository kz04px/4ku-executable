mkdir -p build/linux

cd build/linux

rm ./engine/4ku

cmake ../../src/.
cmake --build . --config Release

# uncomment to display the sections before strip
# readelf -a engine/4ku

# keep only the sections that are needed to run the program
strip engine/4ku --strip-all \
      -R '*' \
      --keep-section=".bss" \
      --keep-section=".dynamic" \
      --keep-section=".dynstr" \
      --keep-section=".dynsym" \
      --keep-section=".fini" \
      --keep-section=".fini_array" \
      --keep-section=".gnu.version_r" \
      --keep-section=".got" \
      --keep-section=".got.plt" \
      --keep-section=".init" \
      --keep-section=".init_array" \
      --keep-section=".interp" \
      --keep-section=".rela.dyn" \
      --keep-section=".rodata" \
      --keep-section=".text"

lzma ./engine/4ku
cat ../../launcher.sh ./engine/4ku.lzma > ./engine/4ku
rm ./engine/4ku.lzma
chmod +x ./engine/4ku
