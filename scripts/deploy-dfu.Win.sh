#!/bin/bash -xe
TOOL_DIR=$( cd "$( dirname "$0" )" && pwd -P )/../tools

# run python3 in Windows, where finding DFU devices is straightforward.
PYTHON3=python.exe

MPTOP=$TOOL_DIR/../external/libpybricks/micropython
DFU=$MPTOP/tools/dfu.py
PYDFU=$MPTOP/tools/pydfu.py

TEXT0_ADDR=0x8008000
DFU_VID=0x0694
DFU_PID=0x0008


echo "DFU Create $1"
$PYTHON3 $(wslpath -w $DFU) -b $TEXT0_ADDR:$1 firmware.dfu

echo "Writing $1 to the board"
$PYTHON3 $(wslpath -w $PYDFU) -u firmware.dfu --vid $DFU_VID --pid $DFU_PID

rm -rf firmware.dfu
