#include "views/streamview.h"
#include "core/paction.h"
#include "core/pulsedata.h"
#include "widgets/devicecombobox.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QVBoxLayout>

ZeusStreamView::ZeusStreamView(ZeusPulseData *pd, ZeusPulseStreamInfo *info)
    : m_index(info->index) {
  QHBoxLayout *layout = new QHBoxLayout;
  m_nameLabel = new QLabel;
  QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding,
                                        QSizePolicy::Policy::Minimum);
  ZeusPulseInfoType comboType = (info->type == ZISinkInput) ? ZISink : ZISource;
  m_deviceCombo = new ZeusDeviceComboBox(pd, comboType);

  m_nameLabel->setMinimumWidth(1);
  syncToInfo(info);

  QString clientName = pd->clientNameByIndexOr(info->client, "");

  // Clients won't get a name later on, so only check once.
  if (clientName.isEmpty() == false)
    layout->addWidget(new QLabel(QString("<b>%1</b>: ").arg(clientName)));

  layout->addWidget(m_nameLabel);
  layout->addItem(spacer);
  layout->addWidget(m_deviceCombo);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  setLayout(layout);

  if (info->type == ZISinkInput)
    connect(m_deviceCombo, &QComboBox::currentIndexChanged, this,
            &ZeusStreamView::updateSinkInputTargetSink);
  else
    connect(m_deviceCombo, &QComboBox::currentIndexChanged, this,
            &ZeusStreamView::updateSourceOutputTargetSource);
}

void ZeusStreamView::syncToInfo(ZeusPulseStreamInfo *info) {
  m_lastTarget = info->target;
  m_nameLabel->setText(info->name);
  m_deviceCombo->changeDeviceTo(info->target);
}

void ZeusStreamView::streamMoveCallback(void *, int success, void *data) {
  if (success)
    return;

  ZeusStreamView *view = (ZeusStreamView *)data;
  ZeusDeviceComboBox *dc = view->m_deviceCombo;

  // Assume that this is the first move done and the stream was created with
  // PA_STREAM_DONT_MOVE. Unplug the signal so that reverting the device doesn't
  // trigger an update. Use nullptr at the end to disconnect both updates.
  disconnect(dc, &QComboBox::currentIndexChanged, view, nullptr);
  dc->changeDeviceTo(view->m_lastTarget);
  dc->setEnabled(false);
  view->sendMoveFailed();
}

void ZeusStreamView::updateSourceOutputTargetSource(int) {
  uint32_t data = m_deviceCombo->currentData().toUInt();

  if (data == m_lastTarget)
    return;

  zeus_pa_move_source_output(m_index, data, &ZeusStreamView::streamMoveCallback,
                             this);
}

void ZeusStreamView::updateSinkInputTargetSink(int) {
  uint32_t data = m_deviceCombo->currentData().toUInt();

  if (data == m_lastTarget)
    return;

  zeus_pa_move_sink_input(m_index, data, &ZeusStreamView::streamMoveCallback,
                          this);
}
