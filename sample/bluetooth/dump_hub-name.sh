#/bin/bash
## readelf -p .name ${1:-build/asp}
OBJCOPY=arm-none-eabi-objcopy
file=$(mktemp)
$OBJCOPY --dump-section .name=$file -S ${1:-build/asp}
cat -t $file
rm -f $file
