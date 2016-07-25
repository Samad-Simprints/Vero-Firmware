
'''
Front-end script for programming Main PCBs 
Takes a barcode string, programs a board and displays its MAC address as a QR code
'''


import qrcode
from mainpcb import install


def displayQRCode( mstring ):
    '''
    Output string as QR code
    '''
    img = qrcode.make( mstring )
    img.show()

def convertToAndroidHex( val ):
    '''
    Convert an integer into the hex representation expected by the test app xx:xx:xx:xx:xx:xx
    '''
    hStr = hex( val )[2:].upper()   # cut off the leading '0x' and convert to uppercase
    return ':'.join( ( hStr[0:2], hStr[2:4], hStr[4:6], hStr[6:8], hStr[8:10], hStr[10:12]))

if __name__ == '__main__':
    '''
    Workflow:

    1. Get workOrder:serialNumber from stdin
    2. Call mainpcb.py
    3. If successful, convert the returned MAC into the format expected by the test app
       and display as a QR code
    '''
    ## get user input (from barcode scanner)
    userInput = input('--> Please enter barcode: ')

    ## split into work order and serial number 
    try:
        workOrder, serialNumber = userInput.split('-')
    except ValueError:
        print( '--> ERROR: Expected a single "-" in %s' % userInput )
        raise Exception

    ## check inputs
    if len( workOrder ) != 6:
        print( '--> ERROR: Expected a work order %s of length 6' % workOrder)
        raise Exception
    if len( serialNumber ) != 3:
        print( '--> ERROR: Expected a serial number %s of length 3' % serialNumber )
        raise Exception
    try:
        int( serialNumber )
    except ValueError:
        print( '--> ERROR: Expected a numerical serial number %s' % serialNumber )
        raise Exception

    print( '--> workOrder: %s' % workOrder )
    print( '--> serialNumber: %s' % serialNumber )

    ret = install( workOrder = workOrder, serialNumber = serialNumber )

    if ret > 0:
        ## display QR code
        displayQRCode( convertToAndroidHex( ret ))
        print( '\n\n--> PASSED\n\n')
    else:
        print( '\n\n--> FAILED\n\n')
