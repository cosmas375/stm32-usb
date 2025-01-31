#include <stdint.h>

#include "usb.h"

#ifndef USBD_DRIVER_H_
#define USBD_DRIVER_H_

typedef uint8_t EndpointNumber;
typedef UsbEndpointType UsbEndpointType;

typedef enum {
  USB_MODE_HOST,
  USB_MODE_DEVICE,
} UsbMode;

void usb_driver_init(void);

void usb_driver_poll(void);

void usb_driver_set_mode(UsbMode mode);

void usb_driver_power_on(void);

void usb_driver_power_off(void);

void usb_driver_connect(void);

void usb_driver_disconnect(void);

void usb_driver_device_set_address(uint8_t address);

void usb_driver_activate_out_endpoint(EndpointNumber endpoint_number,
                                  uint8_t max_packet_size,
                                  UsbEndpointType endpoint_type);

void usb_driver_deactivate_out_endpoint(EndpointNumber endpoint_number);

void usb_driver_activate_in_endpoint(EndpointNumber endpoint_number,
                                  uint8_t max_packet_size,
                                  UsbEndpointType endpoint_type);

void usb_driver_deactivate_in_endpoint(EndpointNumber endpoint_number);

uint8_t usb_driver_write_packet(EndpointNumber endpoint_number, void const *buffer,
                            uint16_t size_byte);

void usb_driver_read_packet(EndpointNumber endpoint_number, void const *buffer,
                        uint16_t size);

__attribute__((weak)) void usb_driver_delay(uint32_t ms);

// hooks
__attribute__((weak)) void on_usb_polled(void);

__attribute__((weak)) void on_usb_reset_received(void);

__attribute__((weak)) void on_usb_setup_data_packet_received(EndpointNumber endpoint_number, uint16_t bytes_count);

__attribute__((weak)) void on_usb_setup_stage_done(EndpointNumber endpoint_number);

__attribute__((weak)) void on_usb_out_data_packet_received(EndpointNumber endpoint_number, uint16_t bytes_count);

__attribute__((weak)) void on_usb_out_transfer_completed(EndpointNumber endpoint_number);

__attribute__((weak)) void on_usb_in_transfer_completed(EndpointNumber endpoint_number);

__attribute__((weak)) void on_usb_tx_fifo_empty(EndpointNumber endpoint_number);

#endif /* USBD_DRIVER_H_ */
