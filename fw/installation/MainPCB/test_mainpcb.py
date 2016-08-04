import unittest
import unittest.mock
import sys
import struct
from lpchelper import bootError, lpcError, lpcProgram
from mainpcb import *

patchMainPcb = 'mainpcb.programMainPcb'
patchCreate = 'mainpcb.createMainPCB'
patchMAC = 'mainpcb.programMAC'
patchPut = 'mainpcb.MainPCB.put'

class failureTests( unittest.TestCase ):
    def setUp( self ):
        # setup mock
        self._mainPatcher = unittest.mock.patch( patchMainPcb )
        self._mainMock = self._mainPatcher.start()
        self._createPatcher = unittest.mock.patch( patchCreate )
        self._createMock = self._createPatcher.start()
        self._macPatcher = unittest.mock.patch( patchMAC )
        self._macMock = self._macPatcher.start()
        self._putPatcher = unittest.mock.patch( patchPut )
        self._putMock = self._putPatcher.start()

    def tearDown( self ):
        self._mainPatcher.stop()
        self._createPatcher.stop()
        self._macPatcher.stop()
        self._putPatcher.stop()

    def test_tooFewInputs( self ):
        ''' Too few inputs supplied '''
        ret = master( ['mainpcb.py', '123456789'] )
        self.assertEqual( ret, eToken.incorrect_args.value)

    def test_tooManyInputs( self ):
        ''' Too few inputs supplied '''
        ret = master( ['mainpcb.py', '123456789', '987654321', 'whatAreYouDoingHere'] )
        self.assertEqual( ret, eToken.incorrect_args.value)

    def test_noBootScript( self ):
        ''' lpc script is MIA '''
        self._mainMock.side_effect = FileNotFoundError()
        ret = install(workOrder = '123456789', serialNumber = '987654321')
        self.assertEqual( ret, eToken.file_or_script_missing.value)

    def test_failureToBootMain( self ):
        ''' program Main PCB fails to boot '''
        self._mainMock.side_effect = bootError()
        ret = install(workOrder = '123456789', serialNumber = '987654321')
        self.assertEqual( ret, eToken.boot_failure.value)

    def test_failureToProgramBinary( self ):
        ''' Failure to program binary '''
        self._mainMock.side_effect = lpcError(lpcProgram, binary = 'anExample.bin')
        ret = install(workOrder = '123456789', serialNumber = '987654321')
        self.assertEqual( ret, eToken.lpc_error.value)

    def test_failureToPost( self ):
        ''' Can't post to REST '''
        self._createMock.side_effect = requests.RequestException()
        ret = install(workOrder = '123456789', serialNumber = '987654321')
        self.assertEqual( ret, eToken.request_error.value)

    def test_failureToBootMAC( self ):
        ''' program MAC fails to boot '''
        self._createMock.return_value = MainPCB(workOrder = '12345A', serialNumber='001', macAddress=111111, bluetoothName='SP1')
        self._macMock.side_effect = bootError()
        ret = install(workOrder = '123456789', serialNumber = '987654321')
        self.assertTrue( self._macMock.called )     # check that we made it to programm mac
        self.assertEqual( ret, eToken.boot_failure.value)
        
    def test_failureToProgramMAC( self ):
        ''' failure to program MAC '''
        self._createMock.return_value = MainPCB(workOrder = '12345A', serialNumber='001', macAddress=111111, bluetoothName='SP1')
        self._macMock.side_effect = lpcError(lpcProgram, binary = 'ccc1')
        ret = install(workOrder = '123456789', serialNumber = '987654321')
        self.assertTrue( self._macMock.called )     # check that we made it to programm mac
        self.assertEqual( ret, eToken.lpc_error.value)

class correctTests( unittest.TestCase ):
    def setUp( self ):
        # setup mock
        self._mainPatcher = unittest.mock.patch( patchMainPcb )
        self._mainMock = self._mainPatcher.start()
        self._createPatcher = unittest.mock.patch( patchCreate )
        self._createMock = self._createPatcher.start()
        self._macPatcher = unittest.mock.patch( patchMAC )
        self._macMock = self._macPatcher.start()
        self._putPatcher = unittest.mock.patch( patchPut )
        self._putMock = self._putPatcher.start()

    def tearDown( self ):
        self._mainPatcher.stop()
        self._createPatcher.stop()
        self._macPatcher.stop()
        self._putPatcher.stop()

    def test_MACreturned( self ):
        ''' check that the MAC address is returned by the script '''
        macAddress = 111111
        self._createMock.return_value = MainPCB(workOrder = '12345A', serialNumber='001', macAddress=macAddress, bluetoothName='SP1')
        ret = install(workOrder = '123456789', serialNumber = '987654321')
        self.assertEqual( ret, macAddress)

if __name__ == '__main__':
    unittest.main()
	
