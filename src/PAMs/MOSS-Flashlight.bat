


REM C:\Users\jmoyes\Desktop\ModRobotics\GitHub\FARKUS\src\hex\MOSS\flashlight


@ECHO off

TITLE Modular Robotics - MOSS Flashlight v1.0.0 (ATMEGA168V/P)

:start

:burnFusesm168
avrdude -p ATtiny13 -P usb -c avrispmkII -b 9600 -u -U lfuse:w:0x7a:m -U hfuse:w:0xfb:m > nul 2>&1.
IF errorlevel 1 GOTO fusesError
GOTO fuseBurnSuccess

:fuseBurnSuccess
GOTO burnFlash

:burnFlash
SLEEP 1
REM avrdude -p m168 -P usb -c avrispmkII -b 115200 -U flash:w:C:\modrobot\Bootloader\build\bootloader_avr.hex:i 2>"C:\modrobot\Bootloader\tmp\flashErr.txt"
REM IF errorlevel 1 GOTO flashm168Error
GOTO flashBurnSuccess

:flashBurnSuccess
CD C:\modrobot\Bootloader\src\avr_battery\
make clean > "C:\modrobot\Bootloader\tmp\makeLog.txt"
EXIT 0

:fusesError
EXIT 2

:flashError
EXIT 3
