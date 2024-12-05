#include <QVBoxLayout>

#include "tabs/playbacktab.h"
#include "views/sinkinputview.h"

ZeusPlaybackTab::ZeusPlaybackTab(void) : ZeusStreamTab() {}

void ZeusPlaybackTab::sinkAdded(const pa_sink_info *info) {
  deviceAdded(info->index, info->name, info->description);
}

void ZeusPlaybackTab::sinkInputAdded(const pa_sink_input_info *i) {
  if (i->client == -1)
    return; // For now, skip outputs for virtual sinks.

  QString clientName = m_clientNames.value(i->client, "");
  QString name = i->name;
  uint32_t index = i->index;
  ZeusSinkInputView *view = new ZeusSinkInputView(clientName, name, index);

  streamAdded(view, i->sink);
}
