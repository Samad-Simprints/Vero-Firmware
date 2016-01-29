
#ifndef __USBCDC_H_INCLUDED__
#define __USBCDC_H_INCLUDED__

typedef void (*usbcdc_transfer_callback_t)(int len);

void usbcdc_init(void);
void usbcdc_transmit(const char *data, unsigned int len,
					 usbcdc_transfer_callback_t calback);


#endif // __USBCDC_H_INCLUDED__
