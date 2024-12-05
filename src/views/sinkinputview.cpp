#include "views/sinkinputview.h"
#include "core/paction.h"

ZeusSinkInputView::ZeusSinkInputView(QString client, QString name,
                                     uint32_t index)
    : ZeusStreamView(client, name, index) {}

void ZeusSinkInputView::setCurrentDeviceByIndex(uint32_t deviceIndex) {
  m_deviceCombo->setCurrentDeviceByIndex(deviceIndex);

  connect(m_deviceCombo, &QComboBox::currentIndexChanged, this,
          &ZeusSinkInputView::updateSinkInputTargetSink);
}

void ZeusSinkInputView::updateSinkInputTargetSink(int) {
  uint32_t data = m_deviceCombo->currentData().toUInt();

  zeus_pa_move_sink_input_by_index(m_index, data);
}
