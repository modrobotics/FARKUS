FARKUS Desk
======

This software supports Modular Robotics' FARKUS Table.  It is designed to be
run on a Raspberry Pi, and has been optimized for use on that platform.  That
said, all of the code is cross-platform compatible and (with some minor modifications*)
you'll be running on Windows.

Assuming you're not trying to state your love for Windows by using an open source
factory automation project, here's how you'll get your Pi set up to use FARKUS Desk.

1. Start from a vanilla distro of Raspbian.  Download from: http://downloads.raspberrypi.org/raspbian_latest 
2. Get apt-get up-to-date:
        ```
        sudo apt-get update
        ```
3. Install wxPython 2.8 for Python 2.7 (Python 2.7 itself is installed by default):
        sudo apt-get install python-wxgtk2.8 python-wxtools wx2.8-i18n libwxgtk2.8-dev
4. Install pyserial.
        cd ~
        wget https://pypi.python.org/packages/source/p/pyserial/pyserial-2.6.tar.gz
        tar -xvf pyserial-2.6.tar.gz
        cd pyserial-2.6
        python ./setup.py install
5. Install git
        sudo apt-get install git
        ssh key-gen -t rsa -C "[your email here]"
6. There are some statically-defined file paths in this code.  Please clone this repo to
        cd /home/pi
        git clone https://github.com/modrobotics/FARKUS.git
7. Adjust permissions to allow execution
        sudo chmod o+x /home/pi/FARKUS/python/FarkusDesk.py
8. Run Desk.
        /home/pi/FARKUS/run.exe
9. Profit!

* The cross-platform capability of the serial device discovery functionality
  is currently statically coded to support UNIX/Mac.  Some commenting/uncommenting
  is all that is required to switch to Windows.