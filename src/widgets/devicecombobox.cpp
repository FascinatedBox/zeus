#include "devicecombobox.h"

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
