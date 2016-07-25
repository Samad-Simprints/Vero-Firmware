# Main PCB scripts
Created for __Python 3.5__ and Windows 10

## Installation
All scripts require the installation of the LPC scripts locally and the variable `nxpPath` in settings.py must be set to this installation folder.

### mainpcb.py
Requires the requests python package, which can be installed using the following command:

    py  -m pip install requests

### mainpcbFE.py
Requires the qrcode and pillow python packages, which can be installed using the following commands:

    py  -m pip install qrcode
    py  -m pip install pillow
 
## lpchelper.py
Wrappers around the NXP LPC scripts to flash a binary and MAC address to the Main PCB in the SFS.

Basic usage:

    $ python_interpreter lpchelper.py command arguments
Commands:

    boot                              Boot device
    progMac <MAC in integer form>     Program MAC address to Bank B
    progBankA <path to binary file>   Program binary to Bank A
    readMac                           Read MAC address from Bank B
Note: A command that requires the device to be booted will automatically do so. No separate boot command is required.

## mainpcb.py
Work flow to install binaries (firmware and MAC address) on a Main PCB.

Algorithm:

1. Program firmware to Main PCB
2. Find or create a database record, getting a MAC address and Bluetooth name in return.
3. Program MAC address to Main PCB 
4. Update status in database 
5. Return MAC address or error code

Basic usage:

    $ python_interpreter mainpcb.py workOrder serialNumber
Returns an integer. Positive values are the main PCB's assigned MAC address, negative values indicate an error has occured.    
    
Error returns:

    unknown_error           -1  Unknown Error has occured
    incorrect_args          -2  Incorrect arguments have been passed to the script
    file_or_script_missing  -3  Unable to locate a file or script
    boot_failure            -4  Unable to boot the device
    lpc_error               -5  The LPC script has raised an error
    request_error           -6  HTTP error
    mac_error               -7  Unable to retrieve a MAC address
    
## mainpcbFE.py
Front-end for programming a Main PCB. 

Algorithm:
    
1. Get *workOrder-serialNumber* from stdin
2. Call mainpcb.py
3. If successful, convert the returned MAC into the format expected by the test app
       and display as a QR code
