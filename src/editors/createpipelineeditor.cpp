#include "editors/createpipelineeditor.h"
#include "widgets/devicecombobox.h"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>

ZeusCreatePipelineEditor::ZeusCreatePipelineEditor(ZeusPulseData *pd,
                                                   QWidget *parent)
    : ZeusBaseEditor(parent) {
  QVBoxLayout *layout = new QVBoxLayout;
  QFormLayout *formLayout = new QFormLayout;
  m_sinkCombo = new ZeusDeviceComboBox(pd, ZISink);
  m_sourceCombo = new ZeusDeviceComboBox(pd, ZISource);

  formLayout->addRow("Source Device:", m_sourceCombo);
  formLayout->addRow("Playback Device:", m_sinkCombo);

  layout->addLayout(formLayout);
  layout->addWidget(m_buttonBox);
  setLayout(layout);
}

bool ZeusCreatePipelineEditor::isValid(void) {
  if (m_sinkCombo->currentIndex() == -1)
    return false;

  if (m_sourceCombo->currentIndex() == -1)
    return false;

  return true;
}

void ZeusCreatePipelineEditor::loadAction(ZeusBaseAction *act) {
  auto a = static_cast<ZeusCreatePipelineAct *>(act);

  m_sinkCombo->setCurrentDeviceByName(a->sinkName);
  m_sourceCombo->setCurrentDeviceByName(a->sourceName);
}

ZeusCreatePipelineAct *ZeusCreatePipelineEditor::makeAction(void) {
  QString sinkName = m_sinkCombo->currentDeviceName();
  QString sourceName = m_sourceCombo->currentDeviceName();

  return ZeusCreatePipelineAct::fromSinkAndSource(sinkName, sourceName);
}

void ZeusCreatePipelineEditor::reset(void) {
  m_sinkCombo->setCurrentIndex(0);
  m_sourceCombo->setCurrentIndex(0);
}
