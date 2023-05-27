#/bin/bash
OBJCOPY=arm-none-eabi-objcopy
elf=${1:-build/asp}
name=${2:-Pybricks Hub}
name=${name:0:15}
for i in $(seq ${#name} 15); do name+="\0"; done
file=$(mktemp)
echo -en $name > $file
$OBJCOPY --update-section .name=$file -S $elf
rm -f $file
