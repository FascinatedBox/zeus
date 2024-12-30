#ifndef ZEUSDEVICECOMBOBOX_H
#define ZEUSDEVICECOMBOBOX_H
#include "core/pulsedata.h"
#include <QComboBox>

class QWheelEvent;

class ZeusDeviceComboBox : public QComboBox {
  Q_OBJECT

public:
  ZeusDeviceComboBox(ZeusPulseData *, ZeusPulseInfoType);

  QString currentDeviceName(void);
  void loadInfo(ZeusPulseStreamInfo *);

private slots:
  void onSinkAdded(ZeusPulseDeviceInfo *);
  void onSinkRemoved(uint32_t);
  void onSourceAdded(ZeusPulseDeviceInfo *);
  void onSourceRemoved(uint32_t);

private:
  void removeDevice(uint32_t index);
  void wheelEvent(QWheelEvent *event);
};

#endif
