#ifndef ZEUSPIPELINESINKINPUTCOMBOBOX_H
#define ZEUSPIPELINESINKINPUTCOMBOBOX_H
#include "widgets/basepulseitemcombobox.h"

class ZeusPulseData;
class ZeusPulseStreamInfo;

class ZeusPipelineSinkInputComboBox : public ZeusBasePulseItemComboBox {
  Q_OBJECT

public:
  ZeusPipelineSinkInputComboBox(ZeusPulseData *);

private slots:
  void onSinkInputAdded(ZeusPulseStreamInfo *);
  void onSinkInputRemoved(uint32_t);
};

#endif
