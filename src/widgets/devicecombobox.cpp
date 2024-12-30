#include "widgets/devicecombobox.h"
#include <QWheelEvent>

#define NAME_ROLE (Qt::UserRole + 1)

ZeusDeviceComboBox::ZeusDeviceComboBox(ZeusPulseData *pd,
                                       ZeusPulseInfoType type)
    : QComboBox() {
  if (type == ZISink)
    ZEUS_PULSE_CONNECT_LOAD(pd, this, ZeusDeviceComboBox, sink, Sink);
  else if (type == ZISource)
    ZEUS_PULSE_CONNECT_LOAD(pd, this, ZeusDeviceComboBox, source, Source);
}

QString ZeusDeviceComboBox::currentDeviceName(void) {
  return itemData(currentIndex(), NAME_ROLE).toString();
}

void ZeusDeviceComboBox::loadInfo(ZeusPulseStreamInfo *info) {
  for (int i = 0; i < count(); i++) {
    uint32_t comboDeviceIndex = itemData(i).toUInt();

    if (info->target == comboDeviceIndex) {
      setCurrentIndex(i);
      break;
    }
  }
}

void ZeusDeviceComboBox::removeDevice(uint32_t index) {
  for (int i = 0; i < count(); i++) {
    uint32_t comboDeviceIndex = itemData(i).toUInt();

    if (index == comboDeviceIndex) {
      removeItem(i);
      break;
    }
  }
}

void ZeusDeviceComboBox::onSinkAdded(ZeusPulseDeviceInfo *info) {
  addItem(info->desc, info->index);
  setItemData(count() - 1, info->name, NAME_ROLE);
}

void ZeusDeviceComboBox::onSinkRemoved(uint32_t index) { removeDevice(index); }

void ZeusDeviceComboBox::onSourceAdded(ZeusPulseDeviceInfo *info) {
  addItem(info->desc, info->index);
  setItemData(count() - 1, info->name, NAME_ROLE);
}

void ZeusDeviceComboBox::onSourceRemoved(uint32_t index) {
  removeDevice(index);
}

void ZeusDeviceComboBox::wheelEvent(QWheelEvent *event) {
  // Only change the target if the user explicitly selects a different one.
  // This prevents accidentally changing targets when the user is scrolling but
  // just so happens to have the mouse wheel over this widget.
  event->ignore();
}
