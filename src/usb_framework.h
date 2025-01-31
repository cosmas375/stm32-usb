#include "usb_device.h"

#include "usb.h"
#include "usb_hid.h"

#ifndef USBD_FRAMEWORK_H_
#define USBD_FRAMEWORK_H_

void usb_framework_init(UsbDevice *usb_device);

void usb_framework_poll(void);

__attribute__((weak)) UsbDeviceDescriptor const *usb_framework_get_device_descriptor(void);

__attribute__((weak)) UsbDeviceQualifier const *usb_framework_get_device_qualifier(void);

__attribute__((weak)) UsbConfigurationDescriptor const *usb_framework_get_configuration_descriptor(void);

__attribute__((weak)) UsbInterfaceDescriptor const *usb_framework_get_interface_descriptor(void);

__attribute__((weak)) UsbEndpointDescriptor const *usb_framework_get_endpoint_descriptor(void);

__attribute__((weak)) UsbHidDescriptor const *usb_framework_get_hid_descriptor(void);

__attribute__((weak)) UsbDTO const *usb_framework_get_hid_report_descriptor(void);

__attribute__((weak)) UsbDTO const *usb_framework_get_hid_report(void);

__attribute__((weak)) void usb_framework_delay(uint32_t ms);

#endif
