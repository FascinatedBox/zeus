#include "dialogs/moveplaybackstreamdialog.h"
#include "core/pulsedata.h"
#include "widgets/devicecombobox.h"
#include "widgets/querypropertygroupbox.h"
#include <QDialogButtonBox>
#include <QFormLayout>

ZeusMovePlaybackStreamDialog::ZeusMovePlaybackStreamDialog(QWidget *parent)
    : ZeusBaseDialog(parent) {
  QFormLayout *layout = new QFormLayout;
  m_sinkCombo = new ZeusDeviceComboBox;
  m_queryBox = new ZeusQueryPropertyGroupBox("Selected streams");

  layout->addRow(m_queryBox);
  layout->addRow("Playback Device:", m_sinkCombo);
  layout->addRow(m_buttonBox);
  setLayout(layout);
  setWindowTitle("Move playback stream");
}

void ZeusMovePlaybackStreamDialog::loadDeviceList(ZeusPulseData *pd,
                                                  ZeusDeviceComboBox *combo) {
  QMapIterator<uint32_t, ZeusPulseDeviceInfo *> iter = pd->sinkIterator();

  while (iter.hasNext()) {
    iter.next();
    combo->addDevice(iter.key(), iter.value()->name, iter.value()->desc);
  }
}

void ZeusMovePlaybackStreamDialog::connectToPulseData(ZeusPulseData *pd) {
  connect(pd, &ZeusPulseData::sinkAdded, this,
          &ZeusMovePlaybackStreamDialog::onSinkAdded);
  connect(pd, &ZeusPulseData::sinkRemoved, this,
          &ZeusMovePlaybackStreamDialog::onSinkRemoved);
  loadDeviceList(pd, m_sinkCombo);
  m_queryBox->connectToPulseData(pd);
}

void ZeusMovePlaybackStreamDialog::onSinkAdded(uint32_t index,
                                               ZeusPulseDeviceInfo *info) {
  m_sinkCombo->addDevice(index, info->name, info->desc);
}

void ZeusMovePlaybackStreamDialog::onSinkRemoved(uint32_t index) {
  m_sinkCombo->removeDevice(index);
}

bool ZeusMovePlaybackStreamDialog::isValid(void) {
  if (m_sinkCombo->currentIndex() == -1)
    return false;

  return true;
}

ZeusMovePlaybackStreamAct *ZeusMovePlaybackStreamDialog::makeAction(void) {
  ZeusPulseQuery *query = m_queryBox->intoQuery();
  QString sinkName = m_sinkCombo->currentDeviceName();
  auto result = new ZeusMovePlaybackStreamAct(query, sinkName);

  return result;
}
