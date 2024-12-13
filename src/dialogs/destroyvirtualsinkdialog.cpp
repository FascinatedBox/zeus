#include "dialogs/destroyvirtualsinkdialog.h"
#include "core/utils.h"
#include "widgets/devicecombobox.h"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <pulse/def.h>

#define IS_VIRTUAL_SINK(device) ((device->flags & PA_SOURCE_HARDWARE) == 0)

ZeusDestroyVirtualSinkDialog::ZeusDestroyVirtualSinkDialog(QWidget *parent)
    : ZeusBaseDialog(parent) {
  QVBoxLayout *layout = new QVBoxLayout;
  QFormLayout *formLayout = new QFormLayout;
  m_sinkCombo = new ZeusDeviceComboBox;

  formLayout->addRow("Sink to destroy:", m_sinkCombo);
  layout->addLayout(formLayout);
  layout->addWidget(m_buttonBox);
  setLayout(layout);
  setWindowTitle("Destroy virtual sink");
}

void ZeusDestroyVirtualSinkDialog::loadDeviceList(ZeusPulseData *pd) {
  QMapIterator<uint32_t, ZeusPulseDeviceInfo *> iter = pd->sinkIterator();

  while (iter.hasNext()) {
    iter.next();

    if (IS_VIRTUAL_SINK(iter.value()) == false)
      continue;

    m_sinkCombo->addDevice(iter.key(), iter.value()->name, iter.value()->desc);
  }
}

bool ZeusDestroyVirtualSinkDialog::isValid(void) {
  return (m_sinkCombo->currentIndex() != -1);
}

ZeusDestroyVirtualSinkAct *ZeusDestroyVirtualSinkDialog::makeAction(void) {
  QString sinkName = m_sinkCombo->currentDeviceName();
  ZeusDestroyVirtualSinkAct *result = new ZeusDestroyVirtualSinkAct(sinkName);
  return result;
}

void ZeusDestroyVirtualSinkDialog::onSinkAdded(uint32_t index,
                                               ZeusPulseDeviceInfo *info) {
  if (IS_VIRTUAL_SINK(info) == false)
    return;

  m_sinkCombo->addDevice(index, info->name, info->desc);
}

void ZeusDestroyVirtualSinkDialog::onSinkRemoved(uint32_t index) {
  // Will be ignored if the device isn't there.
  m_sinkCombo->removeDevice(index);
}

void ZeusDestroyVirtualSinkDialog::connectToPulseData(ZeusPulseData *pd) {
  connect(pd, &ZeusPulseData::sinkAdded, this,
          &ZeusDestroyVirtualSinkDialog::onSinkAdded);
  connect(pd, &ZeusPulseData::sinkRemoved, this,
          &ZeusDestroyVirtualSinkDialog::onSinkRemoved);

  loadDeviceList(pd);
}
