#include <QVBoxLayout>

#include "tabs/recordtab.h"
#include "views/sourceoutputview.h"

ZeusRecordTab::ZeusRecordTab(void) : ZeusStreamTab() {}

void ZeusRecordTab::sourceAdded(const pa_source_info *info) {
  deviceAdded(info->index, info->description);
}

void ZeusRecordTab::sourceOutputAdded(const pa_source_output_info *i) {
  QString clientName = m_clientNames.value(i->client, "");
  QString name = i->name;
  uint32_t index = i->index;
  ZeusSourceOutputView *view =
      new ZeusSourceOutputView(clientName, name, index);

  streamAdded(view, i->source);
}
