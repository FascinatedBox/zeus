#include "devicecombobox.h"
#include <QWheelEvent>

ZeusDeviceComboBox::ZeusDeviceComboBox(void) : QComboBox() {}

#define NAME_ROLE (Qt::UserRole + 1)

void ZeusDeviceComboBox::addDevice(uint32_t index, QString name, QString desc) {
  addItem(desc, index);
  setItemData(count() - 1, name, NAME_ROLE);
}

QString ZeusDeviceComboBox::currentDeviceName(void) {
  return itemData(currentIndex(), NAME_ROLE).toString();
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

void ZeusDeviceComboBox::setCurrentDeviceByIndex(uint32_t index) {
  for (int i = 0; i < count(); i++) {
    uint32_t comboDeviceIndex = itemData(i).toUInt();

    if (index == comboDeviceIndex) {
      setCurrentIndex(i);
      break;
    }
  }
}

void ZeusDeviceComboBox::wheelEvent(QWheelEvent *event) {
  // Only change the target if the user explicitly selects a different one.
  // This prevents accidentally changing targets when the user is scrolling but
  // just so happens to have the mouse wheel over this widget.
  event->ignore();
}
