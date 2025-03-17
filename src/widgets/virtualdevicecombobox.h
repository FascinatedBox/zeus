#ifndef ZEUSVIRTUALDEVICECOMBOBOX_H
#define ZEUSVIRTUALDEVICECOMBOBOX_H
#include "widgets/basepulseitemcombobox.h"
#include <QComboBox>

class ZeusPulseData;
class ZeusPulseDeviceInfo;

class ZeusVirtualDeviceComboBox : public ZeusBasePulseItemComboBox {
  Q_OBJECT

public:
  ZeusVirtualDeviceComboBox(ZeusPulseData *);

private slots:
  void onSinkAdded(ZeusPulseDeviceInfo *);
  void onSinkRemoved(uint32_t);
};

#endif
