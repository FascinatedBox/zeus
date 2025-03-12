#include "widgets/basedevicecombobox.h"
#include "core/pulsedata.h"
#include <QWheelEvent>

#define NAME_ROLE (Qt::UserRole + 1)

ZeusBaseDeviceComboBox::ZeusBaseDeviceComboBox(void) : QComboBox() {}

void ZeusBaseDeviceComboBox::addDevice(ZeusPulseDeviceInfo *info) {
  addItem(info->desc, info->index);
  setItemData(count() - 1, info->name, NAME_ROLE);
}

QString ZeusBaseDeviceComboBox::currentDeviceName(void) {
  return itemData(currentIndex(), NAME_ROLE).toString();
}

void ZeusBaseDeviceComboBox::removeDevice(uint32_t index) {
  for (int i = 0; i < count(); i++) {
    uint32_t comboDeviceIndex = itemData(i).toUInt();

    if (index == comboDeviceIndex) {
      removeItem(i);
      break;
    }
  }
}

void ZeusBaseDeviceComboBox::changeDeviceTo(uint32_t index) {
  for (int i = 0; i < count(); i++) {
    uint32_t comboDeviceIndex = itemData(i).toUInt();

    if (index == comboDeviceIndex) {
      setCurrentIndex(i);
      break;
    }
  }
}

void ZeusBaseDeviceComboBox::setCurrentDeviceByName(QString name) {
  for (int i = 0; i < count(); i++) {
    QString n = itemData(i, NAME_ROLE).toString();

    if (name != n)
      continue;

    setCurrentIndex(i);
    break;
  }
}

void ZeusBaseDeviceComboBox::wheelEvent(QWheelEvent *event) {
  // Only change the target if the user explicitly selects a different one.
  // This prevents accidentally changing targets when the user is scrolling but
  // just so happens to have the mouse wheel over this widget.
  event->ignore();
}
