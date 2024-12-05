#ifndef ZEUSSTREAMVIEW_H
#define ZEUSSTREAMVIEW_H
#include "widgets/devicecombobox.h"
#include <QLabel>
#include <QWidget>

class QHBoxLayout;
class ElidingLabel;

class ZeusStreamView : public QWidget {
  Q_OBJECT
public:
  ZeusStreamView(QString client, QString name, uint32_t index);

  uint32_t index(void) { return m_index; }
  void addDevice(uint32_t index, QString name, QString desc);
  void removeDevice(uint32_t index);
  virtual void setCurrentDeviceByIndex(uint32_t index) = 0;

protected:
  QHBoxLayout *setupTopLine(QString client, QString name);

  QLabel *m_clientNameLabel;
  ElidingLabel *m_nameLabel;
  ZeusDeviceComboBox *m_deviceCombo;
  uint32_t m_index;
};

#endif
