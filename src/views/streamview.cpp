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
  QString clientName = pd->clientNameByIndexOr(info->client, "");
  QHBoxLayout *layout = new QHBoxLayout;
  m_clientNameLabel = new QLabel(QString("<b>%1</b>").arg(clientName));
  m_nameLabel = new QLabel(this);
  QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding,
                                        QSizePolicy::Policy::Minimum);

  ZeusPulseInfoType comboType = (info->type == ZISinkInput) ? ZISink : ZISource;
  m_deviceCombo = new ZeusDeviceComboBox(pd, comboType);

  m_nameLabel->setMinimumWidth(1);
  syncToInfo(info);
  layout->addWidget(m_clientNameLabel);
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
  m_nameLabel->setText(QString(": %1").arg(info->name));
  m_deviceCombo->loadInfo(info);
}

void ZeusStreamView::updateSourceOutputTargetSource(int) {
  uint32_t data = m_deviceCombo->currentData().toUInt();

  zeus_pa_move_source_output(m_index, data, nullptr, nullptr);
}

void ZeusStreamView::updateSinkInputTargetSink(int) {
  uint32_t data = m_deviceCombo->currentData().toUInt();

  zeus_pa_move_sink_input(m_index, data, nullptr, nullptr);
}
