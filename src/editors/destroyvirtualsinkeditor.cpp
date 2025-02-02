#include "editors/destroyvirtualsinkeditor.h"
#include "core/utils.h"
#include "widgets/virtualdevicecombobox.h"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>

ZeusDestroyVirtualSinkEditor::ZeusDestroyVirtualSinkEditor(ZeusPulseData *pd,
                                                           QWidget *parent)
    : ZeusBaseEditor(parent) {
  QVBoxLayout *layout = new QVBoxLayout;
  QFormLayout *formLayout = new QFormLayout;
  m_sinkCombo = new ZeusVirtualDeviceComboBox(pd);

  formLayout->addRow("Sink to destroy:", m_sinkCombo);
  layout->addLayout(formLayout);
  layout->addWidget(m_buttonBox);
  setLayout(layout);
}

bool ZeusDestroyVirtualSinkEditor::isValid(void) {
  return (m_sinkCombo->currentIndex() != -1);
}

void ZeusDestroyVirtualSinkEditor::loadAction(ZeusBaseAction *act) {
  auto a = static_cast<ZeusDestroyVirtualSinkAct *>(act);

  m_sinkCombo->setCurrentDeviceByName(a->name);
}

ZeusDestroyVirtualSinkAct *ZeusDestroyVirtualSinkEditor::makeAction(void) {
  QString sinkName = m_sinkCombo->currentDeviceName();

  return new ZeusDestroyVirtualSinkAct(sinkName);
}

void ZeusDestroyVirtualSinkEditor::reset(void) {
  m_sinkCombo->setCurrentIndex(0);
}
