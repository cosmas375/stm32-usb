#include "usb_device.h"

void usb_device_init(UsbDevice *usb_device, UsbDeviceBuffer out_buffer) {
  usb_device->out_buffer = out_buffer;
}

void usb_device_reset(UsbDevice *usb_device) {
  usb_device->out_data_size = 0;

  usb_device->in_data_size = 0;

  usb_device_set_state(usb_device, USB_DEVICE_STATE_DEFAULT);

  usb_device_set_control_transfer_stage(usb_device,
                                        USB_CONTROL_TRANSFER_STAGE_SETUP);
}

void usb_device_set_in_data(UsbDevice *usb_device, UsbDeviceBuffer buffer,
                            uint32_t size) {
  // TODO: assertions

  usb_device->in_buffer = buffer;
  usb_device->in_data_size = size;

  // TODO: return exit code
}

UsbDeviceBuffer usb_device_get_in_data_buffer(UsbDevice *usb_device) {
  return usb_device->in_buffer;
}

uint32_t usb_device_get_in_data_size(UsbDevice *usb_device) {
  return usb_device->in_data_size;
}

void usb_device_set_out_data(UsbDevice *usb_device, UsbDeviceBuffer buffer,
                             uint32_t size) {
  // TODO: assertions

  usb_device->out_buffer = buffer;
  usb_device->out_data_size = size;

  // TODO: return exit code
}

UsbDeviceBuffer usb_device_get_out_data_buffer(UsbDevice *usb_device) {
  return usb_device->out_buffer;
}

uint32_t usb_device_get_out_data_size(UsbDevice *usb_device) {
  return usb_device->out_data_size;
}

void usb_device_set_control_transfer_stage(
    UsbDevice *usb_device,
    UsbDeviceControlTransferStage control_transfer_stage) {
  // TODO: assertions

  usb_device->control_transfer_stage = control_transfer_stage;

  // TODO: return exit code
}

UsbDeviceControlTransferStage
usb_device_get_control_transfer_stage(UsbDevice *usb_device) {
  return usb_device->control_transfer_stage;
}

void usb_device_set_state(UsbDevice *usb_device, UsbDeviceState state) {
  // TODO: assertions

  usb_device->state = state;

  // TODO: return exit code
}

UsbDeviceState usb_device_get_state(UsbDevice *usb_device) {
  return usb_device->state;
}
