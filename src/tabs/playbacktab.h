#ifndef ZEUSPLAYBACKTAB_H
#define ZEUSPLAYBACKTAB_H
#include "tabs/streamtab.h"

class ZeusPulseData;
class ZeusSinkInputView;
class ZeusPulseDeviceInfo;
class ZeusPulseStreamInfo;

class ZeusPlaybackTab : public ZeusStreamTab {
  Q_OBJECT

public:
  ZeusPlaybackTab(void);

  void connectToPulseData(ZeusPulseData *pd);

private slots:
  void onSinkAdded(uint32_t id, ZeusPulseDeviceInfo *info);
  void onSinkInputAdded(uint32_t id, ZeusPulseStreamInfo *info);
};

#endif
