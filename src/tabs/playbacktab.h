#ifndef ZEUSPLAYBACKTAB_H
#define ZEUSPLAYBACKTAB_H
#include "tabs/streamtab.h"

class ZeusSinkInputView;

class ZeusPlaybackTab : public ZeusStreamTab {
  Q_OBJECT

public:
  ZeusPlaybackTab(void);

  void sinkAdded(const pa_sink_info *i);
  void sinkRemoved(uint32_t index) { deviceRemoved(index); }
  void sinkInputAdded(const pa_sink_input_info *i);
  void sinkInputRemoved(uint32_t index) { streamRemoved(index); }
};

#endif
