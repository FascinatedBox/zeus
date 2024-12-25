#ifndef ZEUSRECORDTAB_H
#define ZEUSRECORDTAB_H
#include "tabs/streamtab.h"

class ZeusPulseData;
class ZeusPulseDeviceInfo;
class ZeusPulseStreamInfo;

class ZeusRecordTab : public ZeusStreamTab {
  Q_OBJECT

public:
  ZeusRecordTab(void);

  void connectToPulseData(ZeusPulseData *pd);

private slots:
  void onSourceAdded(uint32_t id, ZeusPulseDeviceInfo *info);
  void onSourceOutputAdded(uint32_t id, ZeusPulseStreamInfo *info);
};

#endif
