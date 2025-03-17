#include "widgets/devicecombobox.h"
#include <QWheelEvent>

ZeusDeviceComboBox::ZeusDeviceComboBox(ZeusPulseData *pd,
                                       ZeusPulseInfoType type)
    : ZeusBasePulseItemComboBox() {
  if (type == ZISink)
    ZEUS_PULSE_CONNECT_LOAD(pd, this, ZeusDeviceComboBox, sink, Sink);
  else if (type == ZISource)
    ZEUS_PULSE_CONNECT_LOAD(pd, this, ZeusDeviceComboBox, source, Source);
}

void ZeusDeviceComboBox::onSinkAdded(ZeusPulseDeviceInfo *info) {
  addDevice(info);
}

void ZeusDeviceComboBox::onSinkRemoved(uint32_t index) { removeDevice(index); }

void ZeusDeviceComboBox::onSourceAdded(ZeusPulseDeviceInfo *info) {
  addDevice(info);
}

void ZeusDeviceComboBox::onSourceRemoved(uint32_t index) {
  removeDevice(index);
}
