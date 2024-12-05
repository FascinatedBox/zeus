#include "views/sourceoutputview.h"
#include "core/paction.h"

ZeusSourceOutputView::ZeusSourceOutputView(QString client, QString name,
                                           uint32_t index)
    : ZeusStreamView(client, name, index) {}

void ZeusSourceOutputView::setCurrentDeviceByIndex(uint32_t deviceIndex) {
  m_deviceCombo->setCurrentDeviceByIndex(deviceIndex);

  connect(m_deviceCombo, &QComboBox::currentIndexChanged, this,
          &ZeusSourceOutputView::updateSourceOutputTargetSource);
}

void ZeusSourceOutputView::updateSourceOutputTargetSource(int) {
  uint32_t data = m_deviceCombo->currentData().toUInt();

  zeus_pa_move_source_output_by_index(m_index, data);
}
