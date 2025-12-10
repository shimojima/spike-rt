( cd build
  make
  make asp.bin
  if [ -x asp.bin ]; then
    cp -a asp.bin ..
  fi
)

explorer.exe .
