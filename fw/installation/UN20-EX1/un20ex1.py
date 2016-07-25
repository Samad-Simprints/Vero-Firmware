
import subprocess
import requests
import settings
import platform
import sys

sys_name = platform.system()

if sys_name == 'Windows':
	processes = (
			# start ftp server on un20ex1 process 
			[settings.PLINK,'-ssh','root@%s'%settings.HOST,'-i','%s'%settings.PPK,'-m', 'start-ftp-service.sh','-N'],
			# transfer files using WinSCP to un20ex1
			[settings.WINSCP, '/script=winscp-ftp-commands.txt'],
			# call install script on un20ex1
			[settings.PLINK,'-ssh','root@%s'%settings.HOST,'-i','%s'%settings.PPK,'-m', 'call-install-index.sh','-N'],
		    )
elif sys_name == 'Linux':
	processes = (
			# set ip address (assumes that the UN20 will appear as usb0)
			['sudo','ifconfig','usb0','10.0.1.2','netmask','255.255.0.0'],
			# start ftp server on un20ex1 process 
			['ssh','root@%s'%settings.HOST,'inetd /etc/inetd.conf; exit'],
			# transfer files using ftp to un20ex1
			['./push-files.sh', settings.BUILD_PATH],
			# call install script on un20ex1
			['ssh','root@%s'%settings.HOST,'cd /data; chmod +x install-index-un20; sync; source install-index-un20; halt; exit'],
		    )
else:
    raise Exception


def installUN20():
    ## get user input (from barcode scanner)
    userInput = input('--> Please enter barcode: ')
    status = 'Passed'

    ## split into work order and serial number 
    try:
        workOrder, serialNumber = userInput.split('-')
    except ValueError:
        print( '--> ERROR: Expected a single "-" in %s' % userInput )
        return 

    ## check inputs
    if len( workOrder ) != 6:
        print( '--> ERROR: Expected a work order %s of length 6' % workOrder)
        return
    if len( serialNumber ) != 3:
        print( '--> ERROR: Expected a serial number %s of length 3' % serialNumber )
        return
    try:
        int( serialNumber )
    except ValueError:
        print( '--> ERROR: Expected a numerical serial number %s' % serialNumber )
        return

    print( '--> workOrder: %s' % workOrder )
    print( '--> serialNumber: %s' % serialNumber )

    ## run each process
    for process in processes:
        if sys.version_info >= (3,5):
            # run added in Python 3.5
            progRet = subprocess.run( process, stderr = subprocess.DEVNULL )
            returncode = progRet.returncode	
        else:
            returncode = subprocess.call( process )
        if returncode != 0:
            status = 'Failed'
            print('--> WARNING: Failed to program')
            break

    # post status
    try:
        with open( settings.MANU_API, 'r') as f:
            apiKeyVal = f.readline()
    except:
        print( '--> ERROR: Can not find API key file %s' % settings.MANU_API )
        return 

    response = requests.get( '%s/%s'%(settings.MANU_URL, 'GetInterfacePcbByWorkOrderAndSerial'), 
                              params = {'apiKey' : apiKeyVal, 'WorkOrder' : workOrder, 'Serial': serialNumber})
    if response.ok:
        data = response.json()
        print( '--> Does exist (Id = %d), PUT'%data['Id'])
        #print( data )
        data['Status'] = status
        response = requests.put( '%s/%s/%d'%(settings.MANU_URL, 'PutInterfacePCB', data['Id']), 
                                  params = {'apiKey' : apiKeyVal}, data = data)
    elif response.status_code == 404:
        # Can't find, so post
        print( '--> Does not exist, POST')
        data = {'WorkOrder': workOrder, 'SerialNumber':serialNumber, 'Status' : status}
        #print( data )
        response = requests.post( '%s/%s'%(settings.MANU_URL, 'PostInterfacePCB'), 
                                  params = {'apiKey' : apiKeyVal}, data = data)

        
    # raise anything untoward in response
    response.raise_for_status()
    return

if __name__ == '__main__':
    installUN20()

