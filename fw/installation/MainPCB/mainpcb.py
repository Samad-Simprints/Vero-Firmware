
'''
Programming Main PCB workflow
'''

import sys
import os
import shutil
import logging
import requests
import settings
from lpchelper import (
    programMainPcb,
    programMAC,
    bootError,
    lpcError,
    macError,
    )
from enum import Enum, unique

#### Logger
logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)

# create a file handler
handler = logging.FileHandler('mainpcb.log')
handler.setLevel(logging.INFO)

# create a logging format
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
handler.setFormatter(formatter)

# add the handlers to the logger
logger.addHandler(handler)


@unique     # ensure enum values are unique
class eToken( Enum ):
    '''
    Error enum
    '''
    unknown_error = -1
    incorrect_args = -2
    file_or_script_missing = -3
    boot_failure = -4
    lpc_error = -5
    request_error = -6
    mac_error = -7
    
def getApiKey():
    with open( settings.MANU_API, 'r') as f:
        apiKeyVal = f.readline()
    return apiKeyVal

class MainPCB( object ):
    ''' Object to represent a MainPCB '''

    def __init__(self, workOrder, serialNumber, dId = None, macAddress=None, bluetoothName=None, programmed=False):
        self.WorkOrder = workOrder
        self.SerialNumber = serialNumber
        self.Id = dId
        self.MacAddress = macAddress
        self.BluetoothName = bluetoothName
        self.Programmed = programmed

    def __repr__(self):
        return 'id = %s, wO = %s, sN = %s, MAC = %s, BT = %s, prog = %s'\
                    %( self.Id, 
                       self.WorkOrder, 
                       self.SerialNumber, 
                       hex(self.MacAddress) if self.MacAddress != None else None, 
                       self.BluetoothName,
                       self.Programmed,
                       )

    @property
    def toREST( self ):
        '''
        Convert an object's attributes into a dictionary that is ready to be posted to the REST DB
        '''
        return {
                'Id':                   getattr( self, 'Id', None),
                'WorkOrder' :           self.WorkOrder,
                'SerialNumber':         self.SerialNumber,
                'AndroidTestStatus':    getattr( self, 'AndroidTestStatus', None),
                'JTagTestStatus':       getattr( self, 'JTagTestStatus', None),
                'AssignMacAddress':     getattr( self, 'AssignMacAddress', False),
                #'MacAddress':           hex(self.MacAddress) if getattr( self, 'MacAddress', None) else None,
                #'BluetoothName':        getattr( self, 'BluetoothName', None),
                'Info':                 getattr( self, 'Info', None),
                'CreatedTime':          getattr( self, 'CreatedTime', None),
                'UpdatedTime':          getattr( self, 'UpdatedTime', None),
                'Programmed' :          self.Programmed,
               }

    def fromREST( self, aDict ):
        '''
        Update an object's attributes given a dictionary from the REST DB
        '''
        hStr ='0x'+ aDict['MacAddress'].replace('-','').lower() # convert yy-yy-yy-yy to 0xyyyyyyyy
        self.MacAddress = int( hStr, 16)                        # convert hex string to int
        self.Id = aDict['Id']
        self.AndroidTestStatus = aDict['AndroidTestStatus']
        self.JTagTestStatus = aDict['JTagTestStatus']
        self.AssignMacAddress = aDict['AssignMacAddress']
        self.BluetoothName = aDict['BluetoothName']
        self.Info = aDict['Info']
        self.CreatedTime = aDict['CreatedTime']
        self.UpdatedTime = aDict['UpdatedTime']
        self.Programmed = aDict['Programmed']
        
    def put( self ):
        ''' 
        PUT to database
        '''
        return requests.put( '%s/%s/%d'%(settings.MANU_MAINPCBURL, 'PutMainPCB', self.Id), 
                              params = {'apiKey' : getApiKey()}, 
                              data = self.toREST )

    def post( self ):
        ''' 
        POST to database
        '''
        data = self.toREST
        data['AssignMacAddress'] = True     # makes the DB assign a MAC address
        return requests.post( '%s/%s'%(settings.MANU_MAINPCBURL, 'PostMainPCB'), 
                              params = {'apiKey' : getApiKey()}, 
                              data = data )

    def getWS( self ):
        ''' 
        Get from database using Work Order and Serial Number
        '''
        return requests.get( '%s/%s'%(settings.MANU_MAINPCBURL, 'GetMainPcbByWorkOrderAndSerial'), 
                             params = {'apiKey' : getApiKey(), 'workOder':self.WorkOrder, 'serial':self.SerialNumber } )


def createMainPCB( workOrder, serialNumber ):

    ''' 
    Create a Main PCB from the given workOrder and serialNumber. 
    If a board already exists in the database, then this is used. 
    Otherwise a new board is created via a post. 
    '''

    board = MainPCB( workOrder = workOrder, serialNumber = serialNumber)
    #print( board )

    # look for board in DB with a GET
    response = board.getWS()

    if response.status_code == 200:
        print('--> Found existing record')
    elif response.status_code == 404:
        # if the board can't be found, POST to DB
        print('--> Create new record')
        response = board.post()

    # raise any request error now 
    response.raise_for_status()

    # use the data in the response to set the board attributes
    board.fromREST( response.json())
    #print( board )

    return board


def install( workOrder, serialNumber ):
    '''
    Work flow to install binaries on a Main PCB
        1. Program firmware to Main PCB
        2. Find or create a DB record, getting a MAC address and Bluetooth name in return.
        3. Program MAC address to Main PCB 
        4. Update status 
        5. Return MAC address or error code
    '''
    localBin = 'SIMindex.bin'
    try:
        board = None

        # copy binary file to local directory
        shutil.copyfile( settings.FIRMWARE, localBin) 

        print('--> Program firmware')
        # program binary
        programMainPcb( localBin )
        
        # now the board has been successful flashed, create a DB record for the board
        # Getting a MAC in the process
        print('--> Create DB record')
        board = createMainPCB( workOrder = workOrder, serialNumber = serialNumber )

        # program MAC
        print('--> Program MAC address %x'% board.MacAddress)
        programMAC( board.MacAddress )

    except bootError as err:
        print('--> WARNING: boot error')
        logger.error( err )
        ret = eToken.boot_failure.value 
    except FileNotFoundError as err:
        print('--> WARNING: missing file %s'% err)
        logger.error( err )
        ret = eToken.file_or_script_missing.value
    except lpcError as err:
        print('--> WARNING: LPC script error %s'% err)
        logger.error( err )
        ret = eToken.lpc_error.value
    except requests.RequestException as err:
        print('--> WARNING: Web error %s'%err)
        logger.error( 'RequestException: %s' %err )
        ret = eToken.request_error.value
    except macError as err:
        print('--> WARNING: %s'% err)
        logger.error( err )
        ret = eToken.mac_error.value
    except Exception as err:
        print('--> WARNING: UNKNOWN error \nException message %s'%err)
        ret = eToken.unknown_error.value
    else:
        ret = board.MacAddress

    # remove temp binary
    os.remove( localBin )

    if board:
        # update status
        board.Programmed = ret > 0
        print('--> Update test status to %s'% board.Programmed)
        # update DB
        response = board.put()
        # raise any request error now 
        response.raise_for_status()

    return ret

def master( argv ):
    '''
    Check user inputs and then call the install function
    '''
    if len( argv ) != 3:
        return eToken.incorrect_args.value
    else:
        return install( workOrder = argv[1], serialNumber = argv[2] )

if __name__ == '__main__':
    master( sys.argv )
