#include "views/sinkinputview.h"
#include "core/paction.h"

ZeusSinkInputView::ZeusSinkInputView(QString client, QString name,
                                     uint32_t index)
    : ZeusStreamView(client, name, index) {}

void ZeusSinkInputView::setCurrentDeviceIndex(uint32_t deviceIndex) {
  for (int i = 0; i < m_deviceCombo->count(); i++) {
    uint32_t comboDeviceIndex = m_deviceCombo->itemData(i).toUInt();

    if (deviceIndex == comboDeviceIndex) {
      m_deviceCombo->setCurrentIndex(i);
      break;
    }
  }

  connect(m_deviceCombo, &QComboBox::currentIndexChanged, this,
          &ZeusSinkInputView::updateSinkInputTargetSink);
}

void ZeusSinkInputView::updateSinkInputTargetSink(int) {
  uint32_t data = m_deviceCombo->currentData().toUInt();

  zeus_pa_move_sink_input_by_index(m_index, data);
}
