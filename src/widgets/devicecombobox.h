#ifndef ZEUSDEVICECOMBOBOX_H
#define ZEUSDEVICECOMBOBOX_H
#include <QComboBox>

class ZeusDeviceComboBox : public QComboBox {
  Q_OBJECT

public:
  ZeusDeviceComboBox(void);

  void addDevice(uint32_t index, QString name, QString desc);
  QString currentDeviceName(void);
  void removeDevice(uint32_t index);
  void setCurrentDeviceByIndex(uint32_t index);
};

#endif
