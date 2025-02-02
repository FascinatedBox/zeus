#include "widgets/virtualdevicecombobox.h"
#include "core/pulsedata.h"
#include <QWheelEvent>

#define NAME_ROLE (Qt::UserRole + 1)
#define IS_NOT_VIRTUAL_SINK(info) (info->flags & PA_SOURCE_HARDWARE)

ZeusVirtualDeviceComboBox::ZeusVirtualDeviceComboBox(ZeusPulseData *pd)
    : QComboBox() {
  ZEUS_PULSE_CONNECT_LOAD(pd, this, ZeusVirtualDeviceComboBox, sink, Sink);
}

QString ZeusVirtualDeviceComboBox::currentDeviceName(void) {
  return itemData(currentIndex(), NAME_ROLE).toString();
}

void ZeusVirtualDeviceComboBox::onSinkAdded(ZeusPulseDeviceInfo *info) {
  if (IS_NOT_VIRTUAL_SINK(info))
    return;

  addItem(info->desc, info->index);
  setItemData(count() - 1, info->name, NAME_ROLE);
}

void ZeusVirtualDeviceComboBox::onSinkRemoved(uint32_t index) {
  for (int i = 0; i < count(); i++) {
    uint32_t comboDeviceIndex = itemData(i).toUInt();

    if (index == comboDeviceIndex) {
      removeItem(i);
      break;
    }
  }
}

void ZeusVirtualDeviceComboBox::setCurrentDeviceByName(QString name) {
  for (int i = 0; i < count(); i++) {
    if (name != itemText(i))
      continue;

    setCurrentIndex(i);
    break;
  }
}

void ZeusVirtualDeviceComboBox::wheelEvent(QWheelEvent *event) {
  // Only change the target if the user explicitly selects a different one.
  // This prevents accidentally changing targets when the user is scrolling but
  // just so happens to have the mouse wheel over this widget.
  event->ignore();
}
