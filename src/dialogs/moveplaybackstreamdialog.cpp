#include "dialogs/moveplaybackstreamdialog.h"
#include "core/pulsedata.h"
#include "widgets/devicecombobox.h"
#include "widgets/querypropertygroupbox.h"
#include <QDialogButtonBox>
#include <QFormLayout>

ZeusMovePlaybackStreamDialog::ZeusMovePlaybackStreamDialog(ZeusPulseData *pd,
                                                           QWidget *parent)
    : ZeusBaseDialog(parent) {
  QFormLayout *layout = new QFormLayout;
  m_sinkCombo = new ZeusDeviceComboBox(pd, ZISink);
  m_queryBox = new ZeusQueryPropertyGroupBox(pd, "Selected streams");

  layout->addRow(m_queryBox);
  layout->addRow("Playback Device:", m_sinkCombo);
  layout->addRow(m_buttonBox);
  setLayout(layout);
  setWindowTitle("Move playback stream");
}

bool ZeusMovePlaybackStreamDialog::isValid(void) {
  return (m_sinkCombo->currentIndex() == -1);
}

ZeusMovePlaybackStreamAct *ZeusMovePlaybackStreamDialog::makeAction(void) {
  ZeusPulseQuery *query = m_queryBox->intoQuery();
  QString sinkName = m_sinkCombo->currentDeviceName();

  return new ZeusMovePlaybackStreamAct(query, sinkName);
}
