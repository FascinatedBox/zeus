#include "widgets/querypropertygroupbox.h"
#include "core/pulsequery.h"
#include "core/utils.h"
#include "widgets/querypreviewtree.h"
#include <QComboBox>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

ZeusQueryPropertyEntry::ZeusQueryPropertyEntry(void) {
  QHBoxLayout *layout = new QHBoxLayout;
  m_keyLine = new QLineEdit;
  m_actionCombo = new QComboBox;
  m_valueLine = new QLineEdit;

  m_actionCombo->addItem("equals", ZeusQueryMatchType::MTEqual);
  m_actionCombo->addItem("not equal", ZeusQueryMatchType::MTNotEqual);
  m_actionCombo->setCurrentIndex(0);
  layout->addWidget(m_keyLine);
  layout->addWidget(m_actionCombo);
  layout->addWidget(m_valueLine);
  addControlButtonsToLayout(layout);
  setLayout(layout);
}

std::tuple<QString, ZeusQueryMatchType, QString>
ZeusQueryPropertyEntry::toQueryLine(void) {
  QString key = m_keyLine->text().simplified();
  ZeusQueryMatchType matchType =
      static_cast<ZeusQueryMatchType>(m_actionCombo->currentData().toInt());
  QString value = m_valueLine->text();

  return std::make_tuple(key, matchType, value);
}

ZeusQueryPropertyGroupBox::ZeusQueryPropertyGroupBox(const QString &title)
    : ZeusBasePropertyGroupBox(title) {
  onAddEntry(nullptr);
  m_previewTree = new ZeusQueryPreviewTree;
  auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Apply);

  connect(buttonBox, &QDialogButtonBox::clicked, this,
          &ZeusQueryPropertyGroupBox::onApply);
  m_layout->addWidget(new QLabel("Preview"));
  m_layout->addWidget(m_previewTree);
  m_layout->addWidget(buttonBox);
}

void ZeusQueryPropertyGroupBox::connectToPulseData(ZeusPulseData *pd) {
  m_pd = pd;
  m_previewTree->connectToPulseData(pd);
}

ZeusQueryPropertyEntry *ZeusQueryPropertyGroupBox::implNewEntry(void) {
  return new ZeusQueryPropertyEntry;
}

ZeusPulseQuery *ZeusQueryPropertyGroupBox::intoQuery(void) {
  ZeusPulseQuery *result = new ZeusPulseQuery;

  for (int i = 0; i < entryCount(); ++i) {
    QWidget *w = m_layout->itemAt(i)->widget();
    auto line = static_cast<ZeusQueryPropertyEntry *>(w)->toQueryLine();

    if (::isValidPropertyKV(std::get<0>(line), std::get<2>(line)) == false)
      continue;

    result->addQueryLine(line);
  }

  return result;
}

void ZeusQueryPropertyGroupBox::onApply(QAbstractButton *) {
  auto query = intoQuery();
  auto selections = m_pd->selectPlayback(query);

  m_previewTree->markSelected(selections);
  delete query;
}
