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

  // Must match the order of ZeusQueryMatchType fields.
  m_actionCombo->addItem("equals", ZeusQueryMatchType::MTEqual);
  m_actionCombo->addItem("not equal", ZeusQueryMatchType::MTNotEqual);
  m_actionCombo->setCurrentIndex(0);
  layout->addWidget(m_keyLine);
  layout->addWidget(m_actionCombo);
  layout->addWidget(m_valueLine);
  addControlButtonsToLayout(layout);
  setLayout(layout);
}

void ZeusQueryPropertyEntry::load(QString k, ZeusQueryMatchType t, QString v) {
  m_keyLine->setText(k);
  m_actionCombo->setCurrentIndex((int)t);
  m_valueLine->setText(v);
}

void ZeusQueryPropertyEntry::reset(void) {
  m_keyLine->setText("");
  m_actionCombo->setCurrentIndex(0);
  m_valueLine->setText("");
}

std::tuple<QString, ZeusQueryMatchType, QString>
ZeusQueryPropertyEntry::toQueryLine(void) {
  QString key = m_keyLine->text().simplified();
  ZeusQueryMatchType matchType =
      static_cast<ZeusQueryMatchType>(m_actionCombo->currentData().toInt());
  QString value = m_valueLine->text();

  return std::make_tuple(key, matchType, value);
}

ZeusQueryPropertyGroupBox::ZeusQueryPropertyGroupBox(ZeusPulseData *pd,
                                                     const QString &title,
                                                     ZeusPulseInfoType type)
    : ZeusBasePropertyGroupBox(title), m_pd(pd), m_type(type) {
  onAddEntry(nullptr);
  m_previewTree = new ZeusQueryPreviewTree(pd, type);
  auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Apply);

  connect(buttonBox, &QDialogButtonBox::clicked, this,
          &ZeusQueryPropertyGroupBox::onApply);
  m_layout->addWidget(buttonBox);
  m_layout->addWidget(new QLabel("Preview"));
  m_layout->addWidget(m_previewTree);
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

void ZeusQueryPropertyGroupBox::loadQuery(ZeusPulseQuery *query) {
  auto iter = query->lineIterator();
  int i = 0;

  while (iter.hasNext()) {
    auto line = iter.next();
    auto entry = new ZeusQueryPropertyEntry;

    entry->load(std::get<0>(line), std::get<1>(line), std::get<2>(line));
    insertEntry(i, entry);
    i++;
  }
}

void ZeusQueryPropertyGroupBox::onAddEntry(QWidget *source) {
  int index = m_layout->indexOf(source) + 1;
  auto entry = new ZeusQueryPropertyEntry;

  insertEntry(index, entry);
}

void ZeusQueryPropertyGroupBox::onApply(QAbstractButton *) {
  auto query = intoQuery();
  auto selections = m_pd->selectStreams(m_type, query);

  m_previewTree->markSelected(selections);
  delete query;
}
