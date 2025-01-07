#include "dialogs/movestreamdialog.h"
#include "core/pulsedata.h"
#include "widgets/devicecombobox.h"
#include "widgets/querypropertygroupbox.h"
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QStackedWidget>
#include <QVBoxLayout>

ZeusMoveStreamDialog::ZeusMoveStreamDialog(ZeusPulseData *pd, QWidget *parent)
    : ZeusBaseDialog(parent) {
  QHBoxLayout *radioLayout = new QHBoxLayout;
  QVBoxLayout *layout = new QVBoxLayout;
  m_stack = new QStackedWidget;
  QRadioButton *playbackRadio = new QRadioButton("Playback");
  QRadioButton *recordRadio = new QRadioButton("Record");

  radioLayout->addWidget(playbackRadio);
  radioLayout->addWidget(recordRadio, Qt::AlignLeft);
  loadStackWidget(pd, 0);
  loadStackWidget(pd, 1);
  layout->addLayout(radioLayout);
  layout->addWidget(m_stack);
  layout->addWidget(m_buttonBox);
  setLayout(layout);
  setWindowTitle("Move stream");
  connect(playbackRadio, &QAbstractButton::pressed, this,
          &ZeusMoveStreamDialog::playbackSelected);
  connect(recordRadio, &QAbstractButton::pressed, this,
          &ZeusMoveStreamDialog::recordSelected);
  playbackRadio->setChecked(true);
}

void ZeusMoveStreamDialog::loadStackWidget(ZeusPulseData *pd, int index) {
  ZeusPulseInfoType comboType = (index == 0 ? ZISink : ZISource);
  ZeusPulseInfoType groupType = (index == 0 ? ZISinkInput : ZISourceOutput);
  QWidget *w = new QWidget;
  QVBoxLayout *layout = new QVBoxLayout;
  auto combo = new ZeusDeviceComboBox(pd, comboType);
  auto group = new ZeusQueryPropertyGroupBox(pd, "Selected streams", groupType);

  m_combos[index] = combo;
  m_groups[index] = group;
  layout->addWidget(group);
  layout->addWidget(combo);
  w->setLayout(layout);
  m_stack->addWidget(w);
}

bool ZeusMoveStreamDialog::isValid(void) {
  return m_combos[m_stack->currentIndex()]->currentIndex() != -1;
}

ZeusMoveStreamAct *ZeusMoveStreamDialog::makeAction(void) {
  const char *types[] = {"playback", "record"};
  int index = m_stack->currentIndex();
  ZeusPulseQuery *query = m_groups[index]->intoQuery();
  QString target = m_combos[index]->currentDeviceName();
  QString type = types[index];

  return new ZeusMoveStreamAct(query, type, target);
}

void ZeusMoveStreamDialog::playbackSelected(void) {
  m_stack->setCurrentIndex(0);
}

void ZeusMoveStreamDialog::recordSelected(void) { m_stack->setCurrentIndex(1); }
