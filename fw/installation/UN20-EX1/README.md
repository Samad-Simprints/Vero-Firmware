# UN20-EX1
Scripts to program firmware onto a UN20-EX1 from a Windows machine. Currently there are two callable scripts:

    program-un20ex1.cmd     A Windows batch which programs a UN20-EX1
    un20ex1.py              A Python 3 script which programs a UN20-EX1 and sends the relevant 
                            data to the manufacturing REST API

## Installation
All scripts require PuTTY and WinSCP.

### program-un20ex1.cmd
The following variables need to be set in program-un20ex1.cmd:

    putty_path  = path to PuTTY
    scp_path    = path to WinSCP
    key         = a valid PuTTY private key file

### un20ex1.py
Built for Python 3.5. Requires the *requests* Python library, which can be installed using the following command:

    py  -m pip install requests

The following variables need to be set in settings.py:

    MANU_API    = text file containing a key for the manufacturing API
    PLINK       = path to plink (a PuTTY program)
    WINSCP      = path to WinSCP
    PPK         = a valid PuTTY private key file

## Use
Place **index.tar.gz** and **install-index-un20** in the same directory and call either script. un20ex1.py will ask for a string with the format *workOrder*-*serialNumber*. 
