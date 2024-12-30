#ifndef ZEUSVIRTUALDEVICECOMBOBOX_H
#define ZEUSVIRTUALDEVICECOMBOBOX_H
#include <QComboBox>

class QWheelEvent;
class ZeusPulseData;
class ZeusPulseDeviceInfo;

class ZeusVirtualDeviceComboBox : public QComboBox {
  Q_OBJECT

public:
  ZeusVirtualDeviceComboBox(ZeusPulseData *);

  QString currentDeviceName(void);

private slots:
  void onSinkAdded(ZeusPulseDeviceInfo *);
  void onSinkRemoved(uint32_t);

private:
  void wheelEvent(QWheelEvent *event);
};

#endif
