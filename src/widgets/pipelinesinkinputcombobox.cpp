#include "widgets/pipelinesinkinputcombobox.h"
#include "core/pulsedata.h"

#define ZEUS_PIPEDESC "zeus.pipedesc"

ZeusPipelineSinkInputComboBox::ZeusPipelineSinkInputComboBox(ZeusPulseData *pd)
    : ZeusBasePulseItemComboBox() {
  ZEUS_PULSE_CONNECT_LOAD(pd, this, ZeusPipelineSinkInputComboBox, sinkInput,
                          SinkInput);
}

void ZeusPipelineSinkInputComboBox::onSinkInputAdded(
    ZeusPulseStreamInfo *info) {
  QString pipedesc = info->props.value(ZEUS_PIPEDESC, "");

  if (pipedesc.isEmpty())
    return;

  addRawItem(info->index, pipedesc, pipedesc);
}

void ZeusPipelineSinkInputComboBox::onSinkInputRemoved(uint32_t index) {
  removeDevice(index);
}
