#!/bin/bash
## Script to dump the name of the Hub within the ELF32 image.
## This is almost equivalent to the following command:
##   readelf -p .name ${1:-build/asp}
## albeit different output format, which reveals the trailing null bytes more clearly.
OBJCOPY=arm-none-eabi-objcopy
file=$(mktemp)
$OBJCOPY --dump-section .name=$file -S ${1:-build/asp}
cat -t $file
rm -f $file
