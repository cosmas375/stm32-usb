#include <stdint.h>

#ifndef USB_STANDARDS_H_
#define USB_STANDARDS_H_

typedef enum {
    USB_ENDPOINT_TYPE_CONTROL,
    USB_ENDPOINT_TYPE_ISOCHRONOUS,
    USB_ENDPOINT_TYPE_BULK,
    USB_ENDPOINT_TYPE_INTERRUPT,
} UsbEndpointType;

typedef struct {
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
} UsbDeviceReqest;

typedef struct {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint16_t bcdUSB;
	uint8_t  bDeviceClass;
	uint8_t  bDeviceSubClass;
	uint8_t  bDeviceProtocol;
	uint8_t  bMaxPacketSize0;
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t bcdDevice;
	uint8_t  iManufacturer;
	uint8_t  iProduct;
	uint8_t  iSerialNumber;
	uint8_t  bNumConfigurations;
} __attribute__((__packed__)) UsbDeviceDescriptor;

typedef struct {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint16_t bcdUSB;
	uint8_t  bDeviceClass;
	uint8_t  bDeviceSubClass;
	uint8_t  bDeviceProtocol;
	uint8_t  bMaxPacketSize0;
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t bcdDevice;
	uint8_t  iManufacturer;
	uint8_t  iProduct;
	uint8_t  iSerialNumber;
	uint8_t  bNumConfigurations;
} __attribute__((__packed__)) UsbDeviceQualifier;

typedef struct {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint16_t wTotalLength;
	uint8_t  bNumInterfaces;
	uint8_t  bConfigurationValue;
	uint8_t  iConfiguration;
	uint8_t  bmAttributes;
	uint8_t  bMaxPower;
} __attribute__((__packed__)) UsbConfigurationDescriptor;

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bInterfaceNumber;
	uint8_t bAlternateSetting;
	uint8_t bNumEndpoints;
	uint8_t bInterfaceClass;
	uint8_t bInterfaceSubClass;
	uint8_t bInterfaceProtocol;
	uint8_t iInterface;
} __attribute__((__packed__)) UsbInterfaceDescriptor;

typedef struct {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bEndpointAddress;
	uint8_t  bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t  bInterval;
} __attribute__((__packed__)) UsbEndpointDescriptor;

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t bcdHID;
	uint8_t bCountryCode;
	uint8_t bNumDescriptors;
	uint8_t bDescriptorType0;
	uint16_t wDescriptorLength0;
} __attribute__((__packed__)) UsbHidDescriptor;

typedef struct {
    uint8_t     bDescriptorType;
    uint16_t    wDescriptorLength;
} __attribute__((packed)) UsbHidReportDescriptor;

typedef struct {
    void const *data;
    uint32_t size;
} UsbDTO;

#define USB_DEVICE_REQUEST_TYPE_RECIPIENT_Pos 0
#define USB_DEVICE_REQUEST_TYPE_RECIPIENT_Msk (0b11 << USB_DEVICE_REQUEST_TYPE_RECIPIENT_Pos)
#define USB_DEVICE_REQUEST_TYPE_RECIPIENT USB_DEVICE_REQUEST_TYPE_RECIPIENT_Msk
#define USB_DEVICE_REQUEST_TYPE_RECIPIENT_DEVICE (0 << USB_DEVICE_REQUEST_TYPE_RECIPIENT_Pos)
#define USB_DEVICE_REQUEST_TYPE_RECIPIENT_INTERFACE (1 << USB_DEVICE_REQUEST_TYPE_RECIPIENT_Pos)
#define USB_DEVICE_REQUEST_TYPE_RECIPIENT_ENDPOINT (2 << USB_DEVICE_REQUEST_TYPE_RECIPIENT_Pos)
#define USB_DEVICE_REQUEST_TYPE_RECIPIENT_OTHER (3 << USB_DEVICE_REQUEST_TYPE_RECIPIENT_Pos)

#define USB_DEVICE_REQUEST_TYPE_TYPE_Pos 5
#define USB_DEVICE_REQUEST_TYPE_TYPE_Msk (0b11 << USB_DEVICE_REQUEST_TYPE_TYPE_Pos)
#define USB_DEVICE_REQUEST_TYPE_TYPE USB_DEVICE_REQUEST_TYPE_TYPE_Msk
#define USB_DEVICE_REQUEST_TYPE_TYPE_STANDARD (0 << USB_DEVICE_REQUEST_TYPE_TYPE_Pos)
#define USB_DEVICE_REQUEST_TYPE_TYPE_CLASS (1 << USB_DEVICE_REQUEST_TYPE_TYPE_Pos)
#define USB_DEVICE_REQUEST_TYPE_TYPE_VENDOR (2 << USB_DEVICE_REQUEST_TYPE_TYPE_Pos)

#define USB_DEVICE_REQUEST_TYPE_DIRECTION_Pos 7
#define USB_DEVICE_REQUEST_TYPE_DIRECTION_Msk (0b1 << USB_REQUEST_TYPE_DIRECTION_Pos)
#define USB_DEVICE_REQUEST_TYPE_DIRECTION USB_DEVICE_REQUEST_TYPE_DIRECTION_Msk
#define USB_DEVICE_REQUEST_TYPE_DIRECTION_TO_DEVICE (0 << USB_REQUEST_TYPE_DIRECTION_Pos)
#define USB_DEVICE_REQUEST_TYPE_DIRECTION_TO_HOST (1 << USB_REQUEST_TYPE_DIRECTION_Pos)

#define USB_DEVICE_REQUEST_GET_STATUS 0x00
#define USB_DEVICE_REQUEST_CLEAR_FEATURE 0x01
#define USB_DEVICE_REQUEST_SET_FEATURE 0x03
#define USB_DEVICE_REQUEST_SET_ADDRESS 0x05
#define USB_DEVICE_REQUEST_GET_DESCRIPTOR 0x06
#define USB_DEVICE_REQUEST_SET_DESCRIPTOR 0x07
#define USB_DEVICE_REQUEST_GET_CONFIGURATION 0x08
#define USB_DEVICE_REQUEST_SET_CONFIGURATION 0x09
#define USB_DEVICE_REQUEST_GET_INTERFACE 0x0A
#define USB_DEVICE_REQUEST_SET_INTERFACE 0x0B
#define USB_DEVICE_REQUEST_SYNCH_FRAME 0x0C

#define USB_DESCRIPTOR_TYPE_DEVICE 0x01
#define USB_DESCRIPTOR_TYPE_CONFIGURATION 0x02
#define USB_DESCRIPTOR_TYPE_STRING 0x03
#define USB_DESCRIPTOR_TYPE_INTERFACE 0x04
#define USB_DESCRIPTOR_TYPE_ENDPOINT 0x05
#define USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER 0x06
#define USB_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIGURATION 0x07
#define USB_DESCRIPTOR_TYPE_INTERFACE_POWER 0x08
#define USB_DESCRIPTOR_TYPE_OTG 0x09
#define USB_DESCRIPTOR_TYPE_DEBUG 0x0A
#define USB_DESCRIPTOR_TYPE_INTERFASEASSOC 0x0B
#define USB_DESCRIPTOR_TYPE_HID 0x21
#define USB_DESCRIPTOR_TYPE_HID_REPORT 0x22
#define USB_DESCRIPTOR_TYPE_CS_INTERFACE 0x24
#define USB_DESCRIPTOR_TYPE_CS_ENDPOINT 0x25

#define USB_CLASS_PER_INTERFACE 0x00
#define USB_CLASS_AUDIO 0x01
#define USB_CLASS_PHYSICAL 0x05
#define USB_CLASS_STILL_IMAGE 0x06
#define USB_CLASS_PRINTER 0x07
#define USB_CLASS_MASS_STORAGE 0x08
#define USB_CLASS_HUB 0x09
#define USB_CLASS_CSCID 0x0B
#define USB_CLASS_CONTENT_SEC 0x0D
#define USB_CLASS_VIDEO 0x0E
#define USB_CLASS_HEALTHCARE 0x0F
#define USB_CLASS_AV 0x10
#define USB_CLASS_BILLBOARD 0x11
#define USB_CLASS_CBRIDGE 0x12
#define USB_CLASS_DIAGNOSTIC 0xDC
#define USB_CLASS_WIRELESS 0xE0
#define USB_CLASS_MISC 0xEF
#define USB_CLASS_IAD 0xEF
#define USB_CLASS_APP_SPEC 0xFE
#define USB_CLASS_VENDOR 0xFF
#define USB_CLASS_HID 0x03

#define USB_SUBCLASS_NONE 0x00
#define USB_SUBCLASS_IAD 0x02
#define USB_SUBCLASS_VENDOR 0xFF

#define USB_PROTOCOL_NONE 0x00
#define USB_PROTOCOL_IAD 0x01
#define USB_PROTOCOL_VENDOR 0xFF

#define USB_ENDPOINT_TYPE_CONTROL 0x00
#define USB_ENDPOINT_TYPE_ISOCHRONUS 0x01
#define USB_ENDPOINT_TYPE_BULK 0x02
#define USB_ENDPOINT_TYPE_INTERRUPT 0x03

#endif
