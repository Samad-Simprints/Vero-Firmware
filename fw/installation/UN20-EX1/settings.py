'''

Global variables and settings

'''

''' Installation variables '''
# text file containing key for manufacturing API
MANU_API = 'manukey.txt'
# build path
BUILD_PATH = '../fw/LinuxUN20'
# path to plink exe
PLINK = "C:\Program Files (x86)\PuTTY\plink.exe"
# path to WinSCP exe
WINSCP = "C:\Program Files (x86)\WinSCP\WinSCP.com"
# PuTTY key file
PPK='id_rsa.ppk'


''' Other variables - be careful about changing '''
# Base URL of interface board manufacturing API
MANU_URL = 'https://simprintsmanufacturingapi.azurewebsites.net/api/InterfacePcbs/' 
# IP address of UN20-EX1
HOST='10.0.1.3'
