#ifndef IAP_H_
#define IAP_H_

enum command_code {
	IAPCommand_Init = 49,
	IAPCommand_Prepare_sector_for_write_operation = 50,
	IAPCommand_Copy_RAM_to_Flash = 51,
	IAPCommand_Erase_sector = 52,
	IAPCommand_Blank_check_sector = 53,
	IAPCommand_Read_part_ID = 54,
	IAPCommand_Read_Boot_Code_version = 55,
	IAPCommand_Compare = 56,
	IAPCommand_Reinvoke_ISP = 57,
	IAPCommand_Read_device_serial_number = 58
};

enum error_code
{
	CMD_SUCCESS,
	INVALID_COMMAND,
	SRC_ADDR_ERROR,
	DST_ADDR_ERROR,
	SRC_ADDR_NOT_MAPPED,
	DST_ADDR_NOT_MAPPED,
	COUNT_ERROR,
	INVALID_SECTOR,
	SECTOR_NOT_BLANK,
	SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION,
	COMPARE_ERROR,
	BUSY,
	PARAM_ERROR,
	ADDR_ERROR,
	ADDR_NOT_MAPPED,
	CMD_LOCKED,
	INVALID_CODE,
	INVALID_BAUD_RATE,
	INVALID_STOP_BIT,
	CODE_READ_PROTECTION_ENABLED,
        INVALID_FLASH_UNIT,
        USER_CODE_CHECKSUM,
        ERROR_SETTING_ACTIVE_PARTITION
};

#define     FLASH_SECTOR_0       0x00000000
#define     FLASH_SECTOR_1       0x00002000
#define     FLASH_SECTOR_2       0x00004000
#define     FLASH_SECTOR_3       0x00006000
#define     FLASH_SECTOR_4       0x00008000
#define     FLASH_SECTOR_5       0x0000A000
#define     FLASH_SECTOR_6       0x0000C000
#define     FLASH_SECTOR_7       0x0000E000
#define     FLASH_SECTOR_8       0x00010000
#define     FLASH_SECTOR_9       0x00020000
#define     FLASH_SECTOR_10      0x00030000
#define     FLASH_SECTOR_11      0x00040000
#define     FLASH_SECTOR_12      0x00050000
#define     FLASH_SECTOR_13      0x00060000
#define     FLASH_SECTOR_14      0x00070000

#define     FLASH_BANK_A         0x1A000000
#define     FLASH_BANK_B         0x1B000000
#define     FLASH_BANK_A_ID      0
#define     FLASH_BANK_B_ID      1

#define     FLASH_ADDR(bank, sector)  ((bank) + (sector))

#define     FLASH_SECTOR_SIZE_0_TO_7    ( 8 * 1024)
#define     FLASH_SECTOR_SIZE_8_TO_14   (64 * 1024)

typedef struct {
	unsigned int ser1; // high dword of serial number
	unsigned int ser2;
	unsigned int ser3;
	unsigned int ser4; // low /-/
} DeviceSerial;

unsigned int iap_init();
unsigned int iap_read_serial(DeviceSerial *d);
unsigned int iap_read_id(unsigned int *d);
unsigned int iap_write_prepare(unsigned int start, unsigned int end, unsigned int bank);
unsigned int iap_write(const char *src, const char *dst, int size);
unsigned int iap_blank_check(unsigned int start, unsigned int end, unsigned int bank);
unsigned int iap_erase(unsigned int start, unsigned int end, unsigned int bank);
unsigned int iap_compare(const char *src, const char *dst, int size);
void iap_reinvoke_isp(void);

#endif /* IAP_H_ */
