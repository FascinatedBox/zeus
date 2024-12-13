#include "dialogs/createpipelinedialog.h"
#include "widgets/devicecombobox.h"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>

ZeusCreatePipelineDialog::ZeusCreatePipelineDialog(QWidget *parent)
    : ZeusBaseDialog(parent) {
  QVBoxLayout *layout = new QVBoxLayout;
  QFormLayout *formLayout = new QFormLayout;
  m_sinkCombo = new ZeusDeviceComboBox;
  m_sourceCombo = new ZeusDeviceComboBox;

  formLayout->addRow("Source Device:", m_sourceCombo);
  formLayout->addRow("Playback Device:", m_sinkCombo);

  layout->addLayout(formLayout);
  layout->addWidget(m_buttonBox);
  setLayout(layout);
  setWindowTitle("Create pipeline");
}

void ZeusCreatePipelineDialog::loadDeviceList(ZeusPulseData *pd,
                                              ZeusDeviceComboBox *combo) {
  QMapIterator<uint32_t, ZeusPulseDeviceInfo *> iter = pd->sinkIterator();

  if (combo == m_sourceCombo)
    iter = pd->sourceIterator();

  while (iter.hasNext()) {
    iter.next();
    combo->addDevice(iter.key(), iter.value()->name, iter.value()->desc);
  }
}

void ZeusCreatePipelineDialog::onSinkAdded(uint32_t index,
                                           ZeusPulseDeviceInfo *info) {
  m_sinkCombo->addDevice(index, info->name, info->desc);
}

void ZeusCreatePipelineDialog::onSinkRemoved(uint32_t index) {
  m_sinkCombo->removeDevice(index);
}

void ZeusCreatePipelineDialog::onSourceAdded(uint32_t index,
                                             ZeusPulseDeviceInfo *info) {
  m_sourceCombo->addDevice(index, info->name, info->desc);
}

void ZeusCreatePipelineDialog::onSourceRemoved(uint32_t index) {
  m_sourceCombo->removeDevice(index);
}

void ZeusCreatePipelineDialog::connectToPulseData(ZeusPulseData *pd) {
  connect(pd, &ZeusPulseData::sinkAdded, this,
          &ZeusCreatePipelineDialog::onSinkAdded);
  connect(pd, &ZeusPulseData::sinkRemoved, this,
          &ZeusCreatePipelineDialog::onSinkRemoved);
  connect(pd, &ZeusPulseData::sourceAdded, this,
          &ZeusCreatePipelineDialog::onSourceAdded);
  connect(pd, &ZeusPulseData::sourceRemoved, this,
          &ZeusCreatePipelineDialog::onSourceRemoved);

  loadDeviceList(pd, m_sinkCombo);
  loadDeviceList(pd, m_sourceCombo);
}

bool ZeusCreatePipelineDialog::isValid(void) {
  if (m_sinkCombo->currentIndex() == -1)
    return false;

  if (m_sourceCombo->currentIndex() == -1)
    return false;

  return true;
}

ZeusCreatePipelineAct *ZeusCreatePipelineDialog::makeAction(void) {
  QString sinkName = m_sinkCombo->currentDeviceName();
  QString sourceName = m_sourceCombo->currentDeviceName();
  ZeusCreatePipelineAct *result =
      ZeusCreatePipelineAct::fromSinkAndSource(sinkName, sourceName);
  return result;
}
