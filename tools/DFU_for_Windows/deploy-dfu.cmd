@echo off
setlocal

set PYTHON=python
set DFU=%~dp0\dfu.py
set PYDFU=%~dp0\pydfu.py

set TEXT0_ADDR=0x8008000
set DFU_VID=0x0694
set DFU_PID=0x0008
set TEMPFILE=%~dp0\firmware.dfu

if "%~1"=="" (
  echo ERROR: Upload file not specified
  exit /b
)

if not exist %~f1 (
  echo ERROR: File not found: %~f1
  pause
  exit /b
)

if not exist %DFU% (
  echo ERROR: File not found: %DFU%
  pause
  exit /b
)

if not exist %PYDFU% (
  echo ERROR: File not found: %PYDFU%
  pause
  exit /b
)

%PYTHON% %DFU% -b %TEXT0_ADDR%:%~f1 %TEMPFILE%

if not exist %TEMPFILE% (
  echo ERROR: File conversion failed: %~f1
  pause
  exit /b
)

%PYTHON% %PYDFU% -u %TEMPFILE% --vid %DFU_VID% --pid %DFU_PID%
DEL %TEMPFILE%

pause