#include "editors/createvirtualsinkeditor.h"
#include "core/utils.h"
#include "widgets/kvpropertygroupbox.h"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>

ZeusCreateVirtualSinkEditor::ZeusCreateVirtualSinkEditor(ZeusPulseData *,
                                                         QWidget *parent)
    : ZeusBaseEditor(parent) {
  QVBoxLayout *layout = new QVBoxLayout;
  QFormLayout *formLayout = new QFormLayout;
  m_nameLine = new QLineEdit;
  m_propertyBox = new ZeusKVPropertyGroupBox("Sink Properties (optional)");

  formLayout->addRow("Name:", m_nameLine);
  layout->addLayout(formLayout);
  layout->addWidget(m_propertyBox);
  layout->addWidget(m_buttonBox);
  setLayout(layout);
}

bool ZeusCreateVirtualSinkEditor::isValid(void) {
  return ::isValidPulseDeviceName(m_nameLine->text());
}

void ZeusCreateVirtualSinkEditor::loadAction(ZeusBaseAction *act) {
  auto a = static_cast<ZeusCreateVirtualSinkAct *>(act);

  m_nameLine->setText(a->name);
  m_propertyBox->loadProperties(a->props);
}

ZeusCreateVirtualSinkAct *ZeusCreateVirtualSinkEditor::makeAction(void) {
  QString name = m_nameLine->text();
  QList<QPair<QString, QString>> props = m_propertyBox->collectProperties();

  return new ZeusCreateVirtualSinkAct(name, props);
}

void ZeusCreateVirtualSinkEditor::reset(void) {
  m_nameLine->setText("");
  m_propertyBox->reset();
}
