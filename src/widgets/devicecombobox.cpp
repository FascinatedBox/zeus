#include "devicecombobox.h"

ZeusDeviceComboBox::ZeusDeviceComboBox(void) : QComboBox() {}

void ZeusDeviceComboBox::addDevice(uint32_t index, QString text) {
  addItem(text, index);
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
