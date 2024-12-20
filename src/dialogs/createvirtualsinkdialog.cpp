#include "dialogs/createvirtualsinkdialog.h"
#include "core/utils.h"
#include "widgets/propertygroupbox.h"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>

ZeusCreateVirtualSinkDialog::ZeusCreateVirtualSinkDialog(QWidget *parent)
    : ZeusBaseDialog(parent) {
  QVBoxLayout *layout = new QVBoxLayout;
  QFormLayout *formLayout = new QFormLayout;
  m_nameLine = new QLineEdit;
  m_propertyBox = new ZeusPropertyGroupBox("Sink Properties (optional)");

  formLayout->addRow("Name:", m_nameLine);
  layout->addLayout(formLayout);
  layout->addWidget(m_propertyBox);
  layout->addWidget(m_buttonBox);
  setLayout(layout);
  setWindowTitle("Create virtual sink");
}

bool ZeusCreateVirtualSinkDialog::isValid(void) {
  return ::isValidPulseDeviceName(m_nameLine->text());
}

ZeusCreateVirtualSinkAct *ZeusCreateVirtualSinkDialog::makeAction(void) {
  QString name = m_nameLine->text();
  QList<QPair<QString, QString>> props = m_propertyBox->collectProperties();
  ZeusCreateVirtualSinkAct *result = new ZeusCreateVirtualSinkAct(name, props);

  return result;
}

void ZeusCreateVirtualSinkDialog::connectToPulseData(ZeusPulseData *pd) {}
