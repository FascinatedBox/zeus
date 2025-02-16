#include "widgets/virtualdevicecombobox.h"
#include "core/pulsedata.h"

#define IS_NOT_VIRTUAL_SINK(info) (info->flags & PA_SOURCE_HARDWARE)

ZeusVirtualDeviceComboBox::ZeusVirtualDeviceComboBox(ZeusPulseData *pd)
    : ZeusBaseDeviceComboBox() {
  ZEUS_PULSE_CONNECT_LOAD(pd, this, ZeusVirtualDeviceComboBox, sink, Sink);
}

void ZeusVirtualDeviceComboBox::onSinkAdded(ZeusPulseDeviceInfo *info) {
  if (IS_NOT_VIRTUAL_SINK(info))
    return;

  addDevice(info);
}

void ZeusVirtualDeviceComboBox::onSinkRemoved(uint32_t index) {
  removeDevice(index);
}
