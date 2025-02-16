#ifndef ZEUSDEVICECOMBOBOX_H
#define ZEUSDEVICECOMBOBOX_H
#include "core/pulsedata.h"
#include "widgets/basedevicecombobox.h"

class ZeusDeviceComboBox : public ZeusBaseDeviceComboBox {
  Q_OBJECT

public:
  ZeusDeviceComboBox(ZeusPulseData *, ZeusPulseInfoType);

private slots:
  void onSinkAdded(ZeusPulseDeviceInfo *);
  void onSinkRemoved(uint32_t);
  void onSourceAdded(ZeusPulseDeviceInfo *);
  void onSourceRemoved(uint32_t);
};

#endif
