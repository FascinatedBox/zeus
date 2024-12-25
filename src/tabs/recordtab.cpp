#include "tabs/recordtab.h"
#include "core/pulsedata.h"
#include "views/sourceoutputview.h"

ZeusRecordTab::ZeusRecordTab(void) : ZeusStreamTab() {}

void ZeusRecordTab::connectToPulseData(ZeusPulseData *pd) {
  connect(pd, &ZeusPulseData::clientAdded, this, &ZeusRecordTab::onClientAdded);
  connect(pd, &ZeusPulseData::clientRemoved, this,
          &ZeusRecordTab::onClientRemoved);
  connect(pd, &ZeusPulseData::sourceAdded, this, &ZeusRecordTab::onSourceAdded);
  connect(pd, &ZeusPulseData::sourceRemoved, this,
          &ZeusRecordTab::onDeviceRemoved);
  connect(pd, &ZeusPulseData::sourceOutputAdded, this,
          &ZeusRecordTab::onSourceOutputAdded);
  connect(pd, &ZeusPulseData::sourceOutputRemoved, this,
          &ZeusRecordTab::onStreamRemoved);
}

void ZeusRecordTab::onSourceAdded(uint32_t id, ZeusPulseDeviceInfo *info) {
  deviceAdded(id, info->name, info->desc);
}

void ZeusRecordTab::onSourceOutputAdded(uint32_t id,
                                        ZeusPulseStreamInfo *info) {
  QString clientName = m_clientNames.value(info->client, "");
  QString name = info->name;
  ZeusSourceOutputView *view = new ZeusSourceOutputView(clientName, name, id);

  streamAdded(view, info->target);
}
