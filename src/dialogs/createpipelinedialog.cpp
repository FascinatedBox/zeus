#include "dialogs/createpipelinedialog.h"
#include "widgets/devicecombobox.h"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>

ZeusCreatePipelineDialog::ZeusCreatePipelineDialog(ZeusPulseData *pd,
                                                   QWidget *parent)
    : ZeusBaseDialog(parent) {
  QVBoxLayout *layout = new QVBoxLayout;
  QFormLayout *formLayout = new QFormLayout;
  m_sinkCombo = new ZeusDeviceComboBox(pd, ZISink);
  m_sourceCombo = new ZeusDeviceComboBox(pd, ZISource);

  formLayout->addRow("Source Device:", m_sourceCombo);
  formLayout->addRow("Playback Device:", m_sinkCombo);

  layout->addLayout(formLayout);
  layout->addWidget(m_buttonBox);
  setLayout(layout);
  setWindowTitle("Create pipeline");
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

  return ZeusCreatePipelineAct::fromSinkAndSource(sinkName, sourceName);
}
