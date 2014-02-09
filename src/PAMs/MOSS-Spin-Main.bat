


REM C:\Users\jmoyes\Desktop\ModRobotics\GitHub\FARKUS\src\hex\MOSS\spin-main


@ECHO off

TITLE Modular Robotics - MOSS Spin-Main v1.0.0

:start

:burnFusesm168
avrdude -p ATtiny13 -P usb -c avrispmkII -b 9600 -u -U lfuse:w:0x7a:m -U hfuse:w:0xfb:m
IF errorlevel 1 GOTO fusesError
GOTO fuseBurnSuccess

:fuseBurnSuccess
GOTO burnFlash

:burnFlash
SLEEP 1
avrdude -p ATtiny13 -P usb -c avrispmkII -b 9600 -U flash:w:C:\Users\jmoyes\Desktop\ModRobotics\GitHub\FARKUS\src\hex\MOSS\spin-main\SPIN-MAIN-1.0.0.hex:i > nul 2>&1
IF errorlevel 1 GOTO flashError
GOTO flashBurnSuccess

:flashBurnSuccess
EXIT 0

:fusesError
EXIT 2

:flashError
EXIT 3
