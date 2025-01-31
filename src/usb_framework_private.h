#include "usb.h"
#include "usb_hid.h"
#include "usb_driver.h"
#include "usb_device.h"

#ifndef USBD_FRAMEWORK_PRIVATE_H_
#define USBD_FRAMEWORK_PRIVATE_H_

typedef struct {
    UsbConfigurationDescriptor usb_configuration_descriptor;
    UsbInterfaceDescriptor usb_interface_descriptor;
    UsbHidDescriptor usb_hid_descriptor;
    UsbEndpointDescriptor usb_endpoint_descriptor;
} __attribute__((packed)) UsbDescriptorsComposition;

static UsbDevice *usb_device_instance;

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

static void write_mouse_report(void);

static void process_snandard_device_request(void);
static void process_standard_interface_request(void);
static void process_class_interface_request(void);

static void process_standard_device_get_descriptor_request(void);

static void process_control_transfer_stage(void);

#endif
