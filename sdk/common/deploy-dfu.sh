#!/bin/bash
HERE=$( cd "$( dirname "$0" )" && pwd -P )

if command -v python.exe > /dev/null 2>&1; then
  PYTHON3=python.exe
else
  PYTHON3=python3
fi

MPTOP=$HERE/../../asp3/target/primehub_gcc/
DFU=$MPTOP/tools/dfu.py
PYDFU=$MPTOP/tools/pydfu.py

TEXT0_ADDR=0x8008000
DFU_VID=0x0694
DFU_PID=0x0008


echo "DFU Create $1"
$PYTHON3 $DFU -b $TEXT0_ADDR:$1 firmware.dfu

echo "Writing $1 to the board"
$PYTHON3 $PYDFU -u firmware.dfu --vid $DFU_VID --pid $DFU_PID

rm -rf firmware.dfu
