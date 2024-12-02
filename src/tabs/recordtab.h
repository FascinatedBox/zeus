#ifndef ZEUSRECORDTAB_H
#define ZEUSRECORDTAB_H
#include "tabs/streamtab.h"

class ZeusSinkInputView;

class ZeusRecordTab : public ZeusStreamTab {
  Q_OBJECT

public:
  ZeusRecordTab(void);

  void sourceAdded(const pa_source_info *i);
  void sourceRemoved(uint32_t index) { deviceRemoved(index); }
  void sourceOutputAdded(const pa_source_output_info *i);
  void sourceOutputRemoved(uint32_t index) { streamRemoved(index); }
};

#endif
