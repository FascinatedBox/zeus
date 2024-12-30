#include "dialogs/destroyvirtualsinkdialog.h"
#include "core/utils.h"
#include "widgets/virtualdevicecombobox.h"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>

ZeusDestroyVirtualSinkDialog::ZeusDestroyVirtualSinkDialog(ZeusPulseData *pd,
                                                           QWidget *parent)
    : ZeusBaseDialog(parent) {
  QVBoxLayout *layout = new QVBoxLayout;
  QFormLayout *formLayout = new QFormLayout;
  m_sinkCombo = new ZeusVirtualDeviceComboBox(pd);

  formLayout->addRow("Sink to destroy:", m_sinkCombo);
  layout->addLayout(formLayout);
  layout->addWidget(m_buttonBox);
  setLayout(layout);
  setWindowTitle("Destroy virtual sink");
}

bool ZeusDestroyVirtualSinkDialog::isValid(void) {
  return (m_sinkCombo->currentIndex() != -1);
}

ZeusDestroyVirtualSinkAct *ZeusDestroyVirtualSinkDialog::makeAction(void) {
  QString sinkName = m_sinkCombo->currentDeviceName();

  return new ZeusDestroyVirtualSinkAct(sinkName);
}
