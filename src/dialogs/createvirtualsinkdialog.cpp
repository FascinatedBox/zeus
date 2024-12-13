#include "dialogs/createvirtualsinkdialog.h"
#include "core/utils.h"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>

ZeusCreateVirtualSinkDialog::ZeusCreateVirtualSinkDialog(QWidget *parent)
    : ZeusBaseDialog(parent) {
  QVBoxLayout *layout = new QVBoxLayout;
  QFormLayout *formLayout = new QFormLayout;
  m_nameLine = new QLineEdit;

  formLayout->addRow("Name:", m_nameLine);
  layout->addLayout(formLayout);
  layout->addWidget(m_buttonBox);
  setLayout(layout);
  setWindowTitle("Create virtual sink");
}

bool ZeusCreateVirtualSinkDialog::isValid(void) {
  return ::isValidPulseDeviceName(m_nameLine->text());
}

ZeusCreateVirtualSinkAct *ZeusCreateVirtualSinkDialog::makeAction(void) {
  QString name = m_nameLine->text();

  ZeusCreateVirtualSinkAct *result = new ZeusCreateVirtualSinkAct(name);

  return result;
}

void ZeusCreateVirtualSinkDialog::connectToPulseData(ZeusPulseData *pd) {}
