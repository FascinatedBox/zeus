#include "tabs/playbacktab.h"
#include "core/pulsedata.h"
#include "views/sinkinputview.h"

ZeusPlaybackTab::ZeusPlaybackTab(void) : ZeusStreamTab() {}

void ZeusPlaybackTab::connectToPulseData(ZeusPulseData *pd) {
  connect(pd, &ZeusPulseData::clientAdded, this,
          &ZeusPlaybackTab::onClientAdded);
  connect(pd, &ZeusPulseData::clientRemoved, this,
          &ZeusPlaybackTab::onClientRemoved);
  connect(pd, &ZeusPulseData::sinkAdded, this, &ZeusPlaybackTab::onSinkAdded);
  connect(pd, &ZeusPulseData::sinkRemoved, this,
          &ZeusPlaybackTab::onDeviceRemoved);
  connect(pd, &ZeusPulseData::sinkInputAdded, this,
          &ZeusPlaybackTab::onSinkInputAdded);
  connect(pd, &ZeusPulseData::sinkInputRemoved, this,
          &ZeusPlaybackTab::onStreamRemoved);
}

void ZeusPlaybackTab::onSinkAdded(uint32_t id, ZeusPulseDeviceInfo *info) {
  deviceAdded(id, info->name, info->desc);
}

void ZeusPlaybackTab::onSinkInputAdded(uint32_t id, ZeusPulseStreamInfo *info) {
  if (info->client == (uint32_t)-1)
    return; // For now, skip outputs for virtual sinks.

  QString clientName = m_clientNames.value(info->client, "");
  QString name = info->name;
  ZeusSinkInputView *view = new ZeusSinkInputView(clientName, name, id);

  streamAdded(view, info->target);
}
