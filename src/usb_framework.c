#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <wchar.h>

#include "usb.h"
#include "usb_hid.h"

#include "usb_framework.h"
#include "usb_framework_private.h"

#ifdef USB_DEBUG
#include "swo_logger.h"
#endif

// Public API

void usb_framework_init(UsbDevice *usb_device) {
#ifdef USB_DEBUG
  // swo_logger_log_debug("f: usb_init");
#endif

  usb_device_instance = usb_device;

  usb_driver_init();

  usb_driver_set_mode(USB_MODE_DEVICE);

  usb_driver_power_on();

  usb_driver_connect();
}

void usb_framework_poll(void) {
#ifdef USB_DEBUG
  // swo_logger_log_debug("f: usb_poll");
#endif

  usb_driver_poll();
}

// Driver hooks

void usb_driver_delay(uint32_t ms) {
#ifdef USB_DEBUG
  // swo_logger_log_debug("f: usb_driver_delay");
#endif

  usb_framework_delay(ms);
}

void on_usb_polled(void) {
#ifdef USB_DEBUG
  // swo_logger_log_debug("f: on_usb_polled");
#endif

  process_control_transfer_stage();
}

void on_usb_reset_received(void) {
#ifdef USB_DEBUG
  // swo_logger_log_debug("f: usb_reset_received_handler");
#endif

  usb_device_reset(usb_device_instance);

  usb_driver_device_set_address(0);
}

void on_usb_setup_data_packet_received(uint8_t endpoint_number,
                                       uint16_t byte_count) {
#ifdef USB_DEBUG
  // swo_logger_log_debug("f: on_usb_setup_data_packet_received(endpoint_number
  // "
  //                      "= %d, byte_count = %d)",
  //                      endpoint_number, byte_count);
#endif

  usb_driver_read_packet(endpoint_number,
                         usb_device_get_out_data_buffer(usb_device_instance),
                         byte_count);
}

void on_usb_setup_stage_done(uint8_t endpoint_number) {
#ifdef USB_DEBUG
  // swo_logger_log_debug("f: on_usb_setup_stage_done(endpoint_number = %d)",
  //                      endpoint_number);
#endif

  const UsbDeviceReqest *request =
      usb_device_get_out_data_buffer(usb_device_instance);

#ifdef USB_DEBUG
  // swo_logger_log_debug("request->bmRequestType = %x",
  // request->bmRequestType);
#endif

  switch (request->bmRequestType &
          (USB_DEVICE_REQUEST_TYPE_TYPE | USB_DEVICE_REQUEST_TYPE_RECIPIENT)) {
  case USB_DEVICE_REQUEST_TYPE_TYPE_STANDARD |
      USB_DEVICE_REQUEST_TYPE_RECIPIENT_DEVICE:
    process_snandard_device_request();
    break;
  case USB_DEVICE_REQUEST_TYPE_TYPE_STANDARD |
      USB_DEVICE_REQUEST_TYPE_RECIPIENT_INTERFACE:
    process_standard_interface_request();
    break;
  case USB_DEVICE_REQUEST_TYPE_TYPE_CLASS |
      USB_DEVICE_REQUEST_TYPE_RECIPIENT_INTERFACE:
    process_class_interface_request();
    break;
  }
}

void on_usb_out_data_packet_received(uint8_t endpoint_number,
                                     uint16_t byte_count) {
#ifdef USB_DEBUG
  // swo_logger_log_debug("f: on_out_data_packet_received(endpoint_number "
  //                      "= %d, byte_count = %d)",
  //                      endpoint_number, byte_count);
#endif

  usb_driver_read_packet(endpoint_number,
                         usb_device_get_out_data_buffer(usb_device_instance),
                         byte_count);
}

void on_usb_out_transfer_completed(uint8_t endpoint_number) {
#ifdef USB_DEBUG
  // swo_logger_log_debug(
  //     "f: on_usb_out_transfer_completed(endpoint_number = %d)",
  //     endpoint_number);
#endif

  usb_device_set_control_transfer_stage(usb_device_instance,
                                        USB_CONTROL_TRANSFER_STAGE_SETUP);
}

void on_usb_in_transfer_completed(uint8_t endpoint_number) {
#ifdef USB_DEBUG
  // swo_logger_log_debug("f: on_usb_in_transfer_completed(endpoint_number =
  // %d)",
  //                      endpoint_number);
#endif

  if (usb_device_get_state(usb_device_instance) ==
          USB_DEVICE_STATE_CONFIGURED &&
      endpoint_number ==
          (usb_framework_get_endpoint_descriptor()->bEndpointAddress & 0xF)) {
    write_mouse_report();
  }
}

void on_usb_tx_fifo_empty(EndpointNumber endpoint_number) {
#ifdef USB_DEBUG
  // swo_logger_log_debug("f: on_usb_tx_fifo_empty(endpoint_number = %d)",
  //                      endpoint_number);
#endif

  if (usb_device_get_control_transfer_stage(usb_device_instance) ==
          USB_CONTROL_TRANSFER_STAGE_DATA_IN_IDLE &&
      usb_device_get_in_data_size(usb_device_instance) > 0) {
    usb_device_set_control_transfer_stage(usb_device_instance,
                                          USB_CONTROL_TRANSFER_STAGE_DATA_IN);
  }
}

// Private API

static void process_snandard_device_request(void) {
#ifdef USB_DEBUG
  // swo_logger_log_debug("f: process_standard_device_request");
#endif

  const UsbDeviceReqest *request =
      usb_device_get_out_data_buffer(usb_device_instance);

#ifdef USB_DEBUG
  // swo_logger_log_debug("request = %x", *request);
  // swo_logger_log_debug("request->bRequest = %x", request->bRequest);
#endif

  switch (request->bRequest) {
  case USB_DEVICE_REQUEST_GET_DESCRIPTOR:
#ifdef USB_DEBUG
    // swo_logger_log_debug("USB_DEVICE_REQUEST_GET_DESCRIPTOR");
#endif

    process_standard_device_get_descriptor_request();
    break;
  case USB_DEVICE_REQUEST_SET_ADDRESS:;
#ifdef USB_DEBUG
  // swo_logger_log_debug("USB_DEVICE_REQUEST_SET_ADDRESS");
#endif

    const uint16_t device_address = request->wValue;
    usb_driver_device_set_address(device_address);

    usb_device_set_state(usb_device_instance, USB_DEVICE_STATE_ADDRESSED);

    usb_device_set_control_transfer_stage(usb_device_instance,
                                          USB_CONTROL_TRANSFER_STAGE_STATUS_IN);
    break;
  case USB_DEVICE_REQUEST_SET_CONFIGURATION:
#ifdef USB_DEBUG
    // swo_logger_log_debug("USB_DEVICE_REQUEST_SET_CONFIGURATION");
#endif

    usb_driver_activate_in_endpoint(
        usb_framework_get_endpoint_descriptor()->bEndpointAddress & 0b1111,
        usb_framework_get_endpoint_descriptor()->wMaxPacketSize,
        usb_framework_get_endpoint_descriptor()->bmAttributes & 0b11);

    usb_driver_write_packet(
        usb_framework_get_endpoint_descriptor()->bEndpointAddress & 0xF, NULL,
        0);

    usb_device_set_state(usb_device_instance, USB_DEVICE_STATE_CONFIGURED);

    usb_device_set_control_transfer_stage(usb_device_instance,
                                          USB_CONTROL_TRANSFER_STAGE_STATUS_IN);
    break;
  }
}

static void process_standard_interface_request(void) {
#ifdef USB_DEBUG
  // swo_logger_log_debug("f: process_standard_interface_request");
#endif

  const UsbDeviceReqest *request =
      usb_device_get_out_data_buffer(usb_device_instance);

  const uint8_t descriptor_type = request->wValue >> 8;
  const uint16_t descriptor_length = request->wLength;
#ifdef USB_DEBUG
  // swo_logger_log_debug("descriptor_type = %x, descriptor_length = %d",
  //                      descriptor_type, descriptor_length);
#endif

  switch (descriptor_type) {
  case USB_DESCRIPTOR_TYPE_HID_REPORT:;
#ifdef USB_DEBUG
  // swo_logger_log_debug("USB_DESCRIPTOR_TYPE_HID_REPORT");
#endif

    UsbDTO const *hid_report_descriptor_dto =
        usb_framework_get_hid_report_descriptor();
    usb_device_set_in_data(
        usb_device_instance, hid_report_descriptor_dto->data,
        MIN(descriptor_length, hid_report_descriptor_dto->size));

    usb_device_set_control_transfer_stage(usb_device_instance,
                                          USB_CONTROL_TRANSFER_STAGE_DATA_IN);
    break;
  }
}

static void process_class_interface_request(void) {
#ifdef USB_DEBUG
  // swo_logger_log_debug("f: process_class_interface_request");
#endif

  const UsbDeviceReqest *request =
      usb_device_get_out_data_buffer(usb_device_instance);

  switch (request->bRequest) {
  case USB_HID_SET_IDLE:
#ifdef USB_DEBUG
    // swo_logger_log_debug("USB_HID_SET_IDLE");
#endif

    usb_device_set_control_transfer_stage(usb_device_instance,
                                          USB_CONTROL_TRANSFER_STAGE_STATUS_IN);
    break;
  }
}

static UsbDescriptorsComposition descriptors_composition;
static void process_standard_device_get_descriptor_request(void) {
#ifdef USB_DEBUG
  // swo_logger_log_debug("f: process_standard_device_get_descriptor_request");
#endif

  const UsbDeviceReqest *request =
      usb_device_get_out_data_buffer(usb_device_instance);

  const uint8_t descriptor_type = request->wValue >> 8;
  const uint16_t requested_descriptor_length = request->wLength;
#ifdef USB_DEBUG
  // swo_logger_log_debug("descriptor_type = %x, descriptor_length = %d",
  //                      descriptor_type, requested_descriptor_length);
#endif

  switch (descriptor_type) {
  case USB_DESCRIPTOR_TYPE_DEVICE:;
#ifdef USB_DEBUG
  // swo_logger_log_debug("USB_DESCRIPTOR_TYPE_DEVICE");
#endif

    UsbDeviceDescriptor const *device_descriptor =
        usb_framework_get_device_descriptor();
    usb_device_set_in_data(
        usb_device_instance, device_descriptor,
        MIN(requested_descriptor_length, device_descriptor->bLength));

    usb_device_set_control_transfer_stage(usb_device_instance,
                                          USB_CONTROL_TRANSFER_STAGE_DATA_IN);
    break;
  case USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER:;
#ifdef USB_DEBUG
  // swo_logger_log_debug("USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER");
#endif

    UsbDeviceQualifier const *device_qualifier =
        usb_framework_get_device_qualifier();
    usb_device_set_in_data(
        usb_device_instance, device_qualifier,
        MIN(requested_descriptor_length, device_qualifier->bLength));

    usb_device_set_control_transfer_stage(usb_device_instance,
                                          USB_CONTROL_TRANSFER_STAGE_DATA_IN);
    break;
  case USB_DESCRIPTOR_TYPE_CONFIGURATION:;
#ifdef USB_DEBUG
  // swo_logger_log_debug("USB_DESCRIPTOR_TYPE_CONFIGURATION");
#endif

    descriptors_composition.usb_configuration_descriptor =
        *(usb_framework_get_configuration_descriptor());
    descriptors_composition.usb_interface_descriptor =
        *(usb_framework_get_interface_descriptor());
    descriptors_composition.usb_hid_descriptor =
        *(usb_framework_get_hid_descriptor());
    descriptors_composition.usb_endpoint_descriptor =
        *(usb_framework_get_endpoint_descriptor());

    usb_device_set_in_data(
        usb_device_instance, &descriptors_composition,
        MIN(requested_descriptor_length,
            descriptors_composition.usb_configuration_descriptor.wTotalLength));

    usb_device_set_control_transfer_stage(usb_device_instance,
                                          USB_CONTROL_TRANSFER_STAGE_DATA_IN);
    break;
  }
}

static void process_control_transfer_stage(void) {
  uint8_t const endpoint_number = 0;

  switch (usb_device_get_control_transfer_stage(usb_device_instance)) {
  case USB_CONTROL_TRANSFER_STAGE_SETUP:
    break;
  case USB_CONTROL_TRANSFER_STAGE_DATA_IN:;
#ifdef USB_DEBUG
  // swo_logger_log_debug("USB_CONTROL_TRANSFER_STAGE_DATA_IN");
#endif

    uint8_t data_size = usb_device_get_in_data_size(usb_device_instance);

#ifdef USB_DEBUG
    // swo_logger_log_debug("f: data_size = %d", data_size);
#endif

    uint8_t byte_written = usb_driver_write_packet(
        endpoint_number, usb_device_get_in_data_buffer(usb_device_instance),
        data_size);

    // overflow prevention
    uint8_t in_data_size;
    if (usb_device_get_in_data_size(usb_device_instance) >= byte_written) {
      in_data_size =
          usb_device_get_in_data_size(usb_device_instance) - byte_written;
    } else {
      in_data_size = 0;
    }

    usb_device_set_in_data(usb_device_instance,
                           usb_device_get_in_data_buffer(usb_device_instance) +
                               byte_written,
                           in_data_size);

#ifdef USB_DEBUG
    // swo_logger_log_debug("f: byte_written = %d, in_data_size = %d",
    //                      byte_written,
    //                      usb_device_get_in_data_size(usb_device_instance));
#endif

    usb_device_set_control_transfer_stage(
        usb_device_instance, USB_CONTROL_TRANSFER_STAGE_DATA_IN_IDLE);

    if (usb_device_instance->in_data_size <= 0) {
      if (byte_written ==
          usb_framework_get_device_descriptor()->bMaxPacketSize0) {
        usb_device_set_control_transfer_stage(
            usb_device_instance, USB_CONTROL_TRANSFER_STAGE_DATA_IN_ZERO);
      } else {
        usb_device_set_control_transfer_stage(
            usb_device_instance, USB_CONTROL_TRANSFER_STAGE_STATUS_OUT);
      }
    }
    break;
  case USB_CONTROL_TRANSFER_STAGE_DATA_IN_IDLE:
    break;
  case USB_CONTROL_TRANSFER_STAGE_DATA_IN_ZERO:
    usb_driver_write_packet(1, NULL, 0);
    usb_device_set_control_transfer_stage(
        usb_device_instance, USB_CONTROL_TRANSFER_STAGE_STATUS_OUT);
    break;
  case USB_CONTROL_TRANSFER_STAGE_DATA_OUT:
    break;
  case USB_CONTROL_TRANSFER_STAGE_STATUS_OUT:
    // usb_device_set_control_transfer_stage(usb_device_instance,
    //                                       USB_CONTROL_TRANSFER_STAGE_SETUP);
    break;
  case USB_CONTROL_TRANSFER_STAGE_STATUS_IN:
    usb_driver_write_packet(endpoint_number, NULL, 0);
    usb_device_set_control_transfer_stage(usb_device_instance,
                                          USB_CONTROL_TRANSFER_STAGE_SETUP);
    break;
  }
}

static void write_mouse_report(void) {
#ifdef USB_DEBUG
  // swo_logger_log_debug("f: write_mouse_report");
#endif

  UsbDTO const *reportDTO = usb_framework_get_hid_report();

  usb_driver_write_packet(
      usb_framework_get_endpoint_descriptor()->bEndpointAddress & 0xF,
      reportDTO->data, reportDTO->size);
}
