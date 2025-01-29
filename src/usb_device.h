#include <stdint.h>

#ifndef USB_DEVICE_H_
#define USB_DEVICE_H_

// TODO: doc

typedef enum {
    USB_DEVICE_STATE_ATTACHED,
    USB_DEVICE_STATE_POWERED,
    USB_DEVICE_STATE_DEFAULT,
    USB_DEVICE_STATE_ADDRESSED,
    USB_DEVICE_STATE_CONFIGURED,
    USB_DEVICE_STATE_SUSPENDED,
} UsbDeviceState;

typedef enum {
    USB_CONTROL_TRANSFER_STAGE_SETUP,           // 0
    USB_CONTROL_TRANSFER_STAGE_DATA_IN,         // 1
    USB_CONTROL_TRANSFER_STAGE_DATA_IN_IDLE,    // 2
    USB_CONTROL_TRANSFER_STAGE_DATA_IN_ZERO,    // 3
    USB_CONTROL_TRANSFER_STAGE_DATA_OUT,        // 4
    USB_CONTROL_TRANSFER_STAGE_STATUS_IN,       // 5
    USB_CONTROL_TRANSFER_STAGE_STATUS_OUT,      // 6
} UsbDeviceControlTransferStage;

typedef void const * UsbDeviceBuffer;

typedef uint32_t UsbDeviceBufferSize;

typedef struct {
    UsbDeviceState state;

    UsbDeviceControlTransferStage control_transfer_stage;

    UsbDeviceBuffer in_buffer;
    UsbDeviceBufferSize in_data_size;

    UsbDeviceBuffer out_buffer;
    UsbDeviceBufferSize out_data_size;
} UsbDevice;

void usb_device_init(UsbDevice *usb_device, UsbDeviceBuffer out_buffer);

void usb_device_reset(UsbDevice *usb_device);

void usb_device_set_in_data(UsbDevice *usb_device, UsbDeviceBuffer buffer, UsbDeviceBufferSize size);

UsbDeviceBuffer usb_device_get_in_data_buffer(UsbDevice *usb_device);

UsbDeviceBufferSize usb_device_get_in_data_size(UsbDevice *usb_device);

void usb_device_set_out_data(UsbDevice *usb_device, UsbDeviceBuffer buffer,UsbDeviceBufferSize size);

UsbDeviceBuffer usb_device_get_out_data_buffer(UsbDevice *usb_device);

UsbDeviceBufferSize usb_device_get_out_data_size(UsbDevice *usb_device);

void
usb_device_set_control_transfer_stage(UsbDevice *usb_device,
                           UsbDeviceControlTransferStage control_transfer_stage);

UsbDeviceControlTransferStage
usb_device_get_control_transfer_stage(UsbDevice *usb_device);

void usb_device_set_state(UsbDevice *usb_device,
                                 UsbDeviceState device_state);
UsbDeviceState usb_device_get_state(UsbDevice *usb_device);

#endif /* USB_DEVICE_H_ */
