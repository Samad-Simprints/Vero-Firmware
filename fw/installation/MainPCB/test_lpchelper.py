import unittest
import unittest.mock
import logging
import sys
import struct
from subprocess import CompletedProcess
from lpchelper import *

patchLpc = 'lpchelper.runLpc'
patchBoot = 'lpchelper.runBoot'
patchRead = 'lpchelper.readMacAddr'
patchProgMac = 'lpchelper.programMAC'
patchProgMainPcb = 'lpchelper.programMainPcb'
failCommand = None 

def failOnCommand( command, *args, stdout = None ):
    ''' Patch side-effect for callLpc - returns failure code on command '''
    returncode = -1 if command ==  failCommand else 0
    return CompletedProcess( args = [command, *args], returncode=returncode, stdout = stdout)

class test_generateCRC( unittest.TestCase ):
    def test_CRCone( self ):
        ''' check hte CRC generator returns the correct value '''
        self.assertEqual(generateCRC(0xf0acd7c0000b), 4633)

class test_programMemory( unittest.TestCase ):
    def setUp( self ):
        global failCommand
        failCommand = None
        # setup mock
        self._patcher = unittest.mock.patch( patchLpc )
        self._mock = self._patcher.start()


    def tearDown( self ):
        self._patcher.stop()

    def test_failureToProgram( self ):
        ''' raise exception if lpc program fails '''
        global failCommand
        failCommand = lpcProgram
        self._mock.side_effect = failOnCommand
        with self.assertRaises(lpcError):
            programMemory('ignored', 'ignored')

    def test_failureToVerify( self ):
        ''' raise exception if lpc verify fails '''
        global failCommand
        failCommand = lpcVerify
        self._mock.side_effect = failOnCommand
        with self.assertRaises(lpcError):
            programMemory('ignored', 'ignored')
        
class test_readMacAddr( unittest.TestCase ):
    def setUp( self ):
        global failCommand
        failCommand = None
        # setup mock
        self._patcher = unittest.mock.patch( patchLpc )
        self._mock = self._patcher.start()


    def tearDown( self ):
        self._patcher.stop()

    def test_failureToRead( self ):
        ''' raise exception if lpc mem display fails '''
        global failCommand
        failCommand = lpcProgram
        self._mock.side_effect = failOnCommand
        with self.assertRaises(lpcError):
            readMacAddr()

    def test_checkReturn( self ):
        ''' check return of lpc mem display '''
        displayValue = b'0x1b002000 c0d7acf0 12190b00 ffffffff ffffffff\r\n'
        self._mock.return_value = CompletedProcess( args = [], returncode=0, stdout = displayValue )
        mac, crc = readMacAddr()
        self.assertEqual( mac, 0xf0acd7c0000b)
        self.assertEqual( crc, 4633)

class test_deviceBooted( unittest.TestCase ):
    def setUp( self ):
        global failCommand
        failCommand = None
        # setup mock
        self._bootPatcher = unittest.mock.patch( patchBoot )
        self._bootMock = self._bootPatcher.start()
        self._lpcPatcher = unittest.mock.patch( patchLpc )
        self._lpcMock = self._lpcPatcher.start()


    def tearDown( self ):
        self._bootPatcher.stop()
        self._lpcPatcher.stop()

    def test_failureToBoot( self ):
        ''' raise exception if device fails to boot '''
        global failCommand
        failCommand = lpcTarInfo
        self._bootMock.return_value = CompletedProcess( args = [], returncode = 1 )
        self._lpcMock.side_effect = failOnCommand
        with self.assertRaises(bootError):
            deviceBooted()

    def test_successToBoot( self ):
        ''' successful if device boots '''
        self._bootMock.return_value = CompletedProcess( args = [], returncode = 0 )
        deviceBooted()

class test_programMAC( unittest.TestCase ):
    def setUp( self ):
        global failCommand
        failCommand = None
        # setup mock
        self._bootPatcher = unittest.mock.patch( patchBoot )
        self._bootMock = self._bootPatcher.start()
        self._lpcPatcher = unittest.mock.patch( patchLpc )
        self._lpcMock = self._lpcPatcher.start()
        self._readPatcher = unittest.mock.patch( patchRead )
        self._readMock = self._readPatcher.start()


    def tearDown( self ):
        self._bootPatcher.stop()
        self._lpcPatcher.stop()
        self._readPatcher.stop()

    def test_MACfailureToBoot( self ):
        ''' raise exception when programming MAC and device fails to boot '''
        self._bootMock.return_value = CompletedProcess( args = [], returncode=1 )
        with self.assertRaises(bootError):
            programMAC(0xf0acd7c00000)

    @unittest.skip('Existing MAC is No longer an error')
    def test_failureExistMAC( self ):
        ''' raise exception when programming and device has existing valid MAC address '''
        self._bootMock.return_value = CompletedProcess( args = [], returncode = 0 )
        self._readMock.return_value = 0xf0acd7c0000b, 4633
        with self.assertRaises(macError):
            programMAC(0xf0acd7c00000)

    def test_failureToProgramMAC( self ):
        ''' raise exception when flashing a MAC address and program fails '''
        self._bootMock.return_value = CompletedProcess( args = [], returncode = 0 )
        self._readMock.return_value = 0xffffffffffff, 0
        self._lpcMock.return_value = CompletedProcess( args = [], returncode = 1 )
        with self.assertRaises(lpcError):
            programMAC(0xf0acd7c0000b)

    @unittest.skip('Fix test for bin files')
    def test_successfulCheckProgInputs( self ):
        ''' Program MAC address success, check inputs '''
        newMAC = 0xf0acd7c0000b
        expBin = struct.pack('>Q', 0xf0acd7c0000b1912)
        self._bootMock.return_value = CompletedProcess( args = [], returncode = 0 )
        self._readMock.return_value = 0xffffffffffff, 0
        self._lpcMock.return_value = CompletedProcess( args = [], returncode = 0 )
        programMAC( newMAC )
        #@todo make the following call resistant to function order changes 
        progInputs = self._lpcMock.mock_calls[1]    # get call obj from program call (positional input at index 1)
        self.assertEqual( progInputs[1][2], bankBsector1 )
        self.assertEqual( progInputs[1][1], expBin )

class masterTest( unittest.TestCase ):
    def test_bootFail( self ):
        ''' Test failing boot call from master ''' 
        with unittest.mock.patch( patchBoot ) as mock:
            mock.return_value = CompletedProcess( args = [], returncode = 1 )
            master( [MFuncs.boot.name] )

    def test_bootSuccess( self ):
        ''' Test successful boot call from master '''
        with unittest.mock.patch( patchBoot ) as mock:
            mock.return_value = CompletedProcess( args = [], returncode = 0 )
            master( [MFuncs.boot.name] )

    def test_progMacFailNonValue( self ):
        ''' Test program MAC address with a boot failure from master  ''' 
        with unittest.mock.patch( patchProgMac ) as mock:
            mock.return_value = CompletedProcess( args = [], returncode = 1 )
            master( [MFuncs.progMac.name, 'thisShouldBeAnInteger'] )

    def test_progMacFailProg( self ):
        ''' Test non-int MAC value from master  ''' 
        with unittest.mock.patch( patchProgMac ) as mock:
            mock.side_effect = lpcError('program')
            master( [MFuncs.progMac.name, 1] )

    def test_progMacFailBoot( self ):
        ''' Test program MAC address with a boot failure from master  ''' 
        with unittest.mock.patch( patchProgMac ) as mock:
            mock.side_effect = bootError()
            master( [MFuncs.progMac.name, 1] )

    """
    @todo
    def test_progMacFailExisting( self ):
        ''' Test program MAC address a device with an existing MAC address from master  ''' 
        with unittest.mock.patch( patchProgMac ) as mock:
            mock.side_effect = bootError()
            master( [MFuncs.progMac.name, 1] )
    """

    def test_progMacSuccess( self ):
        ''' Test successful program of MAC value from master  ''' 
        with unittest.mock.patch( patchProgMac ) as mock:
            mock.return_value = None
            master( [MFuncs.progMac.name, 1] )

    def test_progBankAFailProg( self ):
        ''' Test program BankA with a program failure from master  ''' 
        with unittest.mock.patch( patchProgMainPcb ) as mock:
            mock.side_effect = lpcError('program')
            master( [MFuncs.progBankA.name, 'anIndex.bin'] )

    def test_progBankAFailBoot( self ):
        ''' Test program BankA with a boot failure from master  ''' 
        with unittest.mock.patch( patchProgMainPcb ) as mock:
            mock.side_effect = bootError()
            master( [MFuncs.progBankA.name, 'anIndex.bin'] )

    def test_progBankASuccess( self ):
        ''' Test program BankA with a boot failure from master  ''' 
        with unittest.mock.patch( patchProgMainPcb ) as mock:
            mock.return_value = None
            master( [MFuncs.progBankA.name, 'anIndex.bin'] )

    def test_readMACFailRead( self ):
        ''' Test read MAC with a read failure from master  ''' 
        with unittest.mock.patch( patchRead ) as mock:
            mock.side_effect = lpcError('read')
            master( [MFuncs.readMac.name] )

    def test_readMACFailBoot( self ):
        ''' Test read MAC with a boot failure from master  ''' 
        with unittest.mock.patch( patchRead ) as mock:
            mock.side_effect = bootError()
            master( [MFuncs.readMac.name] )

    def test_readMACSuccess( self ):
        ''' Test read MAC with a boot failure from master  ''' 
        with unittest.mock.patch( patchRead ) as mock:
            mock.return_value =0xf0acd7c0000b, 4633
            master( [MFuncs.readMac.name] )
        
if __name__ == '__main__':
    unittest.main()
	
