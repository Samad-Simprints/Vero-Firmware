#include <stdint.h>
#include "system_LPC18xx.h"
#include "flash_iap.h"

#if 0
#define IAP_LOCATION 0x10400100

typedef void (*IAP)(unsigned int [],unsigned int []);

IAP iap_entry;
#endif
#define IAP_LOCATION  *(volatile unsigned int *)0x10400100 
#define iap_entry(a, b)     ((void (*)())(IAP_LOCATION))(a, b)

/* Intialise the IAP system
 * return error_code */
unsigned int iap_init()
{
	unsigned int command[5]={0,0,0,0,0};
	unsigned int result[5]={0,0,0,0,0};

	//iap_entry=(IAP)IAP_LOCATION;
	command[0]=IAPCommand_Init;
	iap_entry(command,result);
	return result[0];
}


/* Read chip's serial number into *d struct
 * Return error_code */
unsigned int iap_read_serial(DeviceSerial *d)
{
	unsigned int command[5]={0,0,0,0,0};
	unsigned int result[5]={0,0,0,0,0};

	//iap_entry=(IAP)IAP_LOCATION;
	command[0]=IAPCommand_Read_device_serial_number;
	iap_entry(command,result);
	d->ser1=result[1];
	d->ser2=result[2];
	d->ser3=result[3];
	d->ser4=result[4];
	return result[0];
}

/* Read chip's ID into *d integer
 *  Return: error_code
 */
unsigned int iap_read_id(unsigned int *d)
{
	unsigned int command[5]={0,0,0,0,0};
	unsigned int result[5]={0,0,0,0,0};

	//iap_entry=(IAP)IAP_LOCATION;
	command[0]=IAPCommand_Read_part_ID;
	iap_entry(command,result);
	*d=result[1];
	return result[0];
}

/* Prepare specified sector(s) number(s) from start to end to write/erase operations
 * start <= end
 * Return: error_code
*/
unsigned int iap_write_prepare(unsigned int start, unsigned int end, unsigned int bank)
{
	unsigned int command[5]={0,0,0,0,0};
	unsigned int result[5]={0,0,0,0,0};

	//iap_entry=(IAP)IAP_LOCATION;
	command[0]= IAPCommand_Prepare_sector_for_write_operation;
	command[1]= start;
	command[2]= end;
        command[3]= bank;
	iap_entry(command,result);
	return result[0];
}

/* Write data from src (RAM) to dst (FLASH). dst should be absolute address
 * i.e. sector_start_adress[16] : write to 16 sector with 0 offset
 * sector_start_adress[16] + 256 : write to 16 sector with 256 bytes offset
 * offset must be  512 | 1024 | 4096
 * Sector must be prepared to write by iap_write_prepare() function
 * Return: error_code
 */
unsigned int iap_write(const char *src, const char *dst, int size)
{
	unsigned int command[5]={0,0,0,0,0};
	unsigned int result[5]={0,0,0,0,0};

//iap_entry=(IAP)IAP_LOCATION;
	command[0]= IAPCommand_Copy_RAM_to_Flash;
	command[1]= (unsigned int)dst;    //  Destination flash address where data bytes are to be written. This address should be a 256 byte boundary.
	command[2]= (unsigned int)src;    //  Source RAM address from which data bytes are to be read. This address should be a word boundary.
	command[3]= size;                  //  Number of bytes to be written. Should be 512 | 1024 | 4096.
	command[4]= SystemCoreClock/1000;	// CPU clock in kHz
	iap_entry(command,result);
	return result[0];
}

/* Check if sector(s) from start to end are blank
 * start <= end
 * Return: error_code
 */
unsigned int iap_blank_check(unsigned int start, unsigned int end, unsigned int bank)
{
	unsigned int command[5]={0,0,0,0,0};
	unsigned int result[5]={0,0,0,0,0};

    //iap_entry=(IAP)IAP_LOCATION;
    command[0]= IAPCommand_Blank_check_sector;
    command[1]= start;  //  Start Sector Number
    command[2]= end;    //  End Sector Number (should be greater than or equal to start sector number)
    command[3]= bank;
    iap_entry(command, result);
    return result[0];
}

/* Erase specified sectors(s) from start to end
 * start <= end
 * Return: error_code
 */
unsigned int iap_erase(unsigned int start, unsigned int end, unsigned int bank)
{
	unsigned int command[5]={0,0,0,0,0};
	unsigned int result[5]={0,0,0,0,0};

	//iap_entry=(IAP)IAP_LOCATION;
    command[0]= IAPCommand_Erase_sector;
    command[1]= start;  //  Start Sector Number
    command[2]= end;    //  End Sector Number (should be greater than or equal to start sector number)
    command[3]= SystemCoreClock/1000;            //  CPU Clock Frequency (CCLK) in kHz
    command[4]= bank;
    iap_entry(command, result);
    return result[0];
}

/* Compare size data from src with dst
 * Return: error_code
 */
unsigned int iap_compare(const char *src, const char *dst, int size)
{
	unsigned int command[5]={0,0,0,0,0};
	unsigned int result[5]={0,0,0,0,0};

	//iap_entry=(IAP)IAP_LOCATION;
    command[0]= IAPCommand_Compare;
    command[1]= (unsigned int)dst;    //  Starting flash or RAM address of data bytes to be compared. This address should be a word boundary.
    command[2]= (unsigned int)src;    //  Starting flash or RAM address of data bytes to be compared. This address should be a word boundary.
    command[3]= size;                         //  Number of bytes to be compared; should be a multiple of 4.
    iap_entry(command, result);
    return result[0];
}

/* Enter ISP mode programmatically
 */
void iap_reinvoke_isp(void)
{
	unsigned int command[5]={0,0,0,0,0};
	unsigned int result[5]={0,0,0,0,0};

	//iap_entry=(IAP)IAP_LOCATION;
	command[0]=IAPCommand_Reinvoke_ISP;
	iap_entry(command, result);
}

/* Erase one or more 512 byte pages in a sector
 * To erase one page set start == end
 * Return: error_code
 */
unsigned int iap_erase_page(const char *page_start, const char *page_end)
{
    unsigned int command[5]={0,0,0,0,0};
    unsigned int result[5]={0,0,0,0,0};

    //iap_entry=(IAP)IAP_LOCATION;
    command[0]= IAPCommand_Erase_page;
    command[1]= (unsigned int)page_start;    //  Starting flash address of first page in sector to erase
    command[2]= (unsigned int)page_end;      //  Ending flash address of last page in sector to erase
    command[3]= SystemCoreClock/1000;       //  CPU Clock Frequency (CCLK) in kHz
    iap_entry(command, result);
    return result[0];
}

/* Specify which bank to boot code out of
 */
void iap_set_boot_bank(unsigned int bank)
{
    unsigned int command[5]={0,0,0,0,0};
    unsigned int result[5]={0,0,0,0,0};

    //iap_entry=(IAP)IAP_LOCATION;
    command[0]= IAPCommand_Set_boot_bank;
    command[1]= (unsigned int)bank;
    command[2]= SystemCoreClock/1000;   //  CPU Clock Frequency (CCLK) in kHz
    iap_entry(command, result);
}
