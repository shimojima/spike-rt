( cd build
  make
  make asp.bin
)

if [ -x build/asp.bin ]; then
  cp -a build/asp.bin .
  explorer.exe .
fi
