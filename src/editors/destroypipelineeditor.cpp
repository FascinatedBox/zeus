#include "editors/destroypipelineeditor.h"
#include "widgets/pipelinesinkinputcombobox.h"
#include <QDialogButtonBox>
#include <QFormLayout>

ZeusDestroyPipelineEditor::ZeusDestroyPipelineEditor(ZeusPulseData *pd,
                                                     QWidget *parent)
    : ZeusBaseEditor(parent) {
  QFormLayout *layout = new QFormLayout;
  m_sinkInputCombo = new ZeusPipelineSinkInputComboBox(pd);

  layout->addRow("Pipeline to destroy:", m_sinkInputCombo);
  layout->addWidget(m_buttonBox);
  setLayout(layout);
}

bool ZeusDestroyPipelineEditor::isValid(void) {
  return (m_sinkInputCombo->currentIndex() != -1);
}

void ZeusDestroyPipelineEditor::loadAction(ZeusBaseAction *act) {
  auto a = static_cast<ZeusDestroyPipelineAct *>(act);

  m_sinkInputCombo->useItemNameAndDesc(a->desc, a->desc);
}

ZeusDestroyPipelineAct *ZeusDestroyPipelineEditor::makeAction(void) {
  QString sinkDesc = m_sinkInputCombo->currentItemDescription();

  return new ZeusDestroyPipelineAct(sinkDesc);
}

void ZeusDestroyPipelineEditor::reset(void) {
  m_sinkInputCombo->setCurrentIndex(0);
}
