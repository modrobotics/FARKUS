@ECHO off

TITLE Modular Robotics - Cubelets Bootloader (PIC16LF1829)

SET ICD3URI="C:\Program Files (x86)\Microchip\MPLAB IDE\Programmer Utilities\ICD3"

:start
ECHO. > "C:\modrobot\Bootloader\src\pic\define_ids.h"

REM Require that CLI arguments are appropriate: arg 1 is an integer (the ID)
IF "%1"=="" (
GOTO idDBFailure
) ELSE (
SET /A next = "%1"
)

REM ECHO   ID being programmed is %next%

SET /A ID0 = %next% ^& 0xff
SET /A ID1 = %next%/256
SET /A ID1 = %ID1% ^& 0xff
SET /A ID2 = %next%/256
SET /A ID2 = %ID2%/256
SET /A ID2 = %ID2% ^& 0xff

ECHO unsigned char MyID2=%ID2%; > "C:\modrobot\Bootloader\src\pic\define_ids.h"
ECHO unsigned char MyID1=%ID1%; >> "C:\modrobot\Bootloader\src\pic\define_ids.h"
ECHO unsigned char MyID0=%ID0%; >> "C:\modrobot\Bootloader\src\pic\define_ids.h"
GOTO make

:make
CD C:\modrobot\Bootloader\src\pic\
make clean > "C:\modrobot\Bootloader\tmp\makeLog.txt"
IF errorlevel 1 GOTO makeError
CD C:\modrobot\Bootloader\src\pic\
make > "C:\modrobot\Bootloader\tmp\makeLog.txt"
IF errorlevel 1 GOTO makeError

:makeSuccess
GOTO burnFlashPIC

:burnFlashPIC
CD %ICD3URI%
ICD3CMD.exe -P16LF1829 -FC:\modrobot\Bootloader\build\bootloader_pic.hex -E -M -L > "C:\modrobot\Bootloader\tmp\makeLog.txt"

IF errorlevel 1 GOTO flashPICError
IF errorlevel 2 GOTO flashPICError
IF errorlevel 3 GOTO flashPICError
IF errorlevel 4 GOTO flashPICError
IF errorlevel 5 GOTO flashPICError
IF errorlevel 6 GOTO flashPICError
IF errorlevel 7 GOTO flashPICError
IF errorlevel 0 GOTO flashBurnSuccess

:flashBurnSuccess
CD C:\modrobot\Bootloader\src\pic\
make clean > "C:\modrobot\Bootloader\tmp\makeLog.txt"
EXIT 0

:makeError
EXIT 1

:flashPICError
EXIT 3

:idDBFailure
EXIT 4
