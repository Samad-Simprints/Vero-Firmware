
import subprocess
import requests
import settings
import platform
import sys

sys_name = platform.system()
unit_name = 'UN20-EX1'
process_timeout = 10

if sys_name == 'Linux':
	processes = (
			# set ip address (assumes that the UN20 will appear as usb0)
			(['sudo','ifconfig','usb0','10.0.1.2','netmask','255.255.0.0'], '--> ERROR: Unable to set IP address of %s'%unit_name),
			# start ftp server on un20ex1 process 
			(['ssh','root@%s'%settings.HOST,'inetd /etc/inetd.conf; exit'], '--> ERROR: Can not start ftp server on %s'%unit_name),
			# transfer files using ftp to un20ex1
			(['./push-files.sh', settings.BUILD_PATH], '--> ERROR: file transfer to %s has failed'%unit_name),
			# call install script on un20ex1
			(['ssh','root@%s'%settings.HOST,'cd /data; chmod +x install-index-un20; sync; source install-index-un20; halt; exit'], 
				'--> ERROR: calling install script on %s has failed'%unit_name)
		    )
else:
    raise Exception


def installUN20():
    status = 'Passed'

    ############################################################
    ## get user input for Interface Board (from barcode scanner)
    ############################################################

    userInput = input('--> Please enter interface PCB barcode: ')

    ## split into work order and serial number 
    try:
        interfaceWO, interfaceSN = userInput.split('-')
    except ValueError:
        print( '--> ERROR: Expected a single "-" in %s' % userInput )
        return 

    ## check inputs
    if len( interfaceWO ) != 6:
        print( '--> ERROR: Expected a work order %s of length 6' % interfaceWO)
        return
    if len( interfaceSN ) != 3:
        print( '--> ERROR: Expected a serial number %s of length 3' % interfaceSN )
        return
    try:
        int( interfaceSN )
    except ValueError:
        print( '--> ERROR: Expected a numerical serial number %s' % interfaceSN )
        return

    print( '--> interfaceWO: %s' % interfaceWO )
    print( '--> interfaceSN: %s' % interfaceSN )


    #####################################################
    ## get user input for UN20-EX1 (from barcode scanner)
    #####################################################

    userInput = input('--> Please enter UN20-EX1 barcode: ')

    ## split into work order and serial number 
    try:
        un20WO, un20SN = userInput.split('-')
    except ValueError:
        print( '--> ERROR: Expected a single "-" in %s' % userInput )
        return 

    ## check inputs

    if len( un20WO ) != 3:
        print( '--> ERROR: Expected a work order %s of length 6' % un20WO)
        return
    if len( un20SN ) != 6:
        print( '--> ERROR: Expected a serial number %s of length 3' % un20SN )
        return
    try:
        int( un20SN )
    except ValueError:
        print( '--> ERROR: Expected a numerical serial number %s' % un20SN )
        return

    print( '--> un20WO: %s' % un20WO )
    print( '--> un20SN: %s' % un20SN )


    ##############################################
    ## wait for user to say it is okay to continue
    ##############################################
    userInput = input('--> Please attach the combined unit to the SimPi and then press any key to continue: ')

    ###################
    ## run each process
    ###################
    try:
        for process in processes:
            if sys.version_info >= (3,5):
                # run added in Python 3.5
                progRet = subprocess.run( process[0], stderr = subprocess.DEVNULL, timeout = process_timeout )
                returncode = progRet.returncode	
            else:
                returncode = subprocess.call( process[0], timeout = process_timeout)
            if returncode != 0:
                status = 'Failed'
                print(process[1])
                break
    except subprocess.TimeoutExpired:
        print( '--> ERROR: Subprocess timeout' )
        status = 'Failed'
    

    #############
    # get api key
    #############
    try:
        with open( settings.MANU_API, 'r') as f:
            apiKeyVal = f.readline()
    except:
        print( '--> ERROR: Can not find API key file %s' % settings.MANU_API )
        return 

    #######################
    # post to interface API
    #######################
    response = requests.get( '%s/%s'%(settings.INTERFACE_URL, 'GetInterfacePcbByWorkOrderAndSerial'), 
                              params = {'apiKey' : apiKeyVal, 'WorkOrder' : interfaceWO, 'Serial': interfaceSN})
    if response.ok:
        iData = response.json()
        print( '--> Interface PUT id = %s'%iData['Id'])
        #print( iData )
        iData['Status'] = status
        response = requests.put( '%s/%s/%d'%(settings.INTERFACE_URL, 'PutInterfacePCB', iData['Id']), 
                                  params = {'apiKey' : apiKeyVal}, data = iData)
    elif response.status_code == 404:
        # Can't find, so post
        print( '--> Interface POST')
        iData = {'WorkOrder': interfaceWO, 'SerialNumber':interfaceSN, 'Status' : status}
        #print( iData )
        response = requests.post( '%s/%s'%(settings.INTERFACE_URL, 'PostInterfacePCB'), 
                                  params = {'apiKey' : apiKeyVal}, data = iData)

    # raise anything untoward in response
    # print( response.url )
    response.raise_for_status()

    ##################
    # post to un20 API
    ##################
    response = requests.get( '%s/%s'%(settings.SCANNER_URL, 'GetScannerUnitBySerial'), 
                              params = {'apiKey' : apiKeyVal, 'serial': un20SN})
    if response.ok:
        uData = response.json()
        print( '--> Scanner PUT id = %s'%uData['Id'])
        #print( uData )
        uData['Status'] = status
        response = requests.put( '%s/%s/%d'%(settings.SCANNER_URL, 'PutScannerUnit', uData['Id']), 
                                  params = {'apiKey' : apiKeyVal}, data = uData)
    elif response.status_code == 404:
        # Can't find, so post
        print( '--> Scanner POST')
        uData = {'SerialNumber':un20SN, 'Status' : status}
        #print( uData )
        response = requests.post( '%s/%s'%(settings.SCANNER_URL, 'PostScannerUnit'), 
                                  params = {'apiKey' : apiKeyVal}, data = uData)
        
    # raise anything untoward in response
    response.raise_for_status()

    if status == 'Passed':
        print('\n\n\n\t\tPASSED\n\n\n')
    else:
        print('\n\n\n\t\tFAILED\n\n\n')
    return

if __name__ == '__main__':
    installUN20()

