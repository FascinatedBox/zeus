#include "editors/createnullsinkeditor.h"
#include "core/utils.h"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>

ZeusCreateNullSinkEditor::ZeusCreateNullSinkEditor(ZeusPulseData *,
                                                   QWidget *parent)
    : ZeusBaseEditor(parent) {
  QVBoxLayout *layout = new QVBoxLayout;
  QFormLayout *formLayout = new QFormLayout;
  m_nameLine = new QLineEdit;

  formLayout->addRow("Name:", m_nameLine);
  layout->addLayout(formLayout);
  layout->addWidget(m_buttonBox);
  setLayout(layout);
}

bool ZeusCreateNullSinkEditor::isValid(void) {
  return ::isValidPulseDeviceName(m_nameLine->text());
}

void ZeusCreateNullSinkEditor::loadAction(ZeusBaseAction *act) {
  auto a = static_cast<ZeusCreateNullSinkAct *>(act);

  m_nameLine->setText(a->name);
}

ZeusCreateNullSinkAct *ZeusCreateNullSinkEditor::makeAction(void) {
  QString name = m_nameLine->text();

  return new ZeusCreateNullSinkAct(name);
}

void ZeusCreateNullSinkEditor::reset(void) { m_nameLine->setText(""); }
