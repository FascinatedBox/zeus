#ifndef ZEUSBASEDEVICECOMBOBOX_H
#define ZEUSBASEDEVICECOMBOBOX_H
#include <QComboBox>

class QWheelEvent;
class ZeusPulseDeviceInfo;
class ZeusPulseStreamInfo;

class ZeusBaseDeviceComboBox : public QComboBox {
  Q_OBJECT

public:
  ZeusBaseDeviceComboBox(void);

  // Wrap currentText to make usage more obvious.
  QString currentDeviceDescription(void) { return currentText(); }
  QString currentDeviceName(void);
  void loadInfo(ZeusPulseStreamInfo *);
  void setCurrentDeviceByName(QString);

protected:
  void addDevice(ZeusPulseDeviceInfo *);
  void removeDevice(uint32_t);

private:
  void wheelEvent(QWheelEvent *);
};

#endif
