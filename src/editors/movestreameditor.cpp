#include "editors/movestreameditor.h"
#include "core/pulsedata.h"
#include "widgets/devicecombobox.h"
#include "widgets/querypropertygroupbox.h"
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QStackedWidget>
#include <QVBoxLayout>

ZeusMoveStreamEditor::ZeusMoveStreamEditor(ZeusPulseData *pd, QWidget *parent)
    : ZeusBaseEditor(parent) {
  QHBoxLayout *radioLayout = new QHBoxLayout;
  QVBoxLayout *layout = new QVBoxLayout;
  m_stack = new QStackedWidget;
  m_playback = new QRadioButton("Playback");
  QRadioButton *recordRadio = new QRadioButton("Record");

  radioLayout->addWidget(m_playback);
  radioLayout->addWidget(recordRadio, Qt::AlignLeft);
  loadStackWidget(pd, 0);
  loadStackWidget(pd, 1);
  layout->addLayout(radioLayout);
  layout->addWidget(m_stack);
  layout->addWidget(m_buttonBox);
  setLayout(layout);
  connect(m_playback, &QAbstractButton::pressed, this,
          &ZeusMoveStreamEditor::playbackSelected);
  connect(recordRadio, &QAbstractButton::pressed, this,
          &ZeusMoveStreamEditor::recordSelected);
  m_playback->setChecked(true);
}

void ZeusMoveStreamEditor::loadStackWidget(ZeusPulseData *pd, int index) {
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

bool ZeusMoveStreamEditor::isValid(void) {
  return m_combos[m_stack->currentIndex()]->currentIndex() != -1;
}

void ZeusMoveStreamEditor::loadAction(ZeusBaseAction *act) {
  auto a = static_cast<ZeusMoveStreamAct *>(act);
  int index = (a->type == "record");

  m_stack->setCurrentIndex(index);
  m_combos[index]->setCurrentDeviceByName(a->target);
  m_groups[index]->loadQuery(a->query);
}

ZeusMoveStreamAct *ZeusMoveStreamEditor::makeAction(void) {
  const char *types[] = {"playback", "record"};
  int index = m_stack->currentIndex();
  ZeusPulseQuery *query = m_groups[index]->intoQuery();
  QString target = m_combos[index]->currentDeviceName();
  QString type = types[index];

  return new ZeusMoveStreamAct(query, type, target);
}

void ZeusMoveStreamEditor::playbackSelected(void) {
  m_stack->setCurrentIndex(0);
}

void ZeusMoveStreamEditor::recordSelected(void) { m_stack->setCurrentIndex(1); }

void ZeusMoveStreamEditor::reset(void) {
  m_playback->setChecked(true);
  m_combos[0]->setCurrentIndex(0);
  m_combos[1]->setCurrentIndex(0);
  m_groups[0]->reset();
  m_groups[1]->reset();
}
