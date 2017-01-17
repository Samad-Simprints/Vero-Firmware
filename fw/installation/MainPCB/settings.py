'''

Global variables and settings

'''

'''
mainpcbFE.py
'''
# name used to save out qr code (only as a backup)
QR_NAME = 'qrcode.BMP'

'''
mainpcb.py
'''
# text file containing key for manufacturing API
MANU_API = 'manukey.txt'
# Base URL of MainPCB manufacturing API
MANU_MAINPCBURL  = 'https://simprintsmanufacturingapi.azurewebsites.net/api/MainPCBs'
# firmware filename
FIRMWARE = '../../build/index THUMB Release/index.bin'

'''
lpchelper.py
'''
# path to LPC script
# nxpPath = 'C:/NXP/LPCScrypt'  # windows
nxpPath = '/Applications/lpcscrypt_1.7.0_696/'  # mac
# Simprints MAC address prefix
simPrintsPrefix = 0xf0acd7c00000

