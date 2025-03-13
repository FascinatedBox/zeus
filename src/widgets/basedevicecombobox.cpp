#include "widgets/basedevicecombobox.h"
#include "core/pulsedata.h"
#include <QMenu>
#include <QWheelEvent>

#define NAME_ROLE (Qt::UserRole + 1)

ZeusBaseDeviceComboBox::ZeusBaseDeviceComboBox(void)
    : QComboBox(), m_placeholderName("") {}

void ZeusBaseDeviceComboBox::addDevice(ZeusPulseDeviceInfo *info) {
  int spot = count();

  if (info->name == m_placeholderName) {
    spot = indexUsedByPlaceholder();
    m_placeholderName = "";
  } else
    addItem(info->desc);

  setItemData(spot, info->index);
  setItemData(spot, info->name, NAME_ROLE);
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

      // The rest of this is unlikely, don't even bother.
      return;
    }
  }

  // It's rare but possible for PulseAudio to send an update but to have the
  // stream's device as invalid. Create an invalid device placeholder and leave
  // it. The stream is probably pinned so it won't matter.
  if (index == (uint32_t)-1) {
    QString text = "Unknown Device";
    int spot = count();

    addItem(text, index);
    setItemData(spot, text, NAME_ROLE);
    setCurrentIndex(spot);
  }
}

int ZeusBaseDeviceComboBox::indexUsedByPlaceholder(void) {
  int result = 0;

  for (int i = 0; i < count(); i++) {
    QString n = itemData(i, NAME_ROLE).toString();

    if (n == m_placeholderName) {
      result = i;
      break;
    }
  }

  return result;
}

void ZeusBaseDeviceComboBox::mousePressEvent(QMouseEvent *event) {
  if (count() == 0) {
    // Don't bother, there's nothing to show.
    event->ignore();
    return;
  }

  // Pavucontrol puts the menu below the button which looks neat.
  QPoint coords = mapToGlobal(QPoint(0, 0)) + QPoint(0, height() + 4);
  QMenu menu(this);
  int current = currentIndex();

  for (int i = 0; i < count(); i++) {
    QAction *act = new QAction(itemText(i), this);

    act->setData(i);
    act->setCheckable(true);

    if (i == current)
      act->setChecked(true);

    menu.addAction(act);
  }

  QAction *result = menu.exec(coords);

  if (result)
    setCurrentIndex(result->data().toInt());
}

void ZeusBaseDeviceComboBox::useDeviceNameAndDesc(QString name, QString desc) {
  for (int i = 0; i < count(); i++) {
    QString n = itemData(i, NAME_ROLE).toString();

    if (name != n)
      continue;

    setCurrentIndex(i);

    if (m_placeholderName.isEmpty() == true)
      // No placeholder to clean up after, so nothing else to do.
      return;

    // The old action loader may have included a placeholder that this new
    // loader doesn't need. Figure it out.
    break;
  }

  if (m_placeholderName == name)
    // The above loop has selected it, and it's still a placeholder. Leave it.
    return;

  // Dump the old one, add the new one.
  removeItem(indexUsedByPlaceholder());
  m_placeholderName = name;

  int spot = count();

  addItem(desc, 0); // Index 0 shouldn't match anything.
  setItemData(spot, name, NAME_ROLE);
  setCurrentIndex(spot);
}

void ZeusBaseDeviceComboBox::wheelEvent(QWheelEvent *event) {
  // Only change the target if the user explicitly selects a different one.
  // This prevents accidentally changing targets when the user is scrolling but
  // just so happens to have the mouse wheel over this widget.
  event->ignore();
}
